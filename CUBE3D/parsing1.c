#include "cub3d.h"

int check_format(char *map)
{
	int i;
	int k;

	i = 0;
	k = 0;
	while(map[i])
		i++;
	//error for having no format
	if(i <= 4)
		return (0);
	i -= 4;
	while(FORMAT[k] && map[i])
	{
		//error for not having the wrong format
		if(FORMAT[k] != map[i])
			return (0);
		k++;
		i++;
	}
	return (1);
}

char *read_map(int fd)
{
	char *new;
	char tmp[READSIZE + 1];
	char *final;
	ssize_t b;
	ssize_t bread;

	final = NULL;
	bread = 0;
	b = 0;
	while((b = read(fd, tmp, READSIZE)) > 0)
	{
		tmp[b] = '\0';
		//gay ass malloc failure handling
		new = malloc(b + bread + 1);
		if(bread)
			memcpy(new, final, bread);
		memcpy(new + bread, tmp, b);
		new[b + bread] = '\0';
		bread += b;
		free(final);
		final = new;
	}
	return final;
}

int get_path(char *info, char **path, int *elements)
{
	int i;
	int k;
	
	i = 0;
	k = 0;
	while(info[i] != ' ')
		i++;
	i++;
	while(info[i + k] != '\n')
		k++;
	(*path) = malloc(k);
	k = 0;
	while(info[i] != '\n')
		(*path)[k++] = info[i++];
	(*path)[k] = '\0';
	(*elements)++;
	return i;
}

int extract_num(char *info, unsigned char *val)
{
	char *num;
	int i;
	int k;

	i = 0;
	k = 0;
	while(info[i + k] != '\n' && info[i + k] != ',')
		k++;
	num = malloc(k + 1);
	k = 0;
	while(info[i] != '\n' && info[i] != ',')
		num[k++] = info[i++];
	num[k] = '\0';
	(*val) = (unsigned char)atoi(num);
	free(num);
	if(info[i] == ',')
		i++;
	return i;
}

int get_rgb(char *info, unsigned int *rgb, int *elements)
{
	int i;
	unsigned char r;
	unsigned char g;
	unsigned char b;

	i = 0;
	while(info[i] != ' ')
		i++;
	i++;
	i += extract_num(info + i, &r);
	i += extract_num(info + i, &g);
	i += extract_num(info + i, &b);
	(*rgb) = (r << 16) | (g << 8) | b;
	(*elements)++;
	return i;
}

int fill_textures(t_textures *txtrs, char *info)
{
	int i;
	int elements;
	
	i = 0;
	elements = 0;
	while(info[i])
	{
		if(info[i] == 'N')
			i += get_path(info + i, &txtrs->no, &elements);
		else if(info[i] == 'S')
			i += get_path(info + i, &txtrs->so, &elements);
		else if(info[i] == 'W')
			i += get_path(info + i, &txtrs->we, &elements);
		else if(info[i] == 'E')
			i += get_path(info + i, &txtrs->ea, &elements);
		else if(info[i] == 'C')
			i += get_rgb(info + i, &txtrs->c, &elements);
		else if(info[i] == 'F')
			i += get_rgb(info + i, &txtrs->f, &elements);
		if(elements == 6)
			return (i);
		i++;
	}
	return (0);
}

int get_cols(char *info)
{
	int i;

	i = 0;
	while(info[i] != '\n')
		i++;
	return i;
}

int get_rows(char *info)
{
	int i;
	int rows;

	rows = 0;
	i = 0;
	while(info[i])
	{
		if(info[i] == '\n')
			rows++;
		i++;
	}
	return (rows);
}

void fill_map(t_map *map, char *info)
{
	int i;
	int x;
	int y;
	
	x = 0;
	y = 0;
	i = map->map_start;
	while(info[i] && info[i] == '\n')
		i++;
	map->rows = get_rows(info + i);
	map->cols = get_cols(info + i);
	map->map = malloc(map->rows * sizeof(char *));
	while(y < map->rows)
		map->map[y++] = malloc(map->cols + 1);
	y = 0;
	while(info[i])
	{
		if(info[i] != '\n')
			map->map[y][x++] = info[i++];
		else
		{
			map->map[y][x] = '\0';
			x = 0;
			i++;
			y++;
		}
	}
	map->map[y] = NULL;
}

