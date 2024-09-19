/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:40:05 by tosuman           #+#    #+#             */
/*   Updated: 2024/09/19 15:29:25 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include "mlx.h"

#include <stdlib.h>

void	init(t_gc *gc)
{
	(void)set_allocator(gc_malloc);
	(void)gc_set_context("DEFAULT");
	ft_memset(gc, 0, sizeof(t_gc));
	gc->sample = 0;
	gc->sample_size = SAMPLE_SIZE;
	gc->resolution = RESOLUTION;
	gc->ideal_resolution = gc->resolution;
	gc->last_moved = ft_uptime_linux() - MOVE_DELAY - 1;
	gc->fully_rendered = false;
	gc->frames_rendered = 1;
	gc->fps_start = ft_uptime_linux();
	gc->fps_string = "1";
	gc->fps = 10;
	gc->mouse_hidden = true;
	gc->antialiasing = 0;
	gc->interpolation = 0;
}

void	finish(int exit_status, t_gc *gc)
{
	if (gc->mlx)
	{
		mlx_do_key_autorepeaton(gc->mlx);
		if (gc->win)
		{
			if (gc->mouse_hidden)
				mlx_mouse_show(gc->mlx, gc->win);
			mlx_destroy_window(gc->mlx, gc->win);
		}
		if (gc->img.img)
			mlx_destroy_image(gc->mlx, gc->img.img);
		if (gc->img2.img)
			mlx_destroy_image(gc->mlx, gc->img2.img);
		if (gc->img3.img)
			mlx_destroy_image(gc->mlx, gc->img3.img);
		mlx_destroy_display(gc->mlx);
	}
	gc_free_all();
	exit(exit_status);
}

void	setup_scene(t_scene *scene)
{
	t_viewport	*viewport;
	t_camera	*camera;

	scene->window_width = WINDOW_WIDTH;
	scene->window_height = WINDOW_HEIGHT;
	viewport = ft_malloc(sizeof(*viewport));
	viewport->width = scene->window_width;
	viewport->height = scene->window_height;
	camera = ft_malloc(sizeof(*camera));
	scene->viewport = viewport;
	scene->camera = camera;
	ft_memset(&scene->control, 0, sizeof(t_control));
	scene->control.e_control_type = CAMERA;
	scene->control.u_control_object.camera = scene->camera;
}

void	parse_args(int ac, char **av, t_scene *scene, t_gc *gc)
{
	if (ac != 2)
	{
		ft_dprintf(2, "Usage: %s SCENEFILE\n", av[0]);
		gc_free_all();
		exit(EXIT_FAILURE);
	}
	else if (read_rt_file(av[1], scene))
	{
		ft_dprintf(2, "Error: scene file is invalid or corrupt\n");
		gc_free_all();
		exit(EXIT_FAILURE);
	}
	gc->scene = scene;
}

int	main(int ac, char **av)
{
	t_gc			gc;
	t_scene			scene;

	init(&gc);
	setup_scene(&scene);
	parse_args(ac, av, &scene, &gc);
	setup_mlx(&gc, &scene);
	mlx_mouse_move(gc.mlx, gc.win,
		gc.scene->window_width / 2, gc.scene->window_height / 2);
	mlx_mouse_hide(gc.mlx, gc.win);
	mlx_loop_hook(gc.mlx, render, (void *)&gc);
	mlx_loop(gc.mlx);
	finish(0, &gc);
	return (EXIT_SUCCESS);
}
	/* orbit(&gc, &scene); */
