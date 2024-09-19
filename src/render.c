/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 02:59:34 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/19 08:38:20 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include "mlx.h"

#include "float.h"
#include <math.h>
#include <stdio.h>

/* T = C + C_l * (vw / (2 * tan(theta / 2)) - Cr * vw / 2 + Cu * vh / 2 */
/* rebase_vec3(t_vec3 *this, t_vec3 **new_basis); */
void	init_viewport_params(t_scene *scene, t_vec3 *terminus)
{
	t_vec3	scaled_up;
	t_vec3	scaled_right;
	t_vec3	scaled_view;
	float	focal_distance;

	copy_vec3(&scene->viewport->down_step, &scene->camera->up);
	sc_mult_vec3(&scene->viewport->down_step, -1);
	copy_vec3(&scene->viewport->right_step, &scene->camera->right);
	copy_vec3(&scene->viewport->top_left, terminus);
	copy_vec3(&scaled_view, &scene->camera->dir);
	focal_distance = (float)scene->window_width / (2 * tanf(scene->fov * PI / 360));
	sc_mult_vec3(&scaled_view, focal_distance);
	copy_vec3(&scaled_right, &scene->camera->right);
	sc_mult_vec3(&scaled_right, (float)scene->window_width / 2.0f);
	copy_vec3(&scaled_up, &scene->camera->up);
	sc_mult_vec3(&scaled_up, (float)scene->window_height / 2.0f);
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

void	sample_frame(t_gc *gc, t_scene *scene, int resolution, int sample,
	int sample_size)
{
	int		i[4];
	t_ray	ray;
	t_vec3	terminus;
	t_hit	hit;
	t_vec3	vec[2];
	t_color	tmp_color;

	init_render(scene, &terminus);
	i[Y] = 0;
	copy_vec3(&vec[ROW_START_VEC], &scene->viewport->top_left);
	while (i[Y] + resolution - 1 < scene->window_height)
	{
		i[X] = 0;
		copy_vec3(&vec[PIXEL], &vec[ROW_START_VEC]);
		while (i[X] + resolution - 1 < scene->window_width)
		{
			if (((i[Y] * scene->window_height) / resolution + i[X] / resolution - sample)
				% sample_size == 0)
			{
				new_ray(&ray, &terminus, &vec[PIXEL]);
				tmp_color = 0;
				if (!cast_ray(&hit, &ray, scene))
				{
					tmp_color = hit.color;
					apply_light(&(hit.color), calculate_lighting(&hit, scene));
				}
				i[I] = -1;
				while (++(i[I]) < resolution)
				{
					i[J] = -1;
					while (++(i[J]) < resolution)
					{
						mlx_pixel_put_buf(&gc->img, i[X] + i[J], i[Y] + i[I],
							hit.color);
						mlx_pixel_put_buf(&gc->img3, i[X] + i[J], i[Y] + i[I],
							tmp_color);
					}
				}
			}
			i[J] = -1;
			while (++(i[J]) < resolution)
				add_vec3(&vec[PIXEL], &scene->viewport->right_step);
			i[X] += resolution;
		}
		i[I] = -1;
		while (++(i[I]) < resolution)
			add_vec3(&vec[ROW_START_VEC], &scene->viewport->down_step);
		i[Y] += resolution;
	}
	if (gc->antialiasing)
	{
		apply_pattern_antialiasing(gc, scene->window_width, scene->window_height, resolution);
		if (gc->interpolation)
			interpolation(gc, (unsigned int)scene->window_width, (unsigned int)scene->window_height, (unsigned int)resolution);
		render_cursor(gc);
		mlx_put_image_to_window(gc->mlx, gc->win, gc->img2.img, 0, 0);
	}
	else
	{
		render_cursor(gc);
		mlx_put_image_to_window(gc->mlx, gc->win, gc->img.img, 0, 0);
	}
	mlx_do_sync(gc->mlx);
}

bool	control_camera(t_gc *gc)
{
	bool	moved;
	const float	move_step = ((float)gc->scene->control.lctrl_pressed * 4.0f + 1.0f)
		* MOVE_STEP / gc->fps;

	moved = false;
	if (gc->scene->control.w_pressed)
		moved |= translate_camera(gc->scene->control.u_control_object.camera,
			DIR_FORWARD, move_step);
	if (gc->scene->control.s_pressed)
		moved |= translate_camera(gc->scene->control.u_control_object.camera,
			DIR_BACKWARD, move_step);
	if (gc->scene->control.a_pressed)
		moved |= translate_camera(gc->scene->control.u_control_object.camera,
			DIR_LEFT, move_step);
	if (gc->scene->control.d_pressed)
		moved |= translate_camera(gc->scene->control.u_control_object.camera,
			DIR_RIGHT, move_step);
	if (gc->scene->control.space_pressed)
		moved |= translate_camera(gc->scene->control.u_control_object.camera,
			DIR_UP, move_step);
	if (gc->scene->control.lshift_pressed)
		moved |= translate_camera(gc->scene->control.u_control_object.camera,
			DIR_DOWN, move_step);
	return (moved);
}

bool	translate_vec3(t_vec3 *vec, t_direction direction, float amount)
{
	bool	moved;

	moved = false;
	if (direction == DIR_FORWARD)
		vec->z -= amount;
	else if (direction == DIR_BACKWARD)
		vec->z += amount;
	else if (direction == DIR_RIGHT)
		vec->x += amount;
	else if (direction == DIR_LEFT)
		vec->x -= amount;
	else if (direction == DIR_UP)
		vec->y += amount;
	else if (direction == DIR_DOWN)
		vec->y -= amount;
	else
	{
		ft_printf("Direction does not exist\n");
		return (false);
	}
	return (true);
}

bool	translate_object(t_obj *obj, t_direction direction, float amount)
{
	bool	moved;

	moved = false;
	if (obj->type == TOK_SPHERE)
		moved |= translate_vec3(obj->sphere.center, direction, amount);
	else if (obj->type == TOK_PLANE)
		moved |= translate_vec3(obj->plane.point, direction, amount);
	else if (obj->type == TOK_CYLINDER)
	{
		moved |= translate_vec3(obj->cylinder.center, direction, amount);
		moved |= translate_vec3(obj->cylinder.base_top, direction, amount);
		moved |= translate_vec3(obj->cylinder.base_bot, direction, amount);
	}
	else
		ft_printf("Object does not exist\n");
	return (moved);
}

bool	control_object(t_gc *gc)
{
	bool	moved;
	const float	move_step = ((float)gc->scene->control.lctrl_pressed * 4.0f + 1.0f)
		* MOVE_STEP / gc->fps;

	moved = false;
	if (gc->scene->control.w_pressed)
		moved |= translate_object(gc->scene->control.u_control_object.object,
			DIR_FORWARD, move_step);
	if (gc->scene->control.s_pressed)
		moved |= translate_object(gc->scene->control.u_control_object.object,
			DIR_BACKWARD, move_step);
	if (gc->scene->control.a_pressed)
		moved |= translate_object(gc->scene->control.u_control_object.object,
			DIR_LEFT, move_step);
	if (gc->scene->control.d_pressed)
		moved |= translate_object(gc->scene->control.u_control_object.object,
			DIR_RIGHT, move_step);
	if (gc->scene->control.space_pressed)
		moved |= translate_object(gc->scene->control.u_control_object.object,
			DIR_UP, move_step);
	if (gc->scene->control.lshift_pressed)
		moved |= translate_object(gc->scene->control.u_control_object.object,
			DIR_DOWN, move_step);
	return (moved);
}

void	manage_controls(t_gc *gc)
{
	bool	moved;

	moved = true;
	if (gc->scene->control.f_pressed && !gc->scene->control.lctrl_pressed)
		gc->scene->fov = fminf(gc->scene->fov + 1, 150);
	else if (gc->scene->control.f_pressed && gc->scene->control.lctrl_pressed)
		gc->scene->fov = fmaxf(gc->scene->fov - 1, 30);
	else
		moved = false;
	if (gc->scene->control.e_control_type == CAMERA)
		moved |= control_camera(gc);
	else if (gc->scene->control.e_control_type == OBJECT)
		moved |= control_object(gc);
	if (moved)
	{
		gc->last_moved = ft_uptime_linux();
		gc->fully_rendered = false;
		gc->resolution = gc->ideal_resolution;
	}
}

void	calculate_fps(t_gc *gc)
{
	float	now;

	now = ft_uptime_linux();
	gc->frames_rendered++;
	gc->fps = (float)gc->frames_rendered / (now - gc->fps_start);
	if (ft_uptime_linux() - gc->fps_start > 1.0)
	{
		if (gc->fps < MIN_FPS)
			gc->ideal_resolution = ft_min(gc->ideal_resolution + 1, 64);
		else if (gc->fps > MIN_FPS + 10)
			gc->ideal_resolution = ft_max(gc->ideal_resolution - 1, 2);
		gc_start_context("FPS");
		gc_free("FPS");
		gc->fps_string = ft_itoa((int)gc->fps);
		gc_end_context();
		gc->fps_start = now;
		gc->frames_rendered = 1;
	}
	mlx_string_put(gc->mlx, gc->win, 50, 50, 0x00FFFFFF, gc->fps_string);
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
		sample_frame(gc, gc->scene, gc->resolution, gc->sample, gc->sample_size);
		gc->sample = (gc->sample + 1) % gc->sample_size;
		if (gc->resolution == 1 && gc->sample == 0)
			gc->fully_rendered = true;
		else
			gc->fully_rendered = false;
		calculate_fps(gc);
	}
	return (0);
}
