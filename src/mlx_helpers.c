/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:42:43 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/10 21:09:14 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

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
		translate_camera(gc->scene, DIR_FORWARD, 1);
	else if (keycode == 's')
		translate_camera(gc->scene, DIR_BACKWARD, 1);
	else if (keycode == 'a')
		translate_camera(gc->scene, DIR_LEFT, 1);
	else if (keycode == 'd')
		translate_camera(gc->scene, DIR_RIGHT, 1);
	else if (keycode == ' ')
		translate_camera(gc->scene, DIR_UP, 1);
	else if (keycode == XK_Shift_L)
		translate_camera(gc->scene, DIR_DOWN, 1);
	else if (keycode == 'h')
		rotate_camera(gc->scene->camera, DIR_LEFT, 15);
	else if (keycode == 'l')
		rotate_camera(gc->scene->camera, DIR_RIGHT, 15);
	else if (keycode == '9')
		rotate_object(gc->scene->objects[0], &quat);
	else if (keycode == '0')
		rotate_object(gc->scene->objects[0], &quat2);
	else if (keycode == '7')
		gc->scene->fov -= 1;
	else if (keycode == '8')
		gc->scene->fov += 1;
	else
	{
		ft_printf("Pressed '%c' (keycode: %d)\n", keycode, keycode);
		return (0);
	}
	render(gc, gc->scene);
	return (0);
}
/* TODO: Remove 'pressed' debug output */
