/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:01:40 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/19 11:05:45 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

#include <math.h>

bool	control_camera(t_gc *gc)
{
	bool			moved;
	const double	move_step = ((int)gc->scene->control.lctrl_pressed
			* 4.0 + 1.0) * MOVE_STEP / gc->fps;

	moved = false;
	if (gc->scene->control.w_pressed)
		moved |= translate_camera(gc->scene->control.u_control_object.camera,
				DIR_FORWARD, move_step);
	if (gc->scene->control.s_pressed)
		moved |= translate_camera(gc->scene->control.u_control_object.camera,
				DIR_BACKWARD, move_step);
	if (gc->scene->control.a_pressed)
		moved |= translate_camera(gc->scene->control.u_control_object.camera,
				DIR_LEFT, move_step);
	if (gc->scene->control.d_pressed)
		moved |= translate_camera(gc->scene->control.u_control_object.camera,
				DIR_RIGHT, move_step);
	if (gc->scene->control.space_pressed)
		moved |= translate_camera(gc->scene->control.u_control_object.camera,
				DIR_UP, move_step);
	if (gc->scene->control.lshift_pressed)
		moved |= translate_camera(gc->scene->control.u_control_object.camera,
				DIR_DOWN, move_step);
	return (moved);
}

bool	control_object(t_gc *gc)
{
	bool			moved;
	const double	move_step = (gc->scene->control.lctrl_pressed
			* 4.0 + 1.0) * MOVE_STEP / gc->fps;

	moved = false;
	if (gc->scene->control.w_pressed)
		moved |= translate_object(gc->scene->control.u_control_object.object,
				DIR_FORWARD, move_step);
	if (gc->scene->control.s_pressed)
		moved |= translate_object(gc->scene->control.u_control_object.object,
				DIR_BACKWARD, move_step);
	if (gc->scene->control.a_pressed)
		moved |= translate_object(gc->scene->control.u_control_object.object,
				DIR_LEFT, move_step);
	if (gc->scene->control.d_pressed)
		moved |= translate_object(gc->scene->control.u_control_object.object,
				DIR_RIGHT, move_step);
	if (gc->scene->control.space_pressed)
		moved |= translate_object(gc->scene->control.u_control_object.object,
				DIR_UP, move_step);
	if (gc->scene->control.lshift_pressed)
		moved |= translate_object(gc->scene->control.u_control_object.object,
				DIR_DOWN, move_step);
	return (moved);
}

void	manage_controls(t_gc *gc)
{
	bool	moved;

	moved = true;
	if (gc->scene->control.f_pressed && !gc->scene->control.lctrl_pressed)
		gc->scene->fov = fmin(gc->scene->fov + 1, 150);
	else if (gc->scene->control.f_pressed && gc->scene->control.lctrl_pressed)
		gc->scene->fov = fmax(gc->scene->fov - 1, 30);
	else
		moved = false;
	if (gc->scene->control.e_control_type == CAMERA)
		moved |= control_camera(gc);
	else if (gc->scene->control.e_control_type == OBJECT)
		moved |= control_object(gc);
	if (moved)
	{
		gc->last_moved = ft_uptime_linux();
		gc->fully_rendered = false;
		gc->resolution = gc->ideal_resolution;
	}
}
