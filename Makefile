NAME = minishell

SRC = src/minishell.c utils/utils1.c\

OBJ = $(SRC:.c=.o)

CC = cc

CFLAGS = -Wall -Werror -Wextra
RLFLAG = -lreadline

COMPILE = $(CC) $(CFLAGS) $(RLFLAG)

all : $(NAME)

$(NAME) : $(OBJ)
	$(COMPILE) $(OBJ) -I/opt/vagrant/embedded/include -L/opt/vagrant/embedded/lib -lreadline -lhistory -o $(NAME)

clean : 
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re