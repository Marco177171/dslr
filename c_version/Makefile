# dir containing source files
SRCDIR = src

# common .c files
SRC = check.c free.c printers.c utils.c statisitc_operations.c data_frame.c

# describe
DESCRIBE = describe
DESCRIBE_SRC = $(SRC) describe.c
DESCRIBE_SRCS = $(addprefix $(SRCDIR)/, $(DESCRIBE_SRC))
DESCRIBE_OBJS = $(DESCRIBE_SRCS:.c=.o)

# histogram
HISTOGRAM = histogram
HISTOGRAM_SRC = $(SRC) histogram.c
HISTOGRAM_SRCS = $(addprefix $(SRCDIR)/, $(HISTOGRAM_SRC))
HISTOGRAM_OBJS = $(HISTOGRAM_SRCS:.c=.o)

# compiling options
FLAGS = -Werror -Wextra -Wall
CC = gcc

all: $(DESCRIBE) $(HISTOGRAM)

$(DESCRIBE): $(DESCRIBE_OBJS)
	$(CC) $(FLAGS) $(DESCRIBE_OBJS) -o $(DESCRIBE) -lm

$(HISTOGRAM): $(HISTOGRAM_OBJS)
	$(CC) $(FLAGS) $(HISTOGRAM_OBJS) -o $(HISTOGRAM) -lncurses -lm
	
%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf src/*.o

fclean: clean
	rm -rf $(DESCRIBE) $(HISTOGRAM)

re: clean all