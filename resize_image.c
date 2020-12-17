/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resize_image.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aiglesia <aiglesia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 09:09:25 by aiglesia          #+#    #+#             */
/*   Updated: 2020/12/17 11:54:27 by aiglesia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include "libft.h"
#include <mlx.h>

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

t_data		*initialize_image(void *mlx_ptr, int size_x, int size_y)
{
	t_data *image;

	if (!(image = ft_calloc(1, sizeof(t_data))))
		return (0);
	image->img = mlx_new_image(mlx_ptr, size_x, size_y);
	image->width = size_x;
	image->height = size_y;
	image->addr = mlx_get_data_addr(image->img, &image->bits_per_pixel,
	&image->line_length, &image->endian);
	return (image);
}

t_data		*load_xpm_image(void *mlx_ptr, char *path)
{
	t_data *image;

	if (!(image = ft_calloc(1, sizeof(t_data))))
		return (0);
	if (!(image->img = mlx_xpm_file_to_image(mlx_ptr, path, &image->width,
	&image->height)))
		return (0);
	image->addr = mlx_get_data_addr(image->img, &image->bits_per_pixel,
	&image->line_length, &image->endian);
	return (image);
}

unsigned	get_pixel(t_data *image, int x, int y)
{
	return (*((unsigned int*)(image->addr +
	(y * image->line_length + x * (image->bits_per_pixel / 8)))));
}

void		draw_pixel(t_data *image, int x, int y, int color)
{
	*(unsigned int*)(image->addr + (y * image->line_length + x
	* (image->bits_per_pixel / 8))) = color;
}

void interpolate_image(t_data *original, t_data *new)
{
	int i;
	int j;

	j = 0;
	i = 0;
	while (i < new->width)
	{
		while (j < new->height)
		{
			draw_pixel(new, i, j, get_pixel(original, i /(float)new->width * original->width, j /(float)new->height * original->height));
			j++;
		}
		
		i++;
		j = 0;
	}
}

t_data *resize_image(char *path, float size_ratio, void *mlx_ptr)
{
	int screen_size = 800;
	int image_size;
	t_data *aux;
	t_data *final_image;

	aux = load_xpm_image(mlx_ptr, path);
	image_size = screen_size *size_ratio;
	final_image = initialize_image(mlx_ptr, image_size, image_size);
	interpolate_image(aux, final_image);
	return(final_image);
}