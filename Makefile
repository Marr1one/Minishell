CC = cc -g3
CFLAGS = -Wall -Wextra 

NAME = minishell
OBJDIR = obj

SRC = $(shell find . -type f -name "*.c")

OBJ = $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))

$(shell mkdir -p $(OBJDIR))

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lreadline
	@echo "*----------------------*"
	@echo "\033[1;32mCompilation completed 🎉\033[0m"
	@echo "*----------------------*"

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

.PHONY: clean fclean re