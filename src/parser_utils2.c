/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 02:15:47 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/17 02:53:45 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

#include <fcntl.h>
#include <unistd.h>

int	parse_line(char *line, t_scene *scene, t_list *objects, t_list *lights)
{
	char	**arr;

	line[ft_strlen(line) - 1] = 0;
	arr = ft_split(line, ' ');
	if (!ft_strncmp(arr[0], "C", 2))
		return (parse_camera(line, scene));
	else if (!ft_strncmp(arr[0], "A", 2) || !ft_strncmp(arr[0], "L", 2))
		parse_lights(line, lights);
	else if (!ft_strncmp(arr[0], "sp", 3))
		return (parse_sphere(objects, arr));
	else if (!ft_strncmp(arr[0], "pl", 3))
		return (parse_plane(objects, arr));
	else if (!ft_strncmp(arr[0], "cy", 3))
		return (parse_cylinder(objects, arr));
	else
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	open_rt_file(char *file, int *fd)
{
	*fd = open(file, O_RDONLY);
	if (ft_strncmp(file + ft_strlen(file) - 3, ".rt", 4) || (*fd < 0))
	{
		ft_dprintf(STDERR_FILENO, "Error: wrong file or extension\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

bool	init_parse(t_list **objects, t_list **lights, char **line, int fd)
{
	*objects = lnew();
	*lights = lnew();
	*line = get_next_line(fd);
	if (*line)
		*line = ft_replace_all(*line, "\t", " ");
	return (false);
}

void	end_parse(t_scene *scene, t_list *objects, t_list *lights, int fd)
{
	scene->objects = ltoarr(objects);
	scene->nb_objs = (int)objects->len;
	scene->lights = ltoarr(lights);
	scene->nb_lights = (int)lights->len;
	close(fd);
}
