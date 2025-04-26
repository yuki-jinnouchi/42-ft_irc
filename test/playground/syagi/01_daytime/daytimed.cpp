#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <iostream>

#define MAX_BACKLOG 5

//
static int listen_socket(int port) {
  struct addrinfo hints = {0}, *res, *ai;
  hints.ai_family = AF_INET;  // IPv4
  // hints.ai_family = AF_INET6;  // IPv6
  // netcatでIPv6で接続する方法
  // nc -6 ::1 <port>
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;  // 自分のIPアドレスを使用

  int ret = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &res);
  if (ret != 0) {
    std::cerr << "Error: getaddrinfo: " << gai_strerror(ret) << std::endl;
    exit(EXIT_FAILURE);
  }
  for (ai = res; ai != NULL; ai = ai->ai_next) {
    int sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (sockfd < 0)
      continue;
    // ソケット再利用
    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
      perror("setsockopt");
      close(sockfd);
      continue;
    }
    if (bind(sockfd, ai->ai_addr, ai->ai_addrlen) < 0) {
      close(sockfd);
      continue;
    }
    if (listen(sockfd, MAX_BACKLOG) < 0) {
      close(sockfd);
      continue;
    }
    std::cerr << "Listening on port " << port << "..." << std::endl;
    freeaddrinfo(res);  // アドレス情報の解放
    return sockfd;      // 成功したソケットのファイルディスクリプタを返す
  }
  std::cerr << "Error: socket/bind/listen failed" << std::endl;
  freeaddrinfo(res);  // アドレス情報の解放
  exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
    exit(EXIT_FAILURE);
  }
  int server = listen_socket(atoi(argv[1]));

  sockaddr_storage addr;
  socklen_t addrlen = sizeof(addr);
  std::cout << "------1" << std::endl;
  int sockfd = accept(server, (struct sockaddr*)&addr, &addrlen);
  std::cout << "------2" << std::endl;
  if (sockfd < 0) {
    perror("accept(2)");
    exit(EXIT_FAILURE);
  }
  time_t t;
  time(&t);
  char* timestr = ctime(&t);
  write(sockfd, timestr, strlen(timestr));
  close(sockfd);
  close(server);
  return 0;
}
