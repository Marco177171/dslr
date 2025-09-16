NAME = histogram
SRCS = *.c
FLAGS = -Werror -Wextra -Wall

$(NAME):
		gcc $(FLAGS) $(SRCS) -o $(NAME) -lncurses -lm

clean:
		rm -rf $(NAME)

all: 	$(NAME)

re:		clean all