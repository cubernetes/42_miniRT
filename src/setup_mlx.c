/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_mlx.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:47:11 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/19 10:48:45 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include "mlx.h"

#include <X11/X.h>
#include <stdbool.h>

void	setup_hooks(t_gc *gc)
{
	mlx_hook(gc->win, DestroyNotify, NoEventMask, (t_hook)destroy_hook, gc);
	mlx_hook(gc->win, KeyPress, KeyPressMask, (t_hook)keydown_hook, gc);
	mlx_hook(gc->win, KeyRelease, KeyReleaseMask, (t_hook)keyup_hook, gc);
	mlx_hook(gc->win, ButtonPress, ButtonPressMask,
		(t_hook)mouse_down_hook, gc);
	mlx_hook(gc->win, MotionNotify, PointerMotionMask, (t_hook)move_hook, gc);
	mlx_hook(gc->win, ConfigureNotify, StructureNotifyMask,
		(t_hook)update_window, gc);
}
	/* mlx_hook(gc->win, ButtonRelease, ButtonReleaseMask,
	 * (t_hook)mouse_up_hook, gc); */

void	setup_images(t_gc *gc, t_scene *scene)
{
	gc->img.img = mlx_new_image(gc->mlx, scene->window_width,
			scene->window_height);
	if (gc->img.img == NULL)
		finish(1, gc);
	gc->img.addr = mlx_get_data_addr(
			gc->img.img, &gc->img.bpp,
			&gc->img.line_length,
			&gc->img.endian);
	gc->img2.img = mlx_new_image(gc->mlx, scene->window_width,
			scene->window_height);
	if (gc->img2.img == NULL)
		finish(1, gc);
	gc->img2.addr = mlx_get_data_addr(
			gc->img2.img, &gc->img2.bpp,
			&gc->img2.line_length,
			&gc->img2.endian);
	gc->img3.img = mlx_new_image(gc->mlx, scene->window_width,
			scene->window_height);
	if (gc->img3.img == NULL)
		finish(1, gc);
	gc->img3.addr = mlx_get_data_addr(
			gc->img3.img, &gc->img3.bpp,
			&gc->img3.line_length,
			&gc->img3.endian);
}

void	setup_mlx(t_gc *gc, t_scene *scene)
{
	gc->mlx = gc_add(mlx_init())->last->as_ptr;
	if (gc->mlx == NULL)
		finish(1, gc);
	gc->win = mlx_new_resizable_window(gc->mlx, scene->window_width,
			scene->window_height, "miniRT");
	if (gc->win == NULL)
		finish(1, gc);
	setup_images(gc, scene);
	setup_hooks(gc);
	mlx_do_key_autorepeatoff(gc->mlx);
}
