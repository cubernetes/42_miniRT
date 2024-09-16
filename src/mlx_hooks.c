/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 11:11:35 by tosuman           #+#    #+#             */
/*   Updated: 2024/09/16 23:07:35 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "miniRT.h"
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
	if (gc->scene->camera->prev_x - x != 0
		|| gc->scene->camera->prev_y - y != 0)
	{
		camera_yaw(gc->scene, gc->scene->camera->prev_x - x);
		camera_pitch(gc->scene, gc->scene->camera->prev_y - y);
		mlx_mouse_move(gc->mlx, gc->win, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		gc->last_moved = ft_uptime_linux();
		gc->fully_rendered = false;
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
	if (keycode == XK_Escape || keycode == 'q')
		destroy_hook(gc);
	else if (keycode == 'w')
		translate_camera(gc->scene, DIR_FORWARD, 2);
	else if (keycode == 's')
		translate_camera(gc->scene, DIR_BACKWARD, 2);
	else if (keycode == 'a')
		translate_camera(gc->scene, DIR_LEFT, 2);
	else if (keycode == 'd')
		translate_camera(gc->scene, DIR_RIGHT, 2);
	else if (keycode == ' ')
		translate_camera(gc->scene, DIR_UP, 2);
	else if (keycode == XK_Shift_L)
		translate_camera(gc->scene, DIR_DOWN, 2);
	else
	{
		ft_printf("Pressed '%c' (keycode: %d)\n", keycode, keycode);
		return (0);
	}
	return (0);
}
/* TODO: Remove 'pressed' debug output */

int	mlx_get_window_dim(void *mlx_ptr, void *win_ptr, int *width, int *height)
{
	XWindowAttributes	w_attr;
	int					status;

	status = XGetWindowAttributes(((t_xvar *)mlx_ptr)->display,
			((t_win_list *)win_ptr)->window, &w_attr);
	*width = w_attr.width;
	*height = w_attr.height;
	return (status);
}

int	mlx_int_resizable_win(t_xvar *xvar, Window win, int w, int h)
{
	XSizeHints			hints;
	long				toto;
	XWindowAttributes	w_attr;

	XGetWMNormalHints(xvar->display, win, &hints, &toto);
	XGetWindowAttributes(xvar->display, xvar->root, &w_attr);
	hints.width = w;
	hints.height = h;
	hints.min_width = 10;
	hints.min_height = 10;
	hints.max_width = w_attr.width;
	hints.max_height = w_attr.height;
	hints.flags = USPosition | USSize | PMinSize | PMaxSize;
	hints.win_gravity = 1;
	hints.x = 0;
	hints.y = 0;
	hints.width_inc = 1;
	hints.height_inc = 1;
	hints.base_width = 100;
	hints.base_height = 100;
	hints.max_aspect.x = 2;
	hints.max_aspect.y = 1;
	hints.min_aspect.x = 1;
	hints.min_aspect.y = 2;
	return (XSetWMNormalHints(xvar->display, win, &hints), 0);
}
