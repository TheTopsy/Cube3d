#include "cub3d.h"


int	main(int ac, char **av)
{
	t_data	img;
	t_player player;
	t_ray *ray;

	if(ac < 2 || ac > 3)
		return (1);
	img.map = treat_map(&player, av[1]);
	if(check_map(img.map) == - 1)
		return (1);

        player.ray_num = (WIDTH / STRIPESIZE);
	ray = malloc(player.ray_num * sizeof(t_ray));
	init_player(&player, &img, &ray);
	
	printf("%c\n", player.map->map[0][6]);

        //normalizeangle(&player.angle);
        //player.ray_num = (WIDTH / STRIPESIZE);

	mlx_start(&img);
	draw_grid(&player, &img);
	draw_player(&player, player.px, player.py, 10, 0x00FF0000, ray);

	mlx_hook(img.win, 2, 1L<<0, keypress, &player);     // Key press
	mlx_hook(img.win, 3, 1L<<1, key_release, &player);   // Key release
	//mlx_put_image_to_window(img.mlx, img.win, player.img->img, 0, 0);
	mlx_loop_hook(img.mlx, loop_hook, &player);
	mlx_loop(img.mlx);
}
