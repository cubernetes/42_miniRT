/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sleep_linux.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 22:13:18 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/17 02:33:31 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <fcntl.h>
#include <unistd.h>

/* REALLY adapted to garbage collected libft */
void	ft_sleep_linux(int ms_delay)
{
	double	start;
	double	now;
	int		fd;
	char	*line;
	char	**parts;

	fd = open("/proc/uptime", O_RDONLY);
	line = get_next_line(fd);
	if (line == NULL)
		return ;
	parts = ft_split(line, ' ');
	if (parts == NULL || parts[0] == NULL)
		return ;
	(close(fd), start = ft_strtof(parts[0]), now = start);
	while ((int)((now - start) * 1000) < ms_delay)
	{
		gc_start_context("FT_SLEEP_LINUX");
		fd = open("/proc/uptime", O_RDONLY);
		line = get_next_line(fd);
		if (line == NULL)
			return ;
		close(fd);
		parts = ft_split(line, ' ');
		if (parts == NULL || parts[0] == NULL)
			return ;
		now = ft_strtof(parts[0]);
		gc_end_context();
		gc_free("FT_SLEEP_LINUX");
	}
}
