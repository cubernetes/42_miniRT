/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select_object.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 10:49:46 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/19 10:49:56 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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
