NAME := ircserv

SRCS := src/main.cpp src/IRCServer.cpp src/Client.cpp src/Channel.cpp\
		src/CommandHandler.cpp src/IRCMessage.cpp src/Socket.cpp \
		src/IRCParser.cpp src/Utils.cpp src/IOWrapper.cpp src/IRCLogger.cpp
OBJS := $(SRCS:.cpp=.o)
CXXFLAGS += -I./include -Wall -Wextra -Werror -O0 -g -std=c++98 -pedantic-errors -DDEBUG
ifdef PROD_FLG
CXXFLAGS := -I./include -Wall -Wextra -Werror -O2 -std=c++98 -pedantic-errors
endif
CXX := c++

.PHONY: all
all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $(NAME)

.PHONY: clean
clean:
	$(RM) $(OBJS)

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)

.PHONY: re
re: fclean all

.PHONY: unit_test
unit_test:
	cd test/unit \
	&& cmake -S . -B build \
	&& cmake --build build \
	&& cd build \
	&& ctest

.PHONY: e2e_test
e2e_test: all
	cd test/e2e \
	&& pytest -vs

.PHONY: ngircd
ngircd:
	sudo service ngircd restart

.PHONY: server
server: all
	./$(NAME) 6677 pass123

.PHONY: client
client: all
	@printf "Please enter a nickname: "; \
	read NICK; \
	irssi --connect=127.0.0.1 --port=6677 --nick=$$NICK --password=pass123

.PHONY: nc
nc: all
	nc -C -4 127.0.0.1 6677

.PHONY: ncv6
ncv6: all
	nc -C -6 ::1 6677
