NAME = minishell

SRC = src/minishell.c src/initializing/init.c src/initializing/envinit.c src/initializing/envset.c src/parsing/parsingenv.c src/parsing/parsing.c src/parsing/parsingchar.c \
		utils/utils1.c utils/lst_utils.c utils/ft_strtrim.c src/type.c utils/cmd_utils.c utils/ft_split.c \
		get_next_line/get_next_line.c get_next_line/get_next_line_utils.c src/exec/exec_utils.c src/exec/cmd_arg_utils.c \
		src/exec/executor.c src/bcomm/bcomm.c

OBJ = $(SRC:.c=.o)

CC = cc

CFLAGS = -Wall -Werror -Wextra -I/opt/vagrant/embedded/include

COMPILE = $(CC) $(CFLAGS) $(RLFLAG)

all : $(NAME)

leak: $(NAME)
	valgrind --leak-check=full --suppressions=leaks.supp ./minishell
# --show-leak-kinds=all --trace-children=yes --track-fds=yes 

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -L/opt/vagrant/embedded/lib -lreadline -lncurses -lhistory -o $(NAME)


clean : 
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re