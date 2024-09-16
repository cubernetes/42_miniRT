/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_uptime_linux.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 22:13:18 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/16 22:56:33 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <fcntl.h>
#include <math.h>

// todo: valgrind the shit out of this or sth.
double	ft_uptime_linux(void)
{
	double	uptime;
	int		fd;
	char	*line;
	char	**parts;

	gc_start_context("FT_UPTIME_LINUX");
	fd = open("/proc/uptime", O_RDONLY);
	line = get_next_line(fd);
	close(fd);
	if (line == NULL)
		return (gc_end_context(), gc_free("FT_UPTIME_LINUX"), -1);
	parts = ft_split(line, ' ');
	if (parts == NULL || parts[0] == NULL)
		return (gc_end_context(), gc_free("FT_UPTIME_LINUX"), -1);
	uptime = ft_strtof(parts[0]);
	gc_end_context();
	gc_free("FT_UPTIME_LINUX");
	if (isnan(uptime))
		return (-1);
	return (uptime);
}
