/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 02:15:45 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/10 21:42:31 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

#include <math.h>

int	parse_camera(char *line, t_scene *scene)
{
	char	**arr;
	t_vec3	tmp;

	arr = ft_split(line, ' ');
	if (!arr[0] || !arr[1] || !arr[2] || !arr[3] || arr[4])
		return (EXIT_FAILURE);
	if (parse_vec3(arr[1], &scene->camera->pos, ANY_VEC))
		return (EXIT_FAILURE);
	if (parse_vec3(arr[2], &scene->camera->dir, NORM_VEC))
		return (EXIT_FAILURE);
	scene->fov = ft_strtof(arr[3]);
	if (isnan(scene->fov) || scene->fov < 0.0 || scene->fov > 180.0)
		return (EXIT_FAILURE);
	copy_vec3(&scene->camera->up, &scene->camera->dir);
	copy_vec3(&tmp, &scene->camera->dir);
	cross_product_vec3(&tmp, &(t_vec3){.x = 0, .y = -1, .z = 0});
	cross_product_vec3(&scene->camera->up, &tmp);
	sc_mult_vec3(&tmp, -1);
	copy_vec3(&scene->camera->right, &tmp);
	return (EXIT_SUCCESS);
}

int	parse_lights(char *line, t_list *lights)
{
	char		**arr;
	t_light		*temp;
	const int	flag = line[0] != 'A';

	arr = ft_split(line, ' ');
	if (!arr[0] || !arr[1] || !arr[2] || (!flag && arr[3])
		|| (flag && !arr[3]) || (flag && arr[4]))
		return (EXIT_FAILURE);
	temp = ft_malloc(sizeof(t_light));
	temp->point = ft_malloc(sizeof(t_vec3));
	if (!flag)
		new_vec3(temp->point, 0, 0, 0);
	else if (parse_vec3(arr[1], temp->point, ANY_VEC))
		return (EXIT_FAILURE);
	temp->ratio = ft_strtof(arr[1 + (flag != 0)]);
	if (isnan(temp->ratio) || temp->ratio < 0.0 || temp->ratio > 1.0)
		return (EXIT_FAILURE);
	if (parse_color(arr[2 + (flag != 0)], &temp->color))
		return (EXIT_FAILURE);
	if (!flag)
		lpush_left(lights, as_ptr(temp));
	else
		lpush(lights, as_ptr(temp));
	return (EXIT_SUCCESS);
}

int	parse_sphere(t_list *objects, char **arr)
{
	t_obj	*temp;

	if (!arr[0] || !arr[1] || !arr[2] || !arr[3] || arr[4])
		return (EXIT_FAILURE);
	temp = ft_malloc(sizeof(t_obj));
	temp->type = TOK_SPHERE;
	temp->sphere.center = ft_malloc(sizeof(t_vec3));
	if (parse_vec3(arr[1], temp->sphere.center, ANY_VEC))
		return (EXIT_FAILURE);
	temp->sphere.radius = ft_strtof(arr[2]) / 2;
	if (isnan(temp->sphere.radius) || temp->sphere.radius <= 0)
		return (EXIT_FAILURE);
	if (parse_color(arr[3], &temp->color))
		return (EXIT_FAILURE);
	lpush(objects, as_ptr(temp));
	return (EXIT_SUCCESS);
}

int	parse_plane(t_list *objects, char **arr)
{
	t_obj	*temp;

	if (!arr[0] || !arr[1] || !arr[2] || !arr[3] || arr[4])
		return (EXIT_FAILURE);
	temp = ft_malloc(sizeof(t_obj));
	temp->type = TOK_PLANE;
	temp->plane.point = ft_malloc(sizeof(t_vec3));
	temp->plane.norm = ft_malloc(sizeof(t_vec3));
	if (parse_vec3(arr[1], temp->plane.point, ANY_VEC))
		return (EXIT_FAILURE);
	if (parse_vec3(arr[2], temp->plane.norm, NORM_VEC))
		return (EXIT_FAILURE);
	if (parse_color(arr[3], &temp->color))
		return (EXIT_FAILURE);
	lpush(objects, as_ptr(temp));
	return (EXIT_SUCCESS);
}

int	parse_cylinder(t_list *objects, char **arr)
{
	t_obj	*temp;

	if (!arr[0] || !arr[1] || !arr[2] || !arr[3] || !arr[4] || !arr[5]
		|| arr[6])
		return (EXIT_FAILURE);
	temp = ft_malloc(sizeof(t_obj));
	temp->type = TOK_CYLINDER;
	temp->cylinder.center = ft_malloc(sizeof(t_vec3));
	temp->cylinder.axis = ft_malloc(sizeof(t_vec3));
	if (parse_vec3(arr[1], temp->cylinder.center, ANY_VEC))
		return (EXIT_FAILURE);
	if (parse_vec3(arr[2], temp->cylinder.axis, NORM_VEC))
		return (EXIT_FAILURE);
	temp->cylinder.radius = ft_strtof(arr[3]) / 2;
	if (isnan(temp->cylinder.radius) || temp->cylinder.radius <= 0)
		return (EXIT_FAILURE);
	temp->cylinder.height = ft_strtof(arr[4]);
	if (isnan(temp->cylinder.height) || temp->cylinder.height <= 0)
		return (EXIT_FAILURE);
	if (parse_color(arr[5], &temp->color))
		return (EXIT_FAILURE);
	new_cylinder(&temp->cylinder, &temp->cylinder);
	lpush(objects, as_ptr(temp));
	return (EXIT_SUCCESS);
}
