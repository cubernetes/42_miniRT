#include "miniRT.h"
#include "mlx.h"

#include <stdlib.h>
#include "float.h" // wtf norminette doesn't like angle brackets + float header

// TODO: add transformations

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

int	destroy_hook(t_gc *gc)
{
	finish(0, gc);
	return (0);
}

#include <X11/X.h>

void	setup_hooks(t_gc *gc)
{
	mlx_hook(gc->win, DestroyNotify, NoEventMask, (int (*)(void*))destroy_hook, gc);
}

void	setup_mlx(t_gc *gc, t_scene *scene)
{
	gc->mlx = gc_add(mlx_init())->last->as_ptr; // TODO: check NULL
	gc->win = mlx_new_window(gc->mlx, scene->wwidth, scene->wheight, "miniRT"); // TODO: check NULL
	gc->img.img = mlx_new_image(gc->mlx, scene->wwidth, scene->wheight); // TODO: check NULL
	gc->img.addr = mlx_get_data_addr(
			gc->img.img, &gc->img.bpp,
			&gc->img.line_length,
			&gc->img.endian); // TODO: check NULL
	setup_hooks(gc);
}

void	setup_scene(t_scene *scene)
{
	scene->wwidth = 800;
	scene->wheight = 600;
}

int	main(void)
{
	t_gc	gc;
	t_scene	scene;
	t_obj	*objects;

	init();
	setup_scene(&scene);
	setup_mlx(&gc, &scene);
	if (parse_input(&objects, &scene))
	{
		ft_dprintf(2, "Error: wrong input format\n");
		return (EXIT_FAILURE);
	}
	render(&gc, &scene, objects);
	mlx_loop(gc.mlx);
	finish(0, &gc);
	return (EXIT_SUCCESS);
}
