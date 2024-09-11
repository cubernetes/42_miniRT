/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:42:43 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/11 21:28:03 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include "mlx.h"

#include <X11/keysym.h>
#include <stdint.h> /* needed for intptr_t */

void	mlx_pixel_put_buf(t_rt_img *data, int x, int y, t_color color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bpp / 8));
	*(unsigned int *)dst = color;
}

int	destroy_hook(void *arg1, ...)
{
	t_gc	*gc;

	gc = arg1;
	finish(0, gc);
	return (0);
}

void	camera_yaw(t_scene *scene, int amount)
{
	rotate_camera(scene->camera, DIR_LEFT, amount / 40.0);
}

void	camera_pitch(t_scene *scene, int amount)
{
	rotate_camera(scene->camera, DIR_UP, amount / 40.0);
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
	/* mlx_mouse_get_pos(gc->mlx, gc->win, &x, &y); */
	/* ft_printf("%d, %d\n", x, y); */
	camera_yaw(gc->scene, gc->scene->camera->prev_x - x);
	camera_pitch(gc->scene, gc->scene->camera->prev_y - y);
	mlx_mouse_move(gc->mlx, gc->win, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
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
