NAME = histogram
SRCS = *.c
FLAGS = -Werror -Wextra -Wall

$(NAME): 
	gcc main.c -std=c99 -lncurses -o ${NAME}

clean:
		rm -rf $(NAME)

all: 	$(NAME)

re:		clean all