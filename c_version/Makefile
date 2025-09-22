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

# scatter plot
SCATTER_PLOT = scatter_plot
SCATTER_PLOT_SRC = $(SRC) scatter_plot.c
SCATTER_PLOT_SRCS = $(addprefix $(SRCDIR)/, $(SCATTER_PLOT_SRC))
SCATTER_PLOT_OBJS = $(SCATTER_PLOT_SRCS:.c=.o)

# compiling options
FLAGS = -Werror -Wextra -Wall
SDL_DIR = ${HOME}/SDL
INCLUDE = -I$(SDL_DIR)/include -L$(SDL_DIR)/lib -lSDL3
CC = gcc

all: install_sdl $(DESCRIBE) $(HISTOGRAM) $(SCATTER_PLOT)

install_sdl:
	@if [ ! -d "${HOME}/SDL" ]; then \
		chmod +x install.sh; bash install.sh; \
	fi

$(DESCRIBE): $(DESCRIBE_OBJS)
	$(CC) $(FLAGS) $(DESCRIBE_OBJS) -o $(DESCRIBE) -lm

$(HISTOGRAM): $(HISTOGRAM_OBJS)
	$(CC) $(FLAGS) $(HISTOGRAM_OBJS) -o $(HISTOGRAM) -lncurses -lm $(INCLUDE)

$(SCATTER_PLOT): $(SCATTER_PLOT_OBJS)
	$(CC) $(FLAGS) $(SCATTER_PLOT_OBJS) -o $(SCATTER_PLOT) -lncurses -lm $(INCLUDE)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf src/*.o

fclean: clean
	rm -rf $(DESCRIBE) $(HISTOGRAM) $(SCATTER_PLOT)

re: clean all
