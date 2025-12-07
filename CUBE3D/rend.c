#include "cub3d.h"

void draw_player(t_player *player, float x0, float y0, int radius, int color, t_ray *ray) // temporary 
{
    int x = radius;
    int y = 0;
    int err = 0;

    my_mlx_pixel_put(player->img, x0, y0, color);
    while (x >= y)
    {
        my_mlx_pixel_put(player->img, x0 + x, y0 + y, color);
        my_mlx_pixel_put(player->img, x0 + y, y0 + x, color);
        my_mlx_pixel_put(player->img, x0 - y, y0 + x, color);
        my_mlx_pixel_put(player->img, x0 - x, y0 + y, color);
        my_mlx_pixel_put(player->img, x0 - x, y0 - y, color);
        my_mlx_pixel_put(player->img, x0 - y, y0 - x, color);
        my_mlx_pixel_put(player->img, x0 + y, y0 - x, color);
        my_mlx_pixel_put(player->img, x0 + x, y0 - y, color);

        y += 1;
        if (err <= 0)
        {
            err += 2*y + 1;
        }
        if (err > 0)
        {
            x -= 1;
            err -= 2*x + 1;
        }
    }

    hdda(player, ray);
}

void normalizeangle(float *angle)
{
    *angle = fmod(*angle, 2.0 * PI);
    if (*angle < 0)
        *angle += 2.0 * PI;
}

/*void hget_step(t_player *player, t_ray *ray, int i)
{
    float angle = ray[i].angle;

    // prevent division by zero / huge hxstep
    float t = tan(angle);
    if (fabs(t) < 0.0001)
    {
        ray[i].hfirstx = HUGE_VAL;
        ray[i].hfirsty = HUGE_VAL;
        ray[i].hxstep  = 0;
        ray[i].hystep  = 0;
        return;
    }

    // first horizontal intersection
    if (angle > PI)
        ray[i].hfirsty = floor(player->py / TILESIZE) * TILESIZE - 0.0001;
    else
        ray[i].hfirsty = floor(player->py / TILESIZE) * TILESIZE + TILESIZE;

    ray[i].hfirstx = player->px + ((ray[i].hfirsty - player->py) / t);

    // vertical step amounts
    ray[i].hystep = (angle > PI) ? -TILESIZE : TILESIZE;
    ray[i].hxstep = ray[i].hystep / t;
}*/



void vget_step(t_player *player, t_ray *ray, int i)
{
	if(ray[i].angle > PI/2 && ray[i].angle < 3*PI/2)
		ray[i].vfirstx = (floor(player->px / TILESIZE) * TILESIZE) - 0.0001;
	else
		ray[i].vfirstx = (floor(player->px / TILESIZE) * TILESIZE) + TILESIZE;
	ray[i].vfirsty = player->py + ((ray[i].vfirstx - player->px) * tan(ray[i].angle));
	
	if(ray[i].angle > PI/2 && ray[i].angle < 3*PI/2)
		ray[i].vxstep = -TILESIZE;
	else
		ray[i].vxstep = TILESIZE;
	ray[i].vystep = ray[i].vxstep * tan(ray[i].angle);
}


void hget_step(t_player *player, t_ray *ray, int i)
{
	if(ray[i].angle > PI)
		ray[i].hfirsty = (floor(player->py / TILESIZE) * TILESIZE) - 0.0001;
	else
		ray[i].hfirsty = (floor(player->py / TILESIZE) * TILESIZE) + TILESIZE;
	ray[i].hfirstx = player->px + ((ray[i].hfirsty - player->py) / tan(ray[i].angle));
	
	if(ray[i].angle > PI)
		ray[i].hystep = -TILESIZE;
	else
		ray[i].hystep = TILESIZE;
	ray[i].hxstep = ray[i].hystep / tan(ray[i].angle);
}

