/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_uptime_linux.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 22:13:18 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/19 15:18:28 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <fcntl.h>
#include <math.h>
#include <unistd.h>

double	ft_uptime_linux(void)
{
	char	buf[100];
	ssize_t	bytes_read;
	int		fd;

	fd = open("/proc/uptime", O_RDONLY);
	if (fd < 0)
		return (-1);
	bytes_read = read(fd, buf, 99);
	close(fd);
	buf[bytes_read] = 0;
	return (ft_atof(buf));
}
