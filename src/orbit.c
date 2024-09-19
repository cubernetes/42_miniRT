/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orbit.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 10:56:01 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/19 10:56:27 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _XOPEN_SOURCE 600

#include "miniRT.h"

#include <math.h>
#include <unistd.h>

void	orbit(t_gc *gc, t_scene *scene)
{
	const float	angle = 3;
	const float	amount = 2 * 200 * sinf(angle * 3 / 180);
	int			i;

	render(gc);
	i = -1;
	while (++i < 120 * 1)
	{
		render(gc);
		rotate_camera(scene->camera, DIR_RIGHT, angle);
		translate_camera(scene->camera, DIR_LEFT, amount);
		rotate_camera(scene->camera, DIR_RIGHT, angle);
		usleep(10000);
	}
}
		/* printf("%f,%f,%f\n", scene->camera->dir.x, scene->camera->dir.y, */
			/* scene->camera->dir.z); */
