/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   antialiasing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 21:02:35 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/12 21:02:35 by nam-vu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include <stdio.h>
#include <math.h>

int	edge_detection(t_rt_img *img, int x, int y, int rs)
{
	t_vec3		grad[2];
	const int	kernel_x[3][3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
	const int	kernel_y[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
	int			i[2];
	t_color		temp;

	ft_memset(grad, 0, sizeof(t_vec3) * 2);
	i[0] = -1;
	while (++(i[0]) < 3)
	{
		i[1] = -1;
		while (++(i[1]) < 3)
		{
			temp = mlx_pixel_get_buf(img,
					x + (i[0] - 1) * rs, y + (i[1] - 1) * rs);
			grad[0].x += kernel_x[2 - i[0]][2 - i[1]] * (int)get_red(&temp);
			grad[0].y += kernel_x[2 - i[0]][2 - i[1]] * (int)get_green(&temp);
			grad[0].z += kernel_x[2 - i[0]][2 - i[1]] * (int)get_blue(&temp);
			grad[1].x += kernel_y[2 - i[0]][2 - i[1]] * (int)get_red(&temp);
			grad[1].y += kernel_y[2 - i[0]][2 - i[1]] * (int)get_green(&temp);
			grad[1].z += kernel_y[2 - i[0]][2 - i[1]] * (int)get_blue(&temp);
		}
	}
	return (length_squared_vec3(grad)
		+ length_squared_vec3(grad + 1) >= EPSILON);
}

void	gaussian_blur(t_antialiasing *data)
{
	int	i;
	int	j;

	data->res_color = 0;
	i = -2;
	while (++i < 2)
	{
		j = -2;
		while (++j < 2)
		{
			data->tmp_color = mlx_pixel_get_buf(&data->gc->img,
					data->x + i * data->resolution,
					data->y + j * data->resolution);
			set_red(&data->res_color, get_red(&data->res_color)
				+ get_red(&data->tmp_color) / 9);
			set_green(&data->res_color, get_green(&data->res_color)
				+ get_green(&data->tmp_color) / 9);
			set_blue(&data->res_color, get_blue(&data->res_color)
				+ get_blue(&data->tmp_color) / 9);
		}
	}
}

void	put_blurred_pixel(t_antialiasing *data)
{
	int	i;
	int	j;

	i = -1;
	while (++i < data->resolution)
	{
		j = -1;
		while (++j < data->resolution)
			mlx_pixel_put_buf(&data->gc->img2, data->x + i,
				data->y + j, data->res_color);
	}
}

void	antialiasing(t_gc *gc, int width, int height, int resolution)
{
	t_antialiasing	data;

	data = (t_antialiasing){.gc = gc, .width = width,
		.height = height, .resolution = resolution};
	data.y = 0;
	while (data.y + data.resolution - 1 < data.height)
	{
		data.x = 0;
		while (data.x + data.resolution - 1 < data.width)
		{
			if (data.x > data.resolution && data.y > data.resolution
				&& data.x + 2 * data.resolution - 1 < data.width
				&& data.y + 2 * data.resolution - 1 < data.height
				&& (edge_detection(&gc->img3, data.x, data.y, data.resolution)))
				gaussian_blur(&data);
			else
				data.res_color = mlx_pixel_get_buf(&gc->img, data.x, data.y);
			put_blurred_pixel(&data);
			data.x += data.resolution;
		}
		data.y += data.resolution;
	}
}
