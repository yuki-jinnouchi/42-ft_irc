NAME := daytime.bin
NAMED := daytimed.bin

.PHONY: all
all:
	$(CXX) daytime.cpp -o $(NAME)
	$(CXX) daytimed.cpp -o $(NAMED)

.PHONY: fclean
fclean:
	$(RM) $(NAME)
	$(RM) $(NAMED)

.PHONY: re
re: fclean all
