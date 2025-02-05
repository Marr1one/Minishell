NAME	=	minishell

SRC		=	main.c \
			parsing.c \
			split.c \
			utils.c \
			add_slash.c \
			built_in.c \
			signals.c \
			chaining.c \
			env_var.c \
			export.c
CC		=	cc

CFLAGS	=	-Wall -Werror -Wextra

LDFLAGS	=	-lreadline

OBJ		=	$(SRC:.c=.o)

all:		$(NAME)

$(NAME):	$(OBJ)
			$(CC) $(OBJ) -o $(NAME) $(LDFLAGS)
			@echo "*----------------------*"
			@echo "\033[1;32mCompilation completed 🎉\033[0m"
			@echo "*----------------------*"


%.o: %.c
			$(CC) $(CFLAGS) -c $< -o $@

clean:
			rm -f $(OBJ)

fclean:		clean
			rm -f $(NAME)

re:			fclean all

.PHONY:		all clean fclean re