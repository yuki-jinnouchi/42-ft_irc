/*
Usage

Terminal 1:
$ mkfifo p q
$ ./a.out p q
バックグラウンドへ(ctrl + z)

Terminal 2:
$ cat > p
バックグラウンドへ(ctrl + z)
$ cat > q
hello
world
バックグラウンドへ(ctrl + z)
$ fg %1
12345
67890

Terminal 1:
$ fg

*/

#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>

// Maximum bytes fetched by a single read()
#define MAX_BUF 1024
// #define MAX_BUF 3
// Maximum number of events to be returned from a single epoll_wait() call
#define MAX_EVENTS 5

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " file...\n";
  }

  //   int epfd = epoll_create(argc - 1);
  // epoll_create1の方が良いらしい
  // https://ja.manpages.org/epoll_create/2
  int epfd = epoll_create1(EPOLL_CLOEXEC);
  if (epfd == -1) {
    std::cerr << "epoll_create failed\n";
    exit(EXIT_FAILURE);
  }

  for (int j = 1; j < argc; j++) {
    int fd = open(argv[j], O_RDONLY);
    if (fd == -1) {
      std::cerr << "open failed\n" << argv[j] << std::endl;
      exit(EXIT_FAILURE);
    }
    std::cout << "Opened \"" << argv[j] << "\" on fd " << fd << std::endl;

    struct epoll_event ev;
    ev.events = EPOLLIN; /* Only interested in input events */
    ev.data.fd = fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
      std::cerr << "epoll_ctl failed" << std::endl;
    }
  }

  int numOpenFds = argc - 1;

  while (numOpenFds > 0) {
    /* Fetch up to MAX_EVENTS items from the ready list of the
       epoll instance */

    printf("About to epoll_wait()\n");

    struct epoll_event evlist[MAX_EVENTS];

    int ready = epoll_wait(epfd, evlist, MAX_EVENTS, -1);
    if (ready == -1) {
      if (errno == EINTR) {
        continue; /* Restart if interrupted by signal */
      } else {
        std::cerr << "epoll_wait failed" << std::endl;
      }
    }

    std::cout << "Ready: " << ready << std::endl;

    /* Deal with returned list of events */
    for (int j = 0; j < ready; j++) {
      std::cout << "  fd=" << evlist[j].data.fd << "; events: "
                << ((evlist[j].events & EPOLLIN) ? "EPOLLIN " : "")
                << ((evlist[j].events & EPOLLHUP) ? "EPOLLHUP " : "")
                << ((evlist[j].events & EPOLLERR) ? "EPOLLERR " : "")
                << std::endl;

      if (evlist[j].events & EPOLLIN) {
        char buf[MAX_BUF];
        int s = read(evlist[j].data.fd, buf, MAX_BUF);
        if (s == -1) {
          std::cerr << "read failed" << std::endl;
          exit(EXIT_FAILURE);
        }
        std::cout << "    read " << s << " bytes: " << std::string(buf, s)
                  << std::endl;

      } else if (evlist[j].events & (EPOLLHUP | EPOLLERR)) {
        /* After the epoll_wait(), EPOLLIN and EPOLLHUP may both have
           been set. But we'll only get here, and thus close the file
           descriptor, if EPOLLIN was not set. This ensures that all
           outstanding input (possibly more than MAX_BUF bytes) is
           consumed (by further loop iterations) before the file
           descriptor is closed. */

        std::cout << "    closing fd " << evlist[j].data.fd << std::endl;

        if (close(evlist[j].data.fd) == -1) {
          std::cerr << "close failed" << std::endl;
          exit(EXIT_FAILURE);
        }
        numOpenFds--;
      }
    }
  }

  std::cout << "All file descriptors closed; bye" << std::endl;
  return 0;
}
