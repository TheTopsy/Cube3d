#ifndef CUB3D_H
#define CUB3D_H

#define FORMAT ".cub"
#define READSIZE 10

#include "./mlx/mlx.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>

# define HEIGHT 900
# define WIDTH 1200
# define COLS 60
# define ROWS 60
# define RAY_NUM 50
# define M_PI 3.14159265358979323846
# define FOV (60.0 * M_PI / 180.0)
# define TILE_SIZE 40

typedef struct s_textures
{
	char *no;
	char *so;
	char *we;
	char *ea;
	unsigned int c;
	unsigned int f;
} t_textures;

typedef struct s_map
{
	char **map;
	int map_start;
	int rows;
	int cols;
} t_map;

typedef struct s_datap
{
	t_textures *txrs;
	t_map *map;
} t_datap;

typedef struct	s_data {
	void	*mlx;
	void 	*win;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;


    char    **map;
	int		**grid;
}				t_data;

typedef struct s_ray
{
	float vhitx;
	float vhity;
	float hhitx;
	float hhity;
	float firstX;
	float firstY;
	float xstep;
	float ystep;
	float length;
	float angle;
}	t_ray;

typedef struct s_player
{
    float px;
    float py;
    float angle;
    int last_mouse_x;
    t_data *img;
    char dir;
    char dor;
	t_ray *ray;
}				t_player;



/*                                        MLX HELPER                                                          */
void   mlx_start(t_data *img);
void   my_mlx_pixel_put(t_data *data, float x, float y, int color);

int loop_hook(t_player *player);
int key_release(int code, t_player *player);
/*                                        DRAWING FUNCTIONS                                                    */
void draw_map(t_data *img, char **map);
void draw_player(t_player *player, float x0, float y0, int radius, int color);
void draw_square(t_data *data, int x, int y, int size);
void draw_rays(t_player *player, float px, float py);

/*                                        INPUT HANDLING                                                       */
int   keypress(int keycode, t_player *player);
int mouse_move(int x, int y, t_player *player);

/*                                        GRID CREATION                                                        */
int  **create_grid(int rows, int cols);
void fill_cell(t_data *img, int **grid, int rows, int cols, int r, int c, int color);
void draw_vertical_line(t_data *img, int x, int color);
void draw_horizontal_line(t_data *img, int y, int color);
void draw_grid(t_data *img, int rows, int cols, int line_color);

/*                                        MAP PARSING                                                         */
char **treat_map(char *map);
int check_format(char *map);

#endif
