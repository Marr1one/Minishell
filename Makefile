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
			export.c \
			echo.c \
			ft_cd.c \
			ft_quotes.c \
			exit.c

CC		=	cc -g3

OBJ_DIR	=	obj

CFLAGS	=	-Wall -Werror -Wextra

LDFLAGS	=	-lreadline

OBJ		=	$(SRC:%.c=$(OBJ_DIR)/%.o)

all:		$(NAME)

$(NAME):	$(OBJ)
			$(CC) $(OBJ) -o $(NAME) $(LDFLAGS)
			@echo "*----------------------*"
			@echo "\033[1;32mCompilation completed 🎉\033[0m"
			@echo "*----------------------*"


$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
			$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
			mkdir -p $(OBJ_DIR)

clean:
			rm -f $(OBJ)

fclean:		clean
			rm -f $(NAME)

re:			fclean all

.PHONY:		all clean fclean re