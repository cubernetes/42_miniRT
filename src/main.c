/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:40:05 by tosuman           #+#    #+#             */
/*   Updated: 2024/09/10 20:56:46 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _XOPEN_SOURCE 600 /* only needed for <unistd.h> and usleep */
#include <unistd.h> /* usleep(3) */

#include "miniRT.h"
#include "libft.h"
#include "mlx.h"

void	init(void)
{
	(void)set_allocator(gc_malloc);
	(void)gc_set_context("DEFAULT");
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

	scene->window_width = 1920;
	scene->window_height = 1080;
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
		finish(EXIT_FAILURE, gc);
	}
	else if (read_rt_file(av[1], scene))
	{
		ft_dprintf(2, "Error: scene file is corrupt");
		finish(EXIT_FAILURE, gc);
	}
	gc->scene = scene;
}

/* #include <math.h> */

int	main(int ac, char **av)
{
	t_gc			gc;
	t_scene			scene;

	init();
	setup_scene(&scene);
	setup_mlx(&gc, &scene);
	parse_args(ac, av, &scene, &gc);
	render(&gc, &scene);
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
	mlx_loop(gc.mlx);
	finish(0, &gc);
	return (EXIT_SUCCESS);
}
