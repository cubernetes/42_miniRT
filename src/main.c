/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:40:05 by tosuman           #+#    #+#             */
/*   Updated: 2024/09/16 23:02:56 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _XOPEN_SOURCE 600 /* only needed for <unistd.h> and usleep */

#include "miniRT.h"
#include "libft.h"
#include "mlx.h"

#include <stdlib.h>
#include <unistd.h> /* usleep(3); todo: remove */

void	init(t_gc *gc)
{
	(void)set_allocator(gc_malloc);
	(void)gc_set_context("DEFAULT");
	gc->sample = 0;
	gc->sample_size = SAMPLE_SIZE;
	gc->resolution = RESOLUTION;
	gc->last_moved = ft_uptime_linux() - MOVE_DELAY - 1;
	gc->fully_rendered = false;
}

void	finish(int exit_status, t_gc *gc)
{
	mlx_destroy_image(gc->mlx, gc->img.img);
	mlx_destroy_window(gc->mlx, gc->win);
	mlx_destroy_display(gc->mlx);
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

/* #include <math.h> */

int	main(int ac, char **av)
{
	t_gc			gc;
	t_scene			scene;

	init(&gc);
	setup_scene(&scene);
	parse_args(ac, av, &scene, &gc);
	setup_mlx(&gc, &scene);
	mlx_mouse_move(gc.mlx, gc.win, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	mlx_mouse_hide(gc.mlx, gc.win);
	gc.scene->camera->prev_x = WINDOW_WIDTH / 2;
	gc.scene->camera->prev_y = WINDOW_HEIGHT / 2;
	/* render(&gc, &scene, gc->sample_size); */
	/* const double	angle = 3; */
	/* const double	amount = 2 * 200 * sin(angle * 3 / 180); */
	/* for (int i = 0; i < 120 * 1; ++i) */
	/* { */
		/* render(&gc, &scene); */
		/* rotate_camera(scene.camera, DIR_RIGHT, angle); */
		/* translate_camera(&scene, DIR_LEFT, amount); */
		/* rotate_camera(scene.camera, DIR_RIGHT, angle); */
		/* usleep(10000); */
	/* } */
	mlx_loop_hook(gc.mlx, render, (void *)&gc);
	mlx_loop(gc.mlx);
	finish(0, &gc);
	return (EXIT_SUCCESS);
}
