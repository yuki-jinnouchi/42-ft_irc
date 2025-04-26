NAME := ircserv

SRCS := src/main.cpp
OBJS := $(SRCS:.cpp=.o)
CXXFLAGS += -I./include -Wall -Wextra -Werror -O0 -g -std=c++98 -pedantic-errors
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
