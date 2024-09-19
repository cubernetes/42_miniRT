/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 11:11:35 by tosuman           #+#    #+#             */
/*   Updated: 2024/09/19 13:31:32 by tischmid         ###   ########.fr       */
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

int	mouse_down_hook(void *arg1, ...)
{
	va_list	ap;
	t_gc	*gc;
	int		x;
	int		y;
	int		button;

	(void)x;
	(void)y;
	(void)gc;
	button = (int)(intptr_t)arg1;
	va_start(ap, arg1);
	x = va_arg(ap, int);
	y = va_arg(ap, int);
	gc = va_arg(ap, t_gc *);
	va_end(ap);
	if (button == Button1 && gc->scene->control.e_control_type == MENU)
	{
		gc->scene->control.e_control_type = CAMERA;
		gc->scene->control.u_control_object.camera = gc->scene->camera;
		if (!gc->mouse_hidden)
			mlx_mouse_hide(gc->mlx, gc->win);
	}
	else
		select_object(gc, x, y);
	return (0);
}

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
	if (gc->scene->control.e_control_type == MENU)
		return (0);
	if (gc->scene->window_width / 2 - x != 0
		|| gc->scene->window_height / 2 - y != 0)
	{
		camera_yaw(gc->scene, gc->scene->window_width / 2 - x);
		camera_pitch(gc->scene, gc->scene->window_height / 2 - y);
		mlx_mouse_move(gc->mlx, gc->win, gc->scene->window_width / 2,
			gc->scene->window_height / 2);
		gc->last_moved = ft_uptime_linux();
		gc->fully_rendered = false;
		gc->resolution = gc->ideal_resolution;
	}
	return (0);
}

/* cast to intptr_t and then int needed. only way to make this generic */
int	keydown_hook(void *arg1, ...)
{
	int		keycode;
	va_list	ap;
	t_gc	*gc;

	keycode = (int)(intptr_t)arg1;
	va_start(ap, arg1);
	gc = va_arg(ap, t_gc *);
	va_end(ap);
	sync_movement(keycode, gc, true);
	return (0);
}

/* cast to intptr_t and then int needed. only way to make this generic */
int	keyup_hook(void *arg1, ...)
{
	int		keycode;
	va_list	ap;
	t_gc	*gc;

	keycode = (int)(intptr_t)arg1;
	va_start(ap, arg1);
	gc = va_arg(ap, t_gc *);
	va_end(ap);
	sync_movement(keycode, gc, false);
	return (0);
}
