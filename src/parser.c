/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 02:14:14 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/17 03:48:11 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int	parse_vec3(char *str, t_vec3 *vec, int flag)
{
	int		i;
	char	*ptr;
	char	*next_ptr;

	if (!str)
		return (EXIT_FAILURE);
	i = -1;
	ptr = str;
	while (++i < 3)
	{
		next_ptr = ft_strchr(ptr, ',');
		if (i < 2 && !next_ptr)
			return (EXIT_FAILURE);
		if (i == 2)
			vec->e[i] = ft_strtof(ft_strdup(ptr));
		else
			vec->e[i] = ft_strtof(ft_strndup(ptr, (size_t)(next_ptr - ptr)));
		if (isnan(vec->e[i]))
			return (EXIT_FAILURE);
		ptr = next_ptr + 1;
	}
	//if (flag == NORM_VEC && (length_squared_vec3(vec) - 1.0) > EPSILON)
	if ((flag == NORM_VEC && (fabs(vec->x) > 1.0
		|| fabs(vec->y) > 1.0 || fabs(vec->z)> 1.0)) && length_squared_vec3(vec) > 0.0)
		return (EXIT_FAILURE);
	if (flag == NORM_VEC)
		unit_vec3(vec);
	return (EXIT_SUCCESS);
}

int	parse_color(char *str, t_color *color)
{
	int		i;
	char	*ptr;
	char	*next_ptr;
	float	temp[3];

	*color = 0;
	if (!str)
		return (EXIT_FAILURE);
	i = -1;
	ptr = str;
	while (++i < 3)
	{
		next_ptr = ft_strchr(ptr, ',');
		if (i < 2 && !next_ptr)
			return (EXIT_FAILURE);
		if (i == 2)
			temp[i] = ft_strtof(ft_strdup(ptr));
		else
			temp[i] = ft_strtof(ft_strndup(ptr, (size_t)(next_ptr - ptr)));
		if (isnan(temp[i]) || temp[i] > 255.0 || temp[i] < 0.0)
			return (EXIT_FAILURE);
		ptr = next_ptr + 1;
	}
	*color = (unsigned int)
		(((int)(temp[0]) << 16) + ((int)(temp[1]) << 8) + ((int)(temp[2])));
	return (EXIT_SUCCESS);
}

int	read_rt_file(char *file, t_scene *scene)
{
	int			fd;
	char		*line;
	t_list		*objects;
	t_list		*lights;
	int			has_cam;
	int			has_amb;

	if (open_rt_file(file, &fd))
		return (EXIT_FAILURE);
	has_cam = init_parse(&objects, &lights, &line, fd);
	has_amb = 0;
	while (line)
	{
		if (line[0] != '\n' && line[0] != '#')
		{
			has_cam += !ft_strncmp(line, "C ", 2);
			has_amb += !ft_strncmp(line, "A ", 2);
			if (parse_line(line, scene, objects, lights))
			{
				close(fd);
				ft_printf("<%s>\n", line);
				return (EXIT_FAILURE);
			}
		}
		line = get_next_line(fd);
		if (line)
			line = ft_replace_all(line, "\t", " ");
	}
	end_parse(scene, objects, lights, fd);
	return (has_cam != 1 || has_amb != 1);
}
