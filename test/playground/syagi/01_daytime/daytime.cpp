#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <iostream>

#define BUFFER_SIZE 1024

static int open_connection(const char* host, const char* service) {
  struct addrinfo hints, *res, *p;
  int sockfd;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;      // IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;  // TCP

  // ホスト名、ポート番号でアドレス情報を取得
  int ret = getaddrinfo(host, service, &hints, &res);
  if (ret != 0) {
    std::cerr << "Error: getaddrinfo: " << gai_strerror(ret) << std::endl;
    exit(EXIT_FAILURE);
  }

  // 使用可能なアドレスを探す
  for (p = res; p != NULL; p = p->ai_next) {
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockfd == -1) {
      continue;  // ソケット作成失敗
    }

    if (connect(sockfd, p->ai_addr, p->ai_addrlen) < 0) {
      // 接続失敗
      close(sockfd);
      continue;
    }
    freeaddrinfo(res);  // アドレス情報の解放
    return sockfd;      // 成功したソケットのファイルディスクリプタを返す
  }
  // すべてのアドレスで接続失敗
  std::cerr << "Error: socket/connect failed" << std::endl;
  freeaddrinfo(res);  // アドレス情報の解放
  exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <hostname> <port>" << std::endl;
    exit(EXIT_FAILURE);
  }
  int sockfd = open_connection(argv[1], argv[2]);
  if (sockfd < 0) {
    std::cerr << "Error: open_connection failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  char buf[BUFFER_SIZE];
  std::cout << "Received: " << std::endl;
  while (true) {
    ssize_t bytes_read = read(sockfd, buf, sizeof(buf) - 1);
    if (bytes_read <= 0) {
      break;  // 読み込み失敗または接続終了
    }
    buf[bytes_read] = '\0';  // NULL終端
    std::cout << buf << std::endl;
  }

  close(sockfd);
  return 0;
}
