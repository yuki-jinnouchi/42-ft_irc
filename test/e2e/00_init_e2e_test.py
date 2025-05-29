import socket
import threading
import time
import pytest
import subprocess

SERVER_HOST = "127.0.0.1"
SERVER_PORT = 6677
SERVER_PASSWORD = "pass123"


## テスト開始時にサーバーを起動
def launch_server():
    """
    Launch the IRC server in a new process.
    """
    # Start the server with the specified port and password
    with open("e2e.log", "w") as logfile:
        process = subprocess.Popen(
            ["../../ircserv", str(SERVER_PORT), SERVER_PASSWORD],
            stdout=logfile,
            stderr=subprocess.STDOUT,
        )
    return process


@pytest.fixture(scope="session", autouse=True)
def server_process():
    """
    テストセッション開始前にサーバーを起動し、セッション終了時にサーバーを停止するフィクスチャ。
    """
    process = launch_server()
    # サーバーが起動するまで少し待機（適宜調整）
    time.sleep(2)
    yield process
    process.terminate()
    process.wait()

def test_dummy():
    assert True, "サーバーを起動するためのダミーのテスト."
