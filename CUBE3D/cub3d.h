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

# define TILESIZE 30
# define HEIGHT 900
# define WIDTH 1600
# define COLS 25
# define ROWS 15
# define FOV 1.0472
# define STRIPESIZE 4
# define PI 3.14159265358979323846

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
	int	bits_per_pixel;
	int	line_length;
	int	endian;
	char	**map;
	int	**grid;
}	t_data;

typedef struct s_ray
{
	float	vhitx;
	float	vhity;
	float	hhitx;
	float	hhity;
	float	hfirstx;
	float	hfirsty;
	float	vfirstx;
	float	vfirsty;
	float	hxstep;
	float	hystep;
	float	vxstep;
	float	vystep;
	float	length;
	float	angle;
	float	nearest;
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
    int ray_num;
    t_map *map;
    t_ray *ray;
}	t_player;

/*typedef struct s_ray
{
	float hitx;
	float hity;
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
}				t_player;
*/


/*                                        MLX HELPER                                                          */
void   mlx_start(t_data *img);
void   my_mlx_pixel_put(t_data *data, float x, float y, int color);

void normalizeangle(float *angle);
void hget_step(t_player *player, t_ray *ray, int i);
void hget_hit(t_player *player, t_ray *ray, int i);
void hdda(t_player *player, t_ray *ray);
void get_nearest_inter(t_player *player, float px, float py);

int loop_hook(t_player *player);
int key_release(int code, t_player *player);
/*                                        DRAWING FUNCTIONS                                                    */
void draw_player(t_player *player, float x0, float y0, int radius, int color, t_ray *ray);
void draw_square(t_data *data, int x, int y, int color);
//void draw_rays(t_player *player, float px, float py);

/*                                        INPUT HANDLING                                                       */
int   keypress(int keycode, t_player *player);
int mouse_move(int x, int y, t_player *player);

/*                                        GRID CREATION                                                        */
int  **create_grid(int rows, int cols);
void draw_vertical_line(t_data *img, int x, int color);
void draw_horizontal_line(t_data *img, int y, int color);
void draw_grid(t_player *player, t_data *img);

/*                                        MAP PARSING                                                         */
char **treat_map(t_player *player, char *map);
int check_format(char *map);


void init_player(t_player *player, t_data *img, t_ray **ray);
void get_player_info(t_player *player, char **map);
float get_angle(char d);
void print_error(int flag);
int check_map(char **map);
int check_boundries(char **map);
int flfl(char **map, int y, int x);


#endif
