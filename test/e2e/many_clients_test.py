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


def test_many_clients():
    """
    多数クライアントが同時に接続でき、各クライアントが PING を送信できるかを確認する。
    """
    num_clients = 10000
    client_sockets = []
    threads = []

    def client_worker(index):
        try:
            s = connect_client(is_send_msg=True, is_hang=False)
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


def test_hanging_client():
    """
    ハングしているクライアントが含まれている場合
    """
    num_clients = 10000
    client_sockets = []
    hanging_sockets = []
    threads = []

    def client_worker(index):
        try:
            if index % 3 == 0:
                s = connect_client(is_send_msg=False, is_hang=True)
                hanging_sockets.append(s)
            elif index % 3 == 1:
                s = connect_client(is_send_msg=True, is_hang=True)
                hanging_sockets.append(s)
            else:
                s = connect_client(is_send_msg=True, is_hang=False)
                client_sockets.append(s)
        except Exception as e:
            pytest.fail(f"Client {index} failed to connect: {e}")

    for i in range(num_clients):
        t = threading.Thread(target=client_worker, args=(i,))
        threads.append(t)
        t.start()
    for t in threads:
        t.join()

    assert (
        len(client_sockets) + len(hanging_sockets)
    ) == num_clients, "全てのクライアントが接続できなかった"


    for s in client_sockets:
        s.close()
    for s in hanging_sockets:
        s.close()
