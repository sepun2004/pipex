GREEN = \033[0;32m
RED = \033[0;31m
NC = \033[0m
CURRENT_FILE = 0
TOTAL_FILES = $(words $(SRC))

NAME = pipex
CC = gcc -g
CFLAGS = -Wall -Wextra -Werror 
RM = rm -f

SRCS = ./src/pipex.c \
		./src/utils.c\
		./src/commands.c \

OBJ = $(SRCS:.c=.o)

all:  $(NAME) #show_progress

$(NAME): $(OBJ)
	@make -s -C libft
	@make bonus -s -C libft
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -L libft -lft
	@echo "$(GREEN)It has been compiled, have a nice day.👍$(NC)";

# %.o: %.c
# 	$(eval CURRENT_FILE=$(shell echo $$(($(CURRENT_FILE) + 1))))
# 	@printf "Compiling $<... $(shell echo $$(($(CURRENT_FILE) * 100 / $(TOTAL_FILES))))%%\r"
# 	@$(CC) $(CFLAGS) -c $< -o $@ 

show_progress:
	@if [ -f $(NAME) ]; then \
		echo "$(GREEN)The compiled code is already updated.$(NC)"; \
	fi

norm:
	@norminette $(SRCS) ./include/pipex.h

clean: 
	@$(RM) $(OBJ)
	@make  clean -s -C libft

fclean: clean
	@rm -f $(NAME)
	@make fclean -s -C libft
	@echo "$(RED)Full Cleaning...$(NC)"
	@echo "$(RED)Full Cleaned!$(NC)"


re: fclean all

.PHONY: all clean fclean re