char **split_info(t_player *player, char *info)
{
	t_map *map;
	t_textures *txtrs;
	
	map = malloc(sizeof(t_map));
	txtrs = malloc(sizeof(t_textures));
	map->map_start = fill_textures(txtrs, info);
	fill_map(map, info);
	int x = 0;
	int y = 0;
	while(y < map->rows)
	{
		while(x < map->cols)
			printf("%c", map->map[y][x++]);
		printf("\n");
		y++;
		x = 0;
	}
	printf("%s\n", txtrs->so);
	printf("%s\n", txtrs->ea);
	player->map = map;
	return (map->map);
}

char **treat_map(t_player *player, char *map)
{
	int fd;

	if(!check_format(map))
		return (NULL);
	fd = open(map, O_RDONLY);
	return split_info(player, read_map(fd));
}
int flfl(char **map, int y, int x)
{
    if (y < 0 || x < 0)
        return 0;
    if (map[y] == NULL || map[y][x] == '\0')
	    return 0;

    if (map[y][x] == '1' || map[y][x] == 'V')
        return 0;
    else if (map[y][x] == ' ' || map[y][x] == '0')
   	 map[y][x] = 'V';
    else
	    return 1;

    if (flfl(map, y + 1, x)) return 1;
    if (flfl(map, y - 1, x)) return 1;
    if (flfl(map, y, x + 1)) return 1;
    if (flfl(map, y, x - 1)) return 1;

    return 0;
}
int check_boundaries(char **map)
{
	int i = 0;
	while ( map[i])
	{
        int len = (int)strlen(map[i]);
        if (len == 0) continue;

        int fj = 0;
        while (fj < len && map[i][fj] == ' ')
            fj++;

        if (fj >= len)
            continue;

        if (map[i][fj] != '1')
        {
            printf("Invalid map\n");
            // return -1;
			exit(0);
        }

        int j = fj;
        while (j < len && map[i][j] != ' ')
            j++;

        if (j - 1 < 0 || map[i][j - 1] != '1')
        {
            printf("Invalid map\n");
            // return -1;
			exit(0);
        }
		i++;
    }
    return 0;
}

int check_map(char **map)
{
    int i = 0, j;
	check_boundaries(map);
	while ( map[i])
    {
        j = 0;
        while ( map[i][j])
        {
            if ( map[i][j] == ' ')
            {

                if (flfl(map, i, j))
                {
                    printf("Invalid map\n");
                    return -1;
                }
            }
            j++;
        }
        i++;
    }
    return 0;
}

float get_angle(char d)
{
	if (d == 'S')
		return 270 * M_PI / 180;
	else if (d == 'N')
		return 90 * M_PI / 180;
	else if (d == 'W')
		return 180 * M_PI / 180;
	else if (d == 'E')
		return 0 * M_PI / 180;
	return 0;
}

void print_error(int flag)
{
	if (flag == 1)
		printf("Error: Too many players on map\n");
	else if (flag == 2)
		printf("Error: No players found on map\n");
	exit(1);
}


void get_player_info(t_player *player, char **map)
{
	int i = 0, j, pos = 0;
    while (map[i])
    {
        j = 0;
        while (map[i][j])
        {
			if (pos == 2)
				print_error(1);
            if ( map[i][j] == 'S' || map[i][j] == 'N' || map[i][j] == 'W' || map[i][j] == 'E')
            {
				player->angle = get_angle(map[i][j]);
				player->px = (float)j * TILESIZE;
				player->py = (float)i * TILESIZE;
				pos++;
            }
				j++;
		}
        i++;
    }
	if (pos == 0)
		print_error(2);
}


void init_player(t_player *player, t_data *img, t_ray **ray)
{
	get_player_info(player, img->map);
	player->img = img;
	player->last_mouse_x = WIDTH / 2;
	player->dir = 0;
	player->dor = 0;
	player->ray = *ray;
}
