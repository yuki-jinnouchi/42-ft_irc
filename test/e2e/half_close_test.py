import socket
import threading
import time
import pytest
import subprocess

SERVER_HOST = "127.0.0.1"
SERVER_PORT = 6677
SERVER_PASSWORD = "pass123"

def connect_client(is_send_msg=True, is_hang=False):
    """
    サーバーに接続し、必要に応じてメッセージ送信またはハング状態をシミュレートする。
    is_send_msg が True の場合は "PING\r\n" を送信する。
    is_hang が True の場合は、接続後に何もせず一定時間待機する。
    """
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((SERVER_HOST, SERVER_PORT))
    if is_send_msg:
        s.sendall(b"PING abc\r\n")
    if is_hang:
        # ハング状態をシミュレート　一定時間スリープ
        time.sleep(10)

    if is_send_msg:
        try:
            data = s.recv(1024)
            assert data == b":irc.example.net PONG irc.example.net :abc\r\n", f"クライアントからの応答が不正: {data}"
        except Exception as e:
            pytest.fail(f"正常なクライアントの受信に失敗: {e}")
    return s

def connect_client_half_close(half_close_read=False, half_close_write=False):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((SERVER_HOST, SERVER_PORT))
    # s.sendall(b"PING\r\n")
    msg = b"hello\r\n"
    # msg = b"PING\r\n"
    s.sendall(msg)
    if half_close_read:
        # 半閉（読み取り側を閉じる）
        s.shutdown(socket.SHUT_RD)
    if half_close_write:
        # 半閉（書き込み側を閉じる）
        s.shutdown(socket.SHUT_WR)
    return s

def test_half_close():
    """
    半閉状態でのクライアント接続テスト。
    """
    num_clients = 100
    client_sockets = []
    threads = []

    def client_worker(index):
        try:
            if index % 3 == 0:
                # 半閉状態(読み込み)で接続
                s = connect_client_half_close(half_close_read=True, half_close_write=False)
                client_sockets.append(s)
            elif index % 3 == 1:
                # 半閉状態(書き込み)で接続
                s = connect_client_half_close(half_close_read=False, half_close_write=True)
                client_sockets.append(s)
            else:
                # 通常接続
                s = connect_client_half_close(half_close_read=False, half_close_write=False)
                client_sockets.append(s)
        except Exception as e:
            pytest.fail(f"Client {index} failed to connect: {e}")

    for i in range(num_clients):
        t = threading.Thread(target=client_worker, args=(i,))
        threads.append(t)
        t.start()
    for t in threads:
        t.join()

    assert len(client_sockets) == num_clients, "全てのクライアントが接続できなかった"

    for s in client_sockets:
        s.close()

    time.sleep(5)

    # -----------------------------------------------------
    # SIGPIPEを無視する処理をいれないと、サーバーが停止するため、
    # 以降のテストが落ちる
    # -----------------------------------------------------

    client_sockets2 = []
    threads2 = []

    def client_worker2(index):
        try:
            s = connect_client()
            client_sockets2.append(s)
        except Exception as e:
            pytest.fail(f"Client {index} failed to connect: {e}")

    for i in range(num_clients):
        t = threading.Thread(target=client_worker2, args=(i,))
        threads2.append(t)
        t.start()
    for t in threads2:
        t.join()

    for s in client_sockets2:
        s.close()

    assert len(client_sockets2) == num_clients, "全てのクライアントが接続できなかった2"




