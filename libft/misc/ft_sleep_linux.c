/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sleep_linux.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 22:13:18 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/19 15:18:42 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void	ft_sleep_linux(int ms_delay)
{
	double	start;
	double	now;

	start = ft_uptime_linux();
	now = start;
	while ((int)((now - start) * 1000) < ms_delay)
		now = ft_uptime_linux();
}
