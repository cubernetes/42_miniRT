/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orbit.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 10:56:01 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/19 13:02:55 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#define _XOPEN_SOURCE 600

#include "miniRT.h"

#include <math.h>
#include <unistd.h>

void	orbit(t_gc *gc, t_scene *scene)
{
	const double	angle = 3;
	const double	amount = 2 * 200 * sin(angle * 3 / 180);
	int			i;

	render(gc);
	i = -1;
	while (++i < 120 * 1)
	{
		render(gc);
		rotate_camera(scene->camera, DIR_RIGHT, angle);
		translate_camera(scene->camera, DIR_LEFT, amount);
		rotate_camera(scene->camera, DIR_RIGHT, angle);
		ft_sleep_linux(10);
	}
}
		/* printf("%f,%f,%f\n", scene->camera->dir.x, scene->camera->dir.y, */
			/* scene->camera->dir.z); */