/*void    hget_hit(t_player *player, t_ray *ray, int i)
{
    float x = ray[i].hfirstx;
    float y = ray[i].hfirsty;

    float step_x = ray[i].hxstep;
    float step_y = ray[i].hystep;

    while (1)
    {
        int map_x = (int)(x / TILESIZE);
        int map_y = (int)(y / TILESIZE);

        // adjust for UP-facing rays (angle > PI)
        if (ray[i].angle > PI)
            map_y -= 1;

        // ===== OUT OF BOUNDS CHECK =====
        if (map_x < 0 || map_x + 1 >= COLS||
            map_y < 0 || map_y + 1 >= ROWS)
            break;

        // ===== WALL HIT CHECK =====
        if (player->img->map[map_y][map_x] == '1')
	{
		break;
	}
        // draw the step (debug)
        my_mlx_pixel_put(player->img, (int)x, (int)y, 0xFF0000);

        // next step
        x += step_x;
        y += step_y;
    }

    ray[i].hhitx = x;
    ray[i].hhity = y;
}*/


void hget_hit(t_player *player, t_ray *ray, int i)
{
	ray[i].hhity = ray[i].hfirsty;
	ray[i].hhitx = ray[i].hfirstx;
	while(1)
	{
		if ((ray[i].hhity < 0) || (ray[i].hhitx < 0))
        		break;
		if(ray[i].angle > PI)
		{
			if ((floor(ray[i].hhity / TILESIZE) + 1 >= ROWS) || (floor(ray[i].hhitx / TILESIZE) + 1 >= COLS))
        			break;	
			if(player->img->map[(int)(floor(ray[i].hhity) / TILESIZE)][(int)floor(ray[i].hhitx) / TILESIZE] == '1')
				break;
		}
		else
		{
			if ((floor(ray[i].hhity / TILESIZE) + 1 >= ROWS) || (floor(ray[i].hhitx / TILESIZE) + 1 >= COLS))
        			break;	
			if(player->img->map[(int)(floor(ray[i].hhity) / TILESIZE)][(int)floor(ray[i].hhitx) / TILESIZE] == '1')
				break;
		}
		my_mlx_pixel_put(player->img, ray[i].hhitx, ray[i].hhity, 0xFF0000);
                ray[i].hhity += ray[i].hystep;
                ray[i].hhitx += ray[i].hxstep;

	}
}

void vget_hit(t_player *player, t_ray *ray, int i)
{
        ray[i].vhity = ray[i].vfirsty;
        ray[i].vhitx = ray[i].vfirstx;
        while(1)
        {
                if ((ray[i].vhity < 0) || (ray[i].vhitx < 0))
                        break;
                if(ray[i].angle > PI/2 && ray[i].angle < 3*PI/2)
                {
                        if ((floor(ray[i].vhity / TILESIZE) + 1 >= ROWS) || (floor(ray[i].vhitx / TILESIZE) + 1 >= COLS))
                                break;
                        if(player->img->map[(int)(floor(ray[i].vhity) / TILESIZE)][(int)floor(ray[i].vhitx) / TILESIZE] == '1')
                                break;
                }
                else
                {
                        if ((floor(ray[i].vhity / TILESIZE) + 1 >= ROWS) || (floor(ray[i].vhitx / TILESIZE) + 1 >= COLS))
                                break;
                        if(player->img->map[(int)(floor(ray[i].vhity) / TILESIZE)][(int)floor(ray[i].vhitx) / TILESIZE] == '1')
                                break;
                }
                my_mlx_pixel_put(player->img, ray[i].vhitx, ray[i].vhity, 0xFF0000);
                ray[i].vhity += ray[i].vystep;
                ray[i].vhitx += ray[i].vxstep;
        }
}

void compare_inter(t_player *player, t_ray *ray, int i)
{
        float vdx = abs(ray[i].vhitx - player->px);
        float vdy = abs(ray[i].vhity - player->py);
        float hdx = abs(ray[i].hhitx - player->px);
        float hdy = abs(ray[i].hhity - player->py);
        float hd = sqrt(hdx * hdx + hdy * hdy);
        float vd = sqrt(vdx * vdx + vdy * vdy);
        if(hd < vd)
                ray[i].nearest = hd;
        else
                ray[i].nearest = vd;
}

