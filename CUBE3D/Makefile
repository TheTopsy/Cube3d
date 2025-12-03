CC = gcc

CFLAGS = -Wall -Wextra

MLX_FLAGS = -lmlx -L./mlx/ -lm -lXext -lX11

SRC = mlx.c rend.c parsing1.c main.c

OBJS = $(SRC:.c=.o)

NAME = cub3d

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
	make clean
	./cub3d

.PHONY: all clean fclean re

