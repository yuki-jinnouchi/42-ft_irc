NAME := ircserv

SRCS := src/main.cpp src/IRCServer.cpp src/ClientSession.cpp \
		src/CommandHandler.cpp src/IRCMessage.cpp src/Socket.cpp \
		src/IRCParser.cpp
OBJS := $(SRCS:.cpp=.o)
CXXFLAGS += -I./include -Wall -Wextra -Werror -O0 -g -std=c++98 -pedantic-errors -DDEBUG
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

.PHONY: run
run: all
	./$(NAME) 6677 pass123

.PHONY: unit_test
unit_test:
	cd test/unit \
	&& cmake -S . -B build \
	&& cmake --build build \
	&& cd build \
	&& ctest

.PHONY: ngircd
ngircd:
	sudo service ngircd restart
