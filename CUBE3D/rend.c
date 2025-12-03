#include "cub3d.h"

void draw_player(t_player *player, float x0, float y0, int radius, int color) // temporary 
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

    draw_rays(player, x0, y0);
}

void draw_map(t_data *img, char **map)
{
	int i = 0, j = 0 , x = 0, y = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == '1')
                		fill_cell(img,img->grid,COLS,ROWS,i,j,0x00FF0000);
			j++;
		}
		i++;
	}
}

void hdda(t_player *player)
{
    for (int i = 0; i < RAY_NUM; ++i)
    {
        player->ray[i].angle = player->angle - (FOV / 2.0) + i * (FOV / RAY_NUM);
        float dx = cos(player->ray[i].angle);
        float dy = sin(player->ray[i].angle);
        
        
        if (dy > 0.0)
        {
            player->ray[i].firstY = (floor(player->py / TILE_SIZE) + 1.0) * TILE_SIZE;
            player->ray[i].ystep  = (float)TILE_SIZE;
        }
        else
        {
            player->ray[i].firstY = floor(player->py / TILE_SIZE) * TILE_SIZE;
            player->ray[i].ystep  = -(float)TILE_SIZE;
        }
        player->ray[i].xstep = player->px + (player->ray[i].firstY - player->py) / tan(player->ray[i].angle);
        
        for (int step = 0; step < 200; ++step)
        {
            if (player->ray[i].xstep < 0.0 || player->ray[i].xstep >= (float)WIDTH || player->ray[i].firstY < 0.0 || player->ray[i].firstY >= (float)HEIGHT)
                break;
            my_mlx_pixel_put(player->img, (int)player->ray[i].xstep, (int)player->ray[i].firstY, 0x00FFa500);
            player->ray[i].firstY += player->ray[i].ystep;
            player->ray[i].xstep += player->ray[i].ystep / tan(player->ray[i].angle);
        }
    }
}
void vdda(t_player *player)
{
    for (int i = 0; i < RAY_NUM; ++i)
    {
        player->ray[i].angle = player->angle - (FOV / 2.0) + i * (FOV / RAY_NUM);
        float dx = cos(player->ray[i].angle);
        float dy = sin(player->ray[i].angle);
        
        if (dx > 0.0)
        {
            player->ray[i].firstX = (floor(player->px / TILE_SIZE) + 1.0) * TILE_SIZE;
            player->ray[i].xstep  = (float)TILE_SIZE;
        }
        else
        {
            player->ray[i].firstX = floor(player->px / TILE_SIZE) * TILE_SIZE;
            player->ray[i].xstep  = -(float)TILE_SIZE;
        }
        player->ray[i].ystep = player->py + (player->ray[i].firstX - player->px) * tan(player->ray[i].angle);
        
        for (int step = 0; step < 200; ++step)
        {
            if (player->ray[i].firstX < 0.0 || player->ray[i].firstX >= (float)WIDTH || player->ray[i].ystep < 0.0 || player->ray[i].ystep >= (float)HEIGHT)
                break;

            my_mlx_pixel_put(player->img, (int)player->ray[i].firstX, (int)player->ray[i].ystep, 0x00FFFF00);
            player->ray[i].firstX += player->ray[i].xstep;
            player->ray[i].ystep += player->ray[i].xstep * tan(player->ray[i].angle);
        }
    }
}
float normalize_angle(float angle)
{
    if (angle < 0)
        angle += 2 * M_PI;
    if (angle > 2 * M_PI)
        angle -= 2 * M_PI;
    return angle;
}

void draw_rays(t_player *player, float px, float py)
{
    player->angle = normalize_angle(player->angle);
    hdda(player);   
    vdda(player);
    for (int i = 0; i < RAY_NUM; i++)
    {
        player->ray[i].angle = player->angle - (FOV / 2) + (i * (FOV / RAY_NUM));
        player->ray[i].angle = normalize_angle(player->ray[i].angle);
        float dx = cos(player->ray[i].angle) * 2;
        float dy = sin(player->ray[i].angle) * 2;
        
        float x = player->px;
        float y = player->py;
        
        for (int step = 0; step < 300; step++)
        {
            if ((x < 0 || x >= WIDTH) || (y < 0 || y >= HEIGHT))
                break;
            if (player->img->map[(int)(y/TILE_SIZE)][(int)(x/TILE_SIZE)] == '1')
                break;
                
            my_mlx_pixel_put(player->img, (int)x, (int)y, 0x0000FF00);
            x += dx;
            y += dy;
        }
    }
}

