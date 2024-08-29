/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 20:34:56 by nam-vu            #+#    #+#             */
/*   Updated: 2024/08/28 03:40:05 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <stdlib.h>

/* todo: make array of pointers instead of normal array */
/* todo: use vec3 structs instead of pointers to vec3 structs,
 * reduces fragmentation and the number of calls to (ft_)malloc
 */
int	parse_input(t_obj **objects, t_scene *scene)
{
	const double	radius = 20;

	scene->nb_lights = 2;
	scene->lights = ft_malloc((unsigned int)scene->nb_lights * sizeof(t_light));
	scene->lights[0].ratio = 0.0;//ambience lighting
	scene->lights[0].point = ft_malloc(sizeof(t_vec3));
	scene->lights[0].color = 0x00FFFFFF;
	new_vec3(scene->lights[0].point, 0, 0, -16);
	scene->lights[1].ratio = 0.5;
	scene->lights[1].point = ft_malloc(sizeof(t_vec3));
	scene->lights[1].color = 0x00FFFFFF;
	new_vec3(scene->lights[1].point, 1000, 0, -41);//500
	scene->nb_objs = 3;
	*objects = ft_malloc(sizeof(t_obj) * (size_t)scene->nb_objs);
	(*objects)[0].type = SPHERE;
	(*objects)[0].color = 0x00da2b27;
	(*objects)[0].sphere.center = ft_malloc(sizeof(t_vec3));
	new_vec3((*objects)[0].sphere.center, 0, 0, -41);
	new_sphere(&(*objects)[0].sphere, (*objects)[0].sphere.center, radius);
	(*objects)[1].type = PLANE;
	(*objects)[1].color = 0x00ce9d52;
	(*objects)[1].plane.norm = ft_malloc(sizeof(t_vec3));
	new_vec3((*objects)[1].plane.norm, 0, 1, 0);
	(*objects)[1].plane.point = ft_malloc(sizeof(t_vec3));
	new_vec3((*objects)[1].plane.point, 0, -50, 0);
	new_plane(&(*objects)[1].plane, (*objects)[1].plane.point, (*objects)[1].plane.norm);
	(*objects)[2].type = PLANE;
	(*objects)[2].color = 0x00f68656;
	(*objects)[2].plane.norm = ft_malloc(sizeof(t_vec3));
	new_vec3((*objects)[2].plane.norm, 0, 1, 0);
	(*objects)[2].plane.point = ft_malloc(sizeof(t_vec3));
	new_vec3((*objects)[2].plane.point, 0, 50, 0);
	new_plane(&(*objects)[2].plane, (*objects)[2].plane.point, (*objects)[2].plane.norm);
	return (EXIT_SUCCESS);
}
