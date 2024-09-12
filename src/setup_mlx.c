/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_mlx.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:47:11 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/11 22:44:39 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include "mlx.h"

#include <X11/X.h>
#include <stdbool.h>

static void	setup_hooks(t_gc *gc)
{
	mlx_hook(gc->win, DestroyNotify, NoEventMask, (t_hook)destroy_hook, gc);
	mlx_hook(gc->win, KeyPress, KeyPressMask, (t_hook)keydown_hook, gc);
	/* mlx_hook(gc->win, KeyPress, ButtonPressMask, (t_hook)keydown_hook, gc); */
	mlx_hook(gc->win, MotionNotify, PointerMotionMask, (t_hook)move_hook, gc);
}

void	unlock_camera(t_gc *gc)
{
	gc->scene->camera->locked = true;
}

void	lock_camera(t_gc *gc)
{
	gc->scene->camera->locked = true;
}

void	setup_mlx(t_gc *gc, t_scene *scene)
{
	gc->mlx = gc_add(mlx_init())->last->as_ptr;
	// TODO: /\ check NULL
	gc->win = mlx_new_window(gc->mlx, scene->window_width, scene->window_height,
			"miniRT");
	// /\ TODO: check NULL
	gc->img.img = mlx_new_image(gc->mlx, scene->window_width,
			scene->window_height);
	// /\ TODO: check NULL
	gc->img.addr = mlx_get_data_addr(
			gc->img.img, &gc->img.bpp,
			&gc->img.line_length,
			&gc->img.endian);
	gc->img2.img = mlx_new_image(gc->mlx, scene->window_width,
			scene->window_height);
	// /\ TODO: check NULL
	gc->img2.addr = mlx_get_data_addr(
			gc->img2.img, &gc->img2.bpp,
			&gc->img2.line_length,
			&gc->img2.endian);
	gc->img2.addr = mlx_get_data_addr(
			gc->img2.img, &gc->img2.bpp,
			&gc->img2.line_length,
			&gc->img2.endian);
	// /\ TODO: check NULL
	setup_hooks(gc);
	unlock_camera(gc);
}
