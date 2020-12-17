/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 09:07:45 by aiglesia          #+#    #+#             */
/*   Updated: 2020/12/17 12:16:12 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include "libft.h"
#include <mlx.h>
#include  <stdio.h>

#define WHITE 0xffefdb
#define ESC 65307
#define ENTER 65293

typedef struct      s_data 
{
    void            *img;
    char            *addr;
    int             bits_per_pixel;
    int             line_length;
    int             endian;
    int             height;
    int             width;
}                   t_data;

typedef struct      s_anim 
{
	t_data *frame_1;
	t_data *frame_2;
}                   t_anim;

typedef struct      s_cumple 
{
	int				stage;
	void			*mlx_ptr;
	void			*win_ptr;
	char			*fecha_written;
	t_anim			pacman;
	t_data			*cake;
}                   gm_inf;

t_data *resize_image(char *path, float size_ratio, void *mlx_ptr);

void render_animated_image(int x, int y, t_anim *image, gm_inf *game_data)
{
	float time = (float)clock()/CLOCKS_PER_SEC * 20;
	if ((int)time % 2)
		mlx_put_image_to_window(game_data->mlx_ptr, game_data->win_ptr, image->frame_1->img, x, y);
	else
		mlx_put_image_to_window(game_data->mlx_ptr, game_data->win_ptr, image->frame_2->img, x, y);
}

void handle_stage_1(gm_inf *game_info)
{
	float init_time = (float)clock()/CLOCKS_PER_SEC*10;

	while ((float)clock()/CLOCKS_PER_SEC*10 - init_time < 2)
		mlx_string_put(game_info->mlx_ptr, game_info->win_ptr, 300, 400, WHITE, "Es el cumple de la garrafa! :)");
	init_time = (float)clock()/CLOCKS_PER_SEC*10;
	mlx_clear_window(game_info->mlx_ptr, game_info->win_ptr);
	while ((float)clock()/CLOCKS_PER_SEC*10 - init_time < 3)
		mlx_string_put(game_info->mlx_ptr, game_info->win_ptr, 300, 400, WHITE, "La garrafa puede comer pastel!");
	game_info->stage = 3;
}

void handle_stage_2(gm_inf *game_info)
{
	t_data *image;
	float init_time = (float)clock()/CLOCKS_PER_SEC*10;

	while ((float)clock()/CLOCKS_PER_SEC*10 - init_time < 2)
		mlx_string_put(game_info->mlx_ptr, game_info->win_ptr, 300, 400, WHITE, "No es el cumple de la garrafa :(");
	init_time = (float)clock()/CLOCKS_PER_SEC*10;
	mlx_clear_window(game_info->mlx_ptr, game_info->win_ptr);
	while ((float)clock()/CLOCKS_PER_SEC*10 - init_time < 3)
		mlx_string_put(game_info->mlx_ptr, game_info->win_ptr, 300, 400, WHITE, "La garrafa esta triste...");
	image = resize_image("textures/garrafa_triste.XPM", 0.5, game_info->mlx_ptr);
	init_time = (float)clock()/CLOCKS_PER_SEC*10;
	while ((float)clock()/CLOCKS_PER_SEC*10 - init_time < 6)
		mlx_put_image_to_window(game_info->mlx_ptr, game_info->win_ptr, image->img, 200, 200);
	mlx_clear_window(game_info->mlx_ptr, game_info->win_ptr);
	free(game_info->fecha_written);
	game_info->fecha_written = ft_strdup("- ");
	game_info->stage = 0;
}

int get_date(int key_code, gm_inf *game_info)
{
	char *aux_2;
	
	if (key_code == ESC)
		exit(0);
	if (key_code == 65288 && ft_strlen(game_info->fecha_written) != 2)
	{
		aux_2 = game_info->fecha_written;
		game_info->fecha_written = ft_calloc(ft_strlen(aux_2), sizeof(char));
		ft_strlcpy(game_info->fecha_written, aux_2, ft_strlen(aux_2));
		free(aux_2);
		mlx_clear_window(game_info->mlx_ptr, game_info->win_ptr);
	}
	if (game_info->stage == 0)
	{
		if (key_code == ENTER)
		{
			if (!ft_strncmp(game_info->fecha_written, "- 17 12", 8))
			{
				game_info->stage = 1;
				mlx_clear_window(game_info->mlx_ptr, game_info->win_ptr);
			}
			else if (ft_strlen(game_info->fecha_written) != 2)
			{
				game_info->stage = 2;
				mlx_clear_window(game_info->mlx_ptr, game_info->win_ptr);
			}
			
		}
		if(ft_isprint(key_code))
		{
			char aux[2];
			aux[0] = key_code;
			aux_2 = game_info->fecha_written;
			game_info->fecha_written = ft_strjoin(game_info->fecha_written, aux);
			free(aux_2);
		}
	}
	return(0);
}

int game_logic(gm_inf *game_info)
{

	if (game_info->stage == 0)
	{
		mlx_string_put(game_info->mlx_ptr, game_info->win_ptr, 300, 400, WHITE, "Introduce una fecha: (dd mm)");
		mlx_string_put(game_info->mlx_ptr, game_info->win_ptr, 300, 430, WHITE, game_info->fecha_written);
	}
	else if (game_info->stage == 1)
		handle_stage_1(game_info);
	else if (game_info->stage == 2)
		handle_stage_2(game_info);
	else
	{
		if (game_info-> stage == 3)
		{
			mlx_clear_window(game_info->mlx_ptr, game_info->win_ptr);
			mlx_put_image_to_window(game_info->mlx_ptr, game_info->win_ptr, game_info->cake->img, 300, 300);
		}
		render_animated_image(100, 300, &game_info->pacman, game_info);
	}
	return (0);
}

void load_animated_images(t_anim *pacman, void *mlx_ptr)
{
	pacman->frame_1 = resize_image("textures/Garrafa_eating_1.XPM", 0.25, mlx_ptr);
	pacman->frame_2 = resize_image("textures/Garrafa_eating_2.XPM", 0.25, mlx_ptr);
}

int main (void)
{
	gm_inf game_info;

	ft_memset(&game_info, 0, sizeof(gm_inf));
	game_info.fecha_written = ft_strdup("- ");
	game_info.mlx_ptr = mlx_init();
	printf("Cargando imágenes (tarda un poquito...)\n");
	game_info.cake = resize_image("textures/cake.XPM", 0.25, game_info.mlx_ptr);
	load_animated_images(&game_info.pacman, game_info.mlx_ptr);
	game_info.win_ptr = mlx_new_window(game_info.mlx_ptr, 800, 800, "Felicidades?");
	mlx_key_hook(game_info.win_ptr, get_date, &game_info);
	mlx_loop_hook(game_info.mlx_ptr, game_logic, &game_info);
	mlx_loop(game_info.mlx_ptr);
}