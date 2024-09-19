/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keycode.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 10:50:29 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/19 10:52:59 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "mlx.h"

void	q_pressed(t_gc *gc)
{
	int	i;

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
	i = -1;
	while (++i < gc->scene->nb_objs)
		gc->scene->objects[i]->selected = false;
}

void	wasd1234_pressed(int keycode, t_gc *gc, bool pressed)
{
	if (keycode == 'w')
		gc->scene->control.w_pressed = pressed;
	else if (keycode == 'a')
		gc->scene->control.a_pressed = pressed;
	else if (keycode == 's')
		gc->scene->control.s_pressed = pressed;
	else if (keycode == 'd')
		gc->scene->control.d_pressed = pressed;
	else if (keycode == '1')
		gc->antialiasing = 0;
	else if (keycode == '2')
		gc->antialiasing = 1;
	else if (keycode == '3')
		gc->interpolation = 0;
	else if (keycode == '4')
		gc->interpolation = 1;
}

int	sync_movement(int keycode, t_gc *gc, bool pressed)
{
	if (keycode == XK_Escape)
		destroy_hook(gc);
	else if (keycode == 'q' && pressed)
		q_pressed(gc);
	else if (keycode == 'f')
		gc->scene->control.f_pressed = pressed;
	else if (ft_strchr("wasd1234", keycode))
		wasd1234_pressed(keycode, gc, pressed);
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