void draw_square(t_data *img, int x, int y, int len)
{
	int sx = x - (len / 2), sy = y - (len / 2), i = 0;
	for (int i = 0; i < len; i++)
	{
		for (int j = 0; j < len; j++)
		{
			my_mlx_pixel_put(img, x + j, y + i, 0x00FFFFFF);
		}
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
    draw_map(player->img, player->img->map);
    draw_player(player, player->px, player->py, 10, 0x00FF0000);
    mlx_put_image_to_window(player->img->mlx, player->img->win, player->img->img, 0, 0);

    return 0;
}

int **create_grid(int rows, int cols)
{
    int **grid = malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
    {
        grid[i] = malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++)
            grid[i][j] = 0;
    }
    return grid;
}

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

void fill_cell(t_data *img, int **grid, int rows, int cols, int r, int c, int color)
{
    if (r < 0 || r >= rows || c < 0 || c >= cols) return;
    int cell_w = WIDTH / cols;
    int cell_h = HEIGHT / rows;

    int x0 = c * cell_w + 1;
    int y0 = r * cell_h + 1;
    int x1 = (c + 1) * cell_w - 1;
    int y1 = (r + 1) * cell_h - 1;

    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 >= WIDTH) x1 = WIDTH - 1;
    if (y1 >= HEIGHT) y1 = HEIGHT - 1;

    for (int y = y0; y <= y1; ++y)
        for (int x = x0; x <= x1; ++x)
            my_mlx_pixel_put(img, x, y, color);

    grid[r][c] = 1;
}

void draw_grid(t_data *img, int cols, int rows, int color)
{
    int cell_w = WIDTH / cols;
    int cell_h = HEIGHT / rows;
    for (int c = 0; c <= cols; ++c) {
        int x = c * cell_w;
        if (x >= WIDTH) x = WIDTH - 1;
        draw_vertical_line(img, x, color);
    }
    for (int r = 0; r <= rows; ++r) {
        int y = r * cell_h;
        if (y >= HEIGHT) y = HEIGHT - 1;
        draw_horizontal_line(img, y, color);
    }
}

int loop_hook(t_player *player)
{
	if(player->dir == 1)
	{
		player->px += cos(player->angle) * 0.5;
        	player->py += sin(player->angle) * 0.5;
		if (player->px < 10) player->px = 10;
        	if (player->px > WIDTH - 10) player->px = WIDTH - 10;
        	if (player->py < 10) player->py = 10;
        	if (player->py > HEIGHT - 10) player->py = HEIGHT - 10;
	}
	if(player->dir == -1)
	{
        	player->px -= cos(player->angle) * 0.5;
        	player->py -= sin(player->angle) * 0.5;
		if (player->px < 10) player->px = 10;
        	if (player->px > WIDTH - 10) player->px = WIDTH - 10;
        	if (player->py < 10) player->py = 10;
        	if (player->py > HEIGHT - 10) player->py = HEIGHT - 10;
	
	}
	if(player->dor == 1)
		player->angle += 0.01;
	if(player->dor == -1)
		player->angle -= 0.01;
	memset(player->img->addr, 0, HEIGHT * player->img->line_length); //i guess i can to this with mlx_destroy_image
    	// draw_map(player->img, player->img->map);
    	draw_player(player, player->px, player->py, 10, 0x00FF0000);
        // draw_grid(player->img, COLS, ROWS, 0x00FFFFFF);
    	mlx_put_image_to_window(player->img->mlx, player->img->win, player->img->img, 0, 0);
	//printf("dir = %d\n", player->dir);
	return (0);
}
