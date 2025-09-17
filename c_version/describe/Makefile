NAME = dslr
SRCS = *.c
FLAGS = -Werror -Wextra -Wall

$(NAME):
		gcc $(FLAGS) $(SRCS) -o $(NAME) -lm

clean:
		rm -rf $(NAME)

all: 	$(NAME)

re:		clean all