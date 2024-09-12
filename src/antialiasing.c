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

static int	mod(int a, int b)
{
	const int	r = a % b;

	if (r < 0)
		return (r + b);
	return (r);
}

void	apply_random_antialiasing(t_gc *gc, int width, int height)
{
	int			x;
	int			y;
	int			i;
	int			rand;
	t_color		res_color;
	t_color		tmp_color;
	const int	sample_amount = 4;

	y = -1;
	while (++y < height)
	{
		x = -1;
		while (++x < width)
		{
			res_color = 0;
			i = -1;
			while (++i < sample_amount)
			{
				rand = ft_rand();
				tmp_color = mlx_pixel_get_buf(&gc->img, x + (mod((rand / 3), 3)) - 1, y + (mod(rand, 3)) - 1);
				set_red(&res_color, get_red(&res_color) + get_red(&tmp_color) / sample_amount);
				set_green(&res_color, get_green(&res_color) + get_green(&tmp_color) / sample_amount);
				set_blue(&res_color, get_blue(&res_color) + get_blue(&tmp_color) / sample_amount);
			}
			mlx_pixel_put_buf(&gc->img2, x, y, res_color);
		}
	}
}

static int	edge_detection(t_rt_img *img, int x, int y, int resolution)
{
	if (mlx_pixel_get_buf(img, x - resolution, y - resolution) == 0
		|| mlx_pixel_get_buf(img, x, y - resolution) == 0
		|| mlx_pixel_get_buf(img, x + resolution, y - resolution) == 0
		|| mlx_pixel_get_buf(img, x - resolution, y) == 0
		|| mlx_pixel_get_buf(img, x, y) == 0
		|| mlx_pixel_get_buf(img, x, y + resolution) == 0
		|| mlx_pixel_get_buf(img, x - resolution, y + resolution) == 0
		|| mlx_pixel_get_buf(img, x, y + resolution) == 0
		|| mlx_pixel_get_buf(img, x + resolution, y + resolution) == 0)
		return (1);
	return (0);
}

void	apply_pattern_antialiasing(t_gc *gc, int width, int height, int resolution)
{
	int			x;
	int			y;
	t_color		res_color;
	t_color		tmp_color;

	y = 0;
	while (y + resolution - 1 < height)
	{
		x = 0;
		while (x + resolution - 1 < width)
		{
			if (edge_detection(&gc->img, x, y, resolution))
			{
				if (x > resolution && y > resolution)
				{
					res_color = 0;
					for (int i = -1; i < 2; i++)
					{
						for (int j = -1; j < 2; j++)
						{
							tmp_color = mlx_pixel_get_buf(&gc->img, x + i * resolution, y + j * resolution);
							set_red(&res_color, get_red(&res_color) + get_red(&tmp_color) / 9);
							set_green(&res_color, get_green(&res_color) + get_green(&tmp_color) / 9);
							set_blue(&res_color, get_blue(&res_color) + get_blue(&tmp_color) / 9);
						}
					}
				}
				else
					res_color = mlx_pixel_get_buf(&gc->img, x, y);
			}
			else
				res_color = mlx_pixel_get_buf(&gc->img, x, y);
			for (int i = 0; i < resolution; i++)
			{
				for (int j = 0; j < resolution; j++)
				{
					mlx_pixel_put_buf(&gc->img2, x + i, y + j, res_color);
				}
			}
			x += resolution;
		}
		y += resolution;
	}
}
