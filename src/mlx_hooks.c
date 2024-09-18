/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 11:11:35 by tosuman           #+#    #+#             */
/*   Updated: 2024/09/18 08:36:24 by tosuman          ###   ########.fr       */
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

void	select_object(t_gc *gc, int x, int y)
{
	t_hit	hit;
	t_ray	ray;
	t_vec3	terminus;
	t_vec3	pixel;
	int		i;

	copy_vec3(&terminus, &gc->scene->camera->pos);
	copy_vec3(&pixel, &gc->scene->viewport->top_left);
	i = -1;
	while (++i < y)
		add_vec3(&pixel, &gc->scene->viewport->down_step);
	i = -1;
	while (++i < x)
		add_vec3(&pixel, &gc->scene->viewport->right_step);
	new_ray(&ray, &terminus, &pixel);
	if (!cast_ray(&hit, &ray, gc->scene))
	{
		gc->scene->control.e_control_type = OBJECT;
		gc->scene->control.u_control_object.object = hit.object;
		i = -1;
		while (++i < gc->scene->nb_objs)
			gc->scene->objects[i]->selected = false;
		hit.object->selected = true;
	}
}

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

int	sync_movement(int keycode, t_gc *gc, bool pressed)
{
	if (keycode == 'q')
		destroy_hook(gc);
	else if (keycode == XK_Escape && pressed)
	{
		if (gc->scene->control.e_control_type == MENU)
			destroy_hook(gc);
		else if (gc->scene->control.e_control_type == CAMERA)
		{
			gc->scene->control.e_control_type = MENU;
			mlx_mouse_show(gc->mlx, gc->win);
			gc->mouse_hidden = false;
		}
		else if (gc->scene->control.e_control_type == OBJECT)
		{
			gc->scene->control.e_control_type = CAMERA;
			gc->scene->control.u_control_object.camera = gc->scene->camera;
		}
	}
	else if (keycode == 'f')
		gc->scene->control.f_pressed = pressed;
	else if (keycode == 'w')
		gc->scene->control.w_pressed = pressed;
	else if (keycode == 'a')
		gc->scene->control.a_pressed = pressed;
	else if (keycode == 's')
		gc->scene->control.s_pressed = pressed;
	else if (keycode == 'd')
		gc->scene->control.d_pressed = pressed;
	else if (keycode == ' ')
		gc->scene->control.space_pressed = pressed;
	else if (keycode == XK_Shift_L)
		gc->scene->control.lshift_pressed = pressed;
	else if (keycode == XK_Control_L)
		gc->scene->control.lctrl_pressed = pressed;
	else
		return (0);
	return (1);
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
	gc->last_moved = ft_uptime_linux();
	gc->fully_rendered = false;
	gc->resolution = gc->ideal_resolution;
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
