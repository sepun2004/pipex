NAME = pipex
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -f

SRCS = /src/main.c

OBJS = $(SRCS:.c=.o)

all: show_progress $(NAME)

$(NAME): $(OBJ)
	@make bonus -s -C libft
	@make -s -C libft
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -L libft -lft
	@echo "$(GREEN)It has been compiled, have a nice day.👍$(NC)";

%.o: %.c
	$(eval CURRENT_FILE=$(shell echo $$(($(CURRENT_FILE) + 1))))
	@printf "Compiling $<... $(shell echo $$(($(CURRENT_FILE) * 100 / $(TOTAL_FILES))))%%\r"
	@$(CC) $(CFLAGS) -c $< -o $@ 

show_progress:
	@if [ -f $(NAME) ]; then \
		echo "$(GREEN)The compiled code is already updated.$(NC)"; \
	fi

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