void hdda(t_player *player, t_ray *ray)
{
	int i;
	float rot_angle;
	i = 0;
	rot_angle = 0;
	while(i < player->ray_num)
	{
		ray[i].angle = (player->angle - (FOV / 2)) + rot_angle;
		normalizeangle(&ray[i].angle);
		hget_step(player, ray, i);
		hget_hit(player, ray, i);
		my_mlx_pixel_put(player->img, ray[i].hhitx, ray[i].hhity, 0xFF0000);
		vget_step(player, ray, i);
		vget_hit(player, ray, i);
		compare_inter(player, ray, i);
		my_mlx_pixel_put(player->img, ray[i].vhitx, ray[i].vhity, 0xFF0000);
		rot_angle += (FOV / player->ray_num);
		i++;
	}
}

void get_nearest_inter(t_player *player, float px, float py)
{
	//fill hhit
	//fill vhit
	//get the nearest
	/*t_ray *ray;
	
	normalizeangle(&player->angle);
	player->ray_num = (WIDTH / STRIPESIZE);
	ray = malloc(player->ray_num * sizeof(t_ray));
	player->ray = ray;*/
	hdda(player, player->ray);
	/*vdda(player, player->ray);
	compare_inter(player);
	
	int b = 0;
	float ray_angle;
	int ray_num;
	
	normalizeangle(&player->angle);
	ray_angle = player->angle - (FOV / 2);
	ray_num = (WIDTH / STRIPESIZE);
	// 1. Ray facing direction
	int facing_down = (ray_angle > 0 && ray_angle < PI);
	int facing_up   = !facing_down;

	// 2. First horizontal intersection
	float first_y;
	if (facing_down)
		first_y = floor(py / (TILESIZE)) * TILESIZE + TILESIZE;
	else
		first_y = floor(py / TILESIZE) * TILESIZE - 1;

	float first_x = px + (first_y - py) / tan(ray_angle);

	// 3. Step sizes
	float step_y = facing_down ? TILESIZE : -TILESIZE;
	float step_x = step_y / tan(ray_angle);

	// 4. Draw first 3 intersections only
	
	while (b < 10)
	{
		if (first_x < 0 || first_x >= WIDTH || first_y < 0 || first_y >= HEIGHT)
			break;
		my_mlx_pixel_put(player->img, (int)first_x, (int)first_y, 0xFFee0000);
		my_mlx_pixel_put(player->img, (int)first_x + 1, (int)first_y, 0xFFee0000);
		my_mlx_pixel_put(player->img, (int)first_x - 1, (int)first_y, 0xFFee0000);
		my_mlx_pixel_put(player->img, (int)first_x, (int)first_y - 1, 0xFFee0000);
		my_mlx_pixel_put(player->img, (int)first_x, (int)first_y + 1, 0xFFee0000);
		first_x += step_x;
		first_y += step_y;
		b++;
	}*/
}

void draw_square(t_data *img, int x, int y, int color)
{
	int i;
	int j;

	i = 1;
	while(i < TILESIZE - 1)
	{
		j = 1;
		while(j < TILESIZE - 1)
		{
			my_mlx_pixel_put(img, x + i, y + j, color);
			j++;
		}
		i++;
	}
}

int keypress(int code, t_player *player)
{
	if (code == 119)
	{
		player->dir = 1;
      		//printf("front\n");  
	//player->px += cos(player->angle) * 2.5;
        //player->py += sin(player->angle) * 2.5;
        // Add bounds checking
        /*if (player->px < 10) player->px = 10;
        if (player->px > WIDTH - 10) player->px = WIDTH - 10;
        if (player->py < 10) player->py = 10;
        if (player->py > HEIGHT - 10) player->py = HEIGHT - 10;*/
    }
    else if (code == 115) 
    {
	    player->dir = -1;
        //player->px -= cos(player->angle) * 2.5;
        //player->py -= sin(player->angle) * 2.5;
        // Add bounds checking
        /*if (player->px < 10) player->px = 10;
        if (player->px > WIDTH - 10) player->px = WIDTH - 10;
        if (player->py < 10) player->py = 10;
        if (player->py > HEIGHT - 10) player->py = HEIGHT - 10;*/
    }
    else if (code == 97) // A key
        player->dor = -1; // Rotate left
    else if (code == 100) // D key
        player->dor = 1; // Rotate right
    else if (code == 65307) // ESC key
    {
        exit(0);
    }
    
    return 0;
}

int key_release(int code, t_player *player)
{
    if (code == 119)
	    player->dir = 0;
    if(code == 115)
	    player->dir = 0;
    else if (code == 97) // A key
        player->dor = 0; // Rotate left
    else if (code == 100) // D key
        player->dor = 0; // Rotate right
    return (0);
}

