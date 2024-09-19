/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 02:59:34 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/19 12:52:54 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include "mlx.h"

#include <math.h>

/* T = C + C_l * (vw / (2 * tan(theta / 2)) - Cr * vw / 2 + Cu * vh / 2 */
/* rebase_vec3(t_vec3 *this, t_vec3 **new_basis); */
void	init_viewport_params(t_scene *scene, t_vec3 *terminus)
{
	t_vec3	scaled_up;
	t_vec3	scaled_right;
	t_vec3	scaled_view;
	double	focal_distance;

	copy_vec3(&scene->viewport->down_step, &scene->camera->up);
	sc_mult_vec3(&scene->viewport->down_step, -1);
	copy_vec3(&scene->viewport->right_step, &scene->camera->right);
	copy_vec3(&scene->viewport->top_left, terminus);
	copy_vec3(&scaled_view, &scene->camera->dir);
	focal_distance = scene->window_width / (2 * tan(scene->fov * PI / 360));
	sc_mult_vec3(&scaled_view, focal_distance);
	copy_vec3(&scaled_right, &scene->camera->right);
	sc_mult_vec3(&scaled_right, scene->window_width / 2.0);
	copy_vec3(&scaled_up, &scene->camera->up);
	sc_mult_vec3(&scaled_up, scene->window_height / 2.0);
	add_vec3(&scene->viewport->top_left, &scaled_view);
	substract_vec3(&scene->viewport->top_left, &scaled_right);
	add_vec3(&scene->viewport->top_left, &scaled_up);
	substract_vec3(&scene->viewport->top_left, &scene->camera->pos);
}

void	init_render(t_scene *scene, t_vec3 *terminus)
{
	copy_vec3(scene->lights[0]->point, &scene->camera->pos);
	copy_vec3(terminus, &scene->camera->pos);
	init_viewport_params(scene, terminus);
}

void	render_cursor(t_gc *gc)
{
	int			i;
	t_rt_img	*curr_img;

	if (gc->antialiasing)
		curr_img = &gc->img2;
	else
		curr_img = &gc->img;
	i = -1;
	while (++i < CURSOR_SIZE)
		mlx_pixel_put_buf(curr_img, gc->scene->window_width / 2
			- (CURSOR_SIZE / 2) + i, gc->scene->window_height / 2, CURSOR_CLR);
	i = -1;
	while (++i < CURSOR_SIZE)
		mlx_pixel_put_buf(curr_img, gc->scene->window_width / 2,
			gc->scene->window_height / 2 + (CURSOR_SIZE / 2) - i, CURSOR_CLR);
}

int	render(void *arg)
{
	t_gc		*gc;

	gc = arg;
	manage_controls(gc);
	if (ft_uptime_linux() - gc->last_moved > MOVE_DELAY
		&& gc->resolution >= 2 && gc->sample == 0)
	{
		gc->resolution--;
		gc->fully_rendered = false;
	}
	if (!gc->fully_rendered)
	{
		sample_frame(gc, gc->resolution, gc->sample, gc->sample_size);
		gc->sample = (gc->sample + 1) % gc->sample_size;
		if (gc->resolution == 1 && gc->sample == 0)
			gc->fully_rendered = true;
		else
			gc->fully_rendered = false;
		calculate_fps(gc);
	}
	return (0);
}
