# Output File
NAME := ./ircserv

# Directories
INCL_ROOT_DIR =	include
INCL_DIR := $(INCL_ROOT_DIR)
INCL_DIR += $(shell find $(INCL_ROOT_DIR) -mindepth 1 -type d)
SRCS_ROOT_DIR = src
SRCS_DIR = $(SRCS_ROOT_DIR)
SRCS_DIR +=	$(shell find $(SRCS_ROOT_DIR) -mindepth 1 -type d)
OBJS_ROOT_DIR =	objs
OBJS_DIR =	$(patsubst $(SRCS_ROOT_DIR)%,$(OBJS_ROOT_DIR)%,$(SRCS_DIR))

# Files
SRCS := src/main.cpp src/IRCServer.cpp src/Client.cpp src/Channel.cpp\
		src/RequestHandler.cpp src/IRCMessage.cpp src/Socket.cpp \
		src/IRCParser.cpp src/Utils.cpp src/IOWrapper.cpp src/IRCLogger.cpp \
		src/IRCSignal.cpp \
		src/commands/ACommand.cpp src/commands/CommandCap.cpp \
		src/commands/CommandNick.cpp src/commands/CommandPass.cpp \
		src/commands/CommandUser.cpp src/commands/CommandPing.cpp \
		src/commands/CommandJoin.cpp src/commands/CommandTopic.cpp \
		src/commands/CommandMode.cpp src/commands/CommandInvite.cpp \
		src/commands/CommandPart.cpp src/commands/CommandPrivMsg.cpp \
		src/commands/CommandKick.cpp \
		src/commands/CommandNull.cpp src/commands/CommandBroadCast.cpp
OBJS = $(patsubst $(SRCS_ROOT_DIR)/%.cpp,$(OBJS_ROOT_DIR)/%.o,$(SRCS))

#-----------------------------------------------
CXX = c++

INCL_FLAGS = $(foreach d, $(INCL_DIR), -I$d)
W3_FLAGS = -Wall -Wextra -Werror
OTHER_FLAGS = -O0 -g -std=c++98 -pedantic-errors -DDEBUG
CXXFLAGS = $(INCL_FLAGS) $(W3_FLAGS) $(OTHER_FLAGS)

ifdef PROD_FLG
		CXXFLAGS := -I./include -Wall -Wextra -Werror -O2 -std=c++98 -pedantic-errors
endif

#===============================================
# Colors
DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

#===============================================
.PHONY: all
all: $(NAME)

$(NAME): $(OBJS)
	 @printf "\r$(GREEN)[%2d/%2d]$(DEF_COLOR) All files compiled! Linking $(CYAN)$(NAME)$(DEF_COLOR)\033[K\n" \
	 	"$$(find $(SRCS_ROOT_DIR) -type f | wc -l)" "$$(find $(SRCS_ROOT_DIR) -type f | wc -l)"
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "\n$(GREEN)<-------- $(NAME) linked. -------------------------->$(DEF_COLOR)"
	@chmod 777 $(NAME)
	@echo "$(GREEN)<-------- $(NAME) permission changed. -------------->$(DEF_COLOR)"
	@echo "$(CYAN)######### $(NAME) compile finished! #################$(DEF_COLOR)"

$(OBJS_ROOT_DIR)/%.o: $(SRCS_ROOT_DIR)/%.cpp | $(OBJS_DIR)
	@printf "\r$(GREEN)[%2d/%2d]$(DEF_COLOR) Compiling objects: $(CYAN)%s$(DEF_COLOR)\033[K\n" \
		"$$(find $(OBJS_ROOT_DIR) -type f | wc -l)" "$$(find $(SRCS_ROOT_DIR) -type f | wc -l)" "$<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

.PHONY: clean
clean:
	@rm -f $(OBJS)
	@rm -rf $(OBJS_DIR)
	@echo "$(GRAY)<-------- $(NAME) object files cleaned. ------------>$(DEF_COLOR)"

.PHONY: fclean
fclean: clean
	@rm -rf $(NAME)
	@echo "$(GRAY)<-------- $(NAME) .a and executable files cleaned. ->$(DEF_COLOR)"

#-----------------------------------------------
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

#-----------------------------------------------
.PHONY: ngircd
ngircd:
	sudo service ngircd restart

.PHONY: server
server: all
	valgrind --leak-check=full --error-exitcode=1 --trace-children=yes --track-fds=yes -q $(NAME) 6677 pass123

.PHONY: client
client: all
	@printf "Please enter a nickname: "; \
	read NICK; \
	irssi --connect=127.0.0.1 --port=6677 --nick=$$NICK --password=pass123

.PHONY: nc
nc: all
	nc -C -4 127.0.0.1 6677

.PHONY: ng
ng: all
	nc -C -4 127.0.0.1 6667

.PHONY: ncv6
ncv6: all
	nc -C -6 ::1 6677

#-----------------------------------------------
aclean: all clean
ac: all clean
rec: re clean
ar:
	ar rcs $(NAME) $(OBJS)

vals:
	@echo "NAME:               $(NAME)"
	@echo "-----------------------------"
	@echo "INCLUDES_ROOT_DIR:  $(INCL_ROOT_DIR)"
	@echo "INCLUDES_DIR:       $(INCL_DIR)"
	@echo "SRCS_ROOT_DIR:      $(SRCS_ROOT_DIR)"
	@echo "SRCS_DIR:           $(SRCS_DIR)"
	@echo "OBJS_ROOT_DIR:      $(OBJS_ROOT_DIR)"
	@echo "OBJS_DIR:           $(OBJS_DIR)"
	@echo "-----------------------------"
	@echo "SOURCES:            $(SRCS)"
	@echo "OBJS:               $(OBJS)"
	@echo "-----------------------------"
	@echo "CXX:                $(CXX)"
	@echo "CXXFLAGS:           $(CXXFLAGS)"

colors:
	@echo "$(DEF_COLOR)DEF_COLOR$(DEF_COLOR)"
	@echo "$(GRAY)GRAY$(DEF_COLOR)"
	@echo "$(RED)RED$(DEF_COLOR)"
	@echo "$(GREEN)GREEN$(DEF_COLOR)"
	@echo "$(YELLOW)YELLOW$(DEF_COLOR)"
	@echo "$(BLUE)BLUE$(DEF_COLOR)"
	@echo "$(MAGENTA)MAGENTA$(DEF_COLOR)"
	@echo "$(CYAN)CYAN$(DEF_COLOR)"
	@echo "$(WHITE)WHITE$(DEF_COLOR)"