int mouse_move(int x, int y, t_player *player)
{
    int mouse_delta = x - player->last_mouse_x;
    
    player->angle += mouse_delta * 0.005;  

    player->last_mouse_x = x;

    memset(player->img->addr, 0, HEIGHT * player->img->line_length);
    draw_player(player, player->px, player->py, 10, 0x00FF0000, player->ray);
    mlx_put_image_to_window(player->img->mlx, player->img->win, player->img->img, 0, 0);

    return 0;
}

/*int **create_grid(int rows, int cols)
{
    int **grid = malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
    {
        grid[i] = malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++)
            grid[i][j] = 0;
    }
    return grid;
}*/

void draw_vertical_line(t_data *img, int x, int color)
{
    for (int y = 0; y < HEIGHT; ++y)
        my_mlx_pixel_put(img, x, y, color);
}

void draw_horizontal_line(t_data *img, int y, int color)
{
    for (int x = 0; x < WIDTH; ++x)
        my_mlx_pixel_put(img, x, y, color);
}

void draw_grid(t_player *player, t_data *img)
{
	int r;
	int c;
	int x;
	int y;

	r = 0;
	while(r < player->map->rows)
	{
		c = 0;
		while(c < player->map->cols)
		{
			x = c * TILESIZE;
			y = r * TILESIZE;
			if(player->map->map[r][c] == '1')
				draw_square(img, x, y, 0xFFFFFF);
			else if(player->map->map[r][c] == '0')
				draw_square(img, x, y, 0x000000);
			c++;
		}
		printf("r = %d, c = %d\n", r, c);
		printf("rows = %d, cols = %d\n", player->map->rows, player->map->cols);
		r++;
	}

	/*
	int r = 0;
	while (r < ROWS)
	{
		int c = 0;
		while (c < COLS)
		{
			int x0 = c * TILESIZE;
			int y0 = r * TILESIZE;

			// draw tile background
			for (int y = 0; y < TILESIZE; y++)
			{
				for (int x = 0; x < TILESIZE; x++)
				{
					my_mlx_pixel_put(img, x0 + x, y0 + y, 0x222222); // empty
				}
			}
			printf("allo\n");
			// draw tile borders (grid lines)
			for (int i = 0; i < TILESIZE; i++)
			{
				my_mlx_pixel_put(img, x0 + i, y0,       0x000000); // top
				my_mlx_pixel_put(img, x0 + i, y0+TILESIZE-1,0x000000); // bottom
				my_mlx_pixel_put(img, x0,       y0 + i, 0x000000); // left
				my_mlx_pixel_put(img, x0+TILESIZE-1,y0 + i, 0x000000); // right
			}
			c++;
		}
		r++;
	}*/
}

int loop_hook(t_player *player)
{
	if(player->dir == 1)
	{
		player->px += cos(player->angle) * 0.9;
        	player->py += sin(player->angle) * 0.9;
		if (player->px < 10) player->px = 10;
        	if (player->px > WIDTH - 10) player->px = WIDTH - 10;
        	if (player->py < 10) player->py = 10;
        	if (player->py > HEIGHT - 10) player->py = HEIGHT - 10;
	}
	if(player->dir == -1)
	{
        	player->px -= cos(player->angle) * 0.9;
        	player->py -= sin(player->angle) * 0.9;
		if (player->px < 10) player->px = 10;
        	if (player->px > WIDTH - 10) player->px = WIDTH - 10;
        	if (player->py < 10) player->py = 10;
        	if (player->py > HEIGHT - 10) player->py = HEIGHT - 10;
	
	}
	if(player->dor == 1)
		player->angle += 0.009;
	if(player->dor == -1)
		player->angle -= 0.009;
	memset(player->img->addr, 0, HEIGHT * player->img->line_length); //i guess i can to this with mlx_destroy_image
        draw_grid(player, player->img);
    	draw_player(player, player->px, player->py, 10, 0x00FF0000, player->ray);
    	mlx_put_image_to_window(player->img->mlx, player->img->win, player->img->img, 0, 0);
	//printf("dir = %d\n", player->dir);
	return (0);
}
