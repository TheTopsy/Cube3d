CC = cc

CFLAGS = -Wall -Wextra -g

MLX_FLAGS = -lmlx -L/mnt/c/Users/azert/Desktop/CUBE/CUBE3D/mlx/ -lm -lXext -lX11

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

.PHONY: all clean fclean re

