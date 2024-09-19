/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_window.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 10:58:01 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/19 11:00:05 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "mlx.h"

static void	reinit_images(t_gc *gc)
{
	mlx_destroy_image(gc->mlx, gc->img.img);
	gc->img.img = mlx_new_image(gc->mlx,
			gc->scene->window_width, gc->scene->window_height);
	if (gc->img.img == NULL)
		finish(1, gc);
	gc->img.addr = mlx_get_data_addr(
			gc->img.img, &gc->img.bpp,
			&gc->img.line_length,
			&gc->img.endian);
	(mlx_destroy_image(gc->mlx, gc->img2.img), gc->img2.img = mlx_new_image(
			gc->mlx, gc->scene->window_width, gc->scene->window_height));
	if (gc->img2.img == NULL)
		finish(1, gc);
	gc->img2.addr = mlx_get_data_addr(
			gc->img2.img, &gc->img2.bpp,
			&gc->img2.line_length,
			&gc->img2.endian);
	(mlx_destroy_image(gc->mlx, gc->img3.img), gc->img3.img = mlx_new_image(
			gc->mlx, gc->scene->window_width, gc->scene->window_height));
	if (gc->img3.img == NULL)
		finish(1, gc);
	gc->img3.addr = mlx_get_data_addr(
			gc->img3.img, &gc->img3.bpp,
			&gc->img3.line_length,
			&gc->img3.endian);
}

int	update_window(void *arg1, ...)
{
	t_gc	*gc;
	int		real_width;
	int		real_height;
	int		status;

	gc = arg1;
	status = mlx_get_window_dim(gc->mlx, gc->win, &real_width, &real_height);
	if ((gc->scene->window_width == real_width
			&& gc->scene->window_height == real_height) && !status)
		return (1);
	gc->scene->window_width = real_width;
	gc->scene->window_height = real_height;
	gc->scene->viewport->width = gc->scene->window_width;
	gc->scene->viewport->height = gc->scene->window_height;
	reinit_images(gc);
	setup_hooks(gc);
	return (0);
}
