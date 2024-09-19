/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:04:11 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/19 15:33:58 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "mlx.h"

void	calculate_fps(t_gc *gc)
{
	double	now;

	now = ft_uptime_linux();
	gc->frames_rendered++;
	gc->fps = (double)gc->frames_rendered / (now - gc->fps_start);
	if (ft_uptime_linux() - gc->fps_start > 1.0)
	{
		if (gc->fps < MIN_FPS)
			gc->ideal_resolution = ft_min(gc->ideal_resolution + 1, 64);
		else if (gc->fps > MIN_FPS + 10)
			gc->ideal_resolution = ft_max(gc->ideal_resolution - 1, 2);
		gc->fps_string = ft_itoa_static((int)gc->fps);
		gc->fps_start = now;
		gc->frames_rendered = 1;
	}
	if (gc->scene->window_width >= 100 && gc->scene->window_height >= 100)
		mlx_string_put(gc->mlx, gc->win, 50, 50, 0x00FFFFFF, gc->fps_string);
}
