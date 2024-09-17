/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 11:11:35 by tosuman           #+#    #+#             */
/*   Updated: 2024/09/17 03:16:12 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include "mlx.h"
#include "mlx_int.h"

#include <stdint.h> /* needed for intptr_t */
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

int	destroy_hook(void *arg1, ...)
{
	t_gc	*gc;

	gc = arg1;
	finish(0, gc);
	return (0);
}

int	move_hook(void *arg1, ...)
{
	va_list	ap;
	t_gc	*gc;
	int		x;
	int		y;

	x = (int)(intptr_t)arg1;
	va_start(ap, arg1);
	y = va_arg(ap, int);
	gc = va_arg(ap, t_gc *);
	va_end(ap);
	if (gc->scene->window_width / 2 - x != 0
		|| gc->scene->window_height / 2 - y != 0)
	{
		camera_yaw(gc->scene, gc->scene->window_width / 2 - x);
		camera_pitch(gc->scene, gc->scene->window_height / 2 - y);
		mlx_mouse_move(gc->mlx, gc->win, gc->scene->window_width / 2, gc->scene->window_height / 2);
		gc->last_moved = ft_uptime_linux();
		gc->fully_rendered = false;
		gc->resolution = RESOLUTION;
	}
	return (0);
}

/* cast to intptr_t and then int needed. only way to make this generic */
int	keydown_hook(void *arg1, ...)
{
	int		keycode;
	va_list	ap;
	t_gc	*gc;
	t_quat	quat;
	t_quat	quat2;

	new_unit_quat(&quat, 10, &(t_vec3){.x = 0, .y = 1, .z = 0});
	new_unit_quat(&quat2, -10, &(t_vec3){.x = 0, .y = 1, .z = 0});
	keycode = (int)(intptr_t)arg1;
	va_start(ap, arg1);
	gc = va_arg(ap, t_gc *);
	va_end(ap);
	gc->last_moved = ft_uptime_linux();
	gc->fully_rendered = false;
	gc->resolution = RESOLUTION;
	if (keycode == XK_Escape || keycode == 'q')
		destroy_hook(gc);
	else if (keycode == 'w')
		translate_camera(gc->scene, DIR_FORWARD, MOVE_STEP);
	else if (keycode == 's')
		translate_camera(gc->scene, DIR_BACKWARD, MOVE_STEP);
	else if (keycode == 'a')
		translate_camera(gc->scene, DIR_LEFT, MOVE_STEP);
	else if (keycode == 'd')
		translate_camera(gc->scene, DIR_RIGHT, MOVE_STEP);
	else if (keycode == ' ')
		translate_camera(gc->scene, DIR_UP, MOVE_STEP);
	else if (keycode == XK_Shift_L)
		translate_camera(gc->scene, DIR_DOWN, MOVE_STEP);
	else
	{
		ft_printf("Pressed '%c' (keycode: %d)\n", keycode, keycode);
		return (0);
	}
	return (0);
}
/* TODO: Remove 'pressed' debug output */

/* Damn... */
/* void	update_dimensions(t_gc *gc) */
/* { */
	/* t_xvar		*xvar; */
	/* t_win_list	*win; */
	/* Window		root; */
/*  */
	/* xvar = gc->mlx; */
	/* win = gc->win; */
	/* XGetGeometry(xvar->display, */
		/* win->window, */
		/* &root, */
		/* &gc->x, */
		/* &gc->y, */
		/* (unsigned int *)&gc->scene->window_width, */
		/* (unsigned int *)&gc->scene->window_height, */
		/* &(unsigned int){0}, */
		/* &(unsigned int){0}); */
	/* XTranslateCoordinates(xvar->display, */
		/* win->window, */
		/* root, */
		/* 0, */
		/* 0, */
		/* &gc->x, */
		/* &gc->y, */
		/* &root); */
	/* gc->scene->camera->center_x = gc->x + gc->scene->window_width / 2; */
	/* gc->scene->camera->center_y = gc->y + gc->scene->window_height / 2; */
/* } */

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
	mlx_destroy_image(gc->mlx, gc->img.img);
	// todo: reinit img2, img3, etc.
	gc->img.img = mlx_new_image(gc->mlx,
		gc->scene->window_width, gc->scene->window_height);
	gc->img.addr = mlx_get_data_addr(
			gc->img.img, &gc->img.bpp,
			&gc->img.line_length,
			&gc->img.endian);
	setup_hooks(gc);
	return (0);
}
