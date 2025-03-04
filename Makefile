CC = cc -g3
CFLAGS = -Wall -Wextra -Werror 

NAME = minishell
OBJDIR = obj

# Find all source files
SRC = $(shell find . -type f -name "*.c")

# Convert source files to object files in the obj directory
OBJ = $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))

# Ensure the obj directory exists
$(shell mkdir -p $(OBJDIR))

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lreadline
	@echo "*----------------------*"
	@echo "\033[1;32mCompilation completed 🎉\033[0m"
	@echo "*----------------------*"

# Rule to compile .c files into .o files in the obj directory
$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)

.PHONY: clean fclean re