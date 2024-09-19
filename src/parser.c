/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 02:14:14 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/19 12:59:00 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

//TODO: removed null protection from parse_vec3 and parse_color
// check that its actually not needed

int	parse_vec3(char str[static 1], t_vec3 *vec, int flag)
{
	int		i;
	char	*ptr;
	char	*next_ptr;

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
	if ((flag == NORM_VEC && (fabs(vec->x) > 1.0 || fabs(vec->y) > 1.0
				|| fabs(vec->z) > 1.0)) && length_squared_vec3(vec) > 0.0)
		return (EXIT_FAILURE);
	if (flag == NORM_VEC)
		unit_vec3(vec);
	return (EXIT_SUCCESS);
}

int	parse_color(char str[static 1], t_color *color)
{
	int		i;
	char	*ptr;
	char	*next_ptr;
	double	temp[3];

	*color = 0;
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
	int			has_flag[2];

	if (open_rt_file(file, &fd))
		return (EXIT_FAILURE);
	has_flag[0] = init_parse(&objects, &lights, &line, fd);
	has_flag[1] = 0;
	while (line)
	{
		if (line[0] != '\n' && line[0] != '#')
		{
			has_flag[0] += !ft_strncmp(line, "C ", 2);
			has_flag[1] += !ft_strncmp(line, "A ", 2);
			if (parse_line(line, scene, objects, lights))
				return (close(fd), ft_printf("<%s>\n", line), EXIT_FAILURE);
		}
		line = get_next_line(fd);
		if (line)
			line = ft_replace_all(line, "\t", " ");
	}
	end_parse(scene, objects, lights, fd);
	return (has_flag[0] != 1 || has_flag[1] != 1);
}
