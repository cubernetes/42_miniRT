#include "miniRT.h"
#include "mlx.h"

#include <stdlib.h>
#include "float.h" // wtf norminette doesn't like angle brackets + float header
#include <stdarg.h>

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

#include <X11/X.h>
#include <X11/keysym.h>

int	destroy_hook(void *arg1, ...)
{
	t_gc	*gc;

	gc = arg1;
	finish(0, gc);
	return (0);
}

#include <stdint.h>

/* cast to intptr_t and then int needed. only way to make this generic */
int	keydown_hook(void *arg1, ...)
{
	int		keycode;
	va_list	ap;
	t_gc	*gc;

	keycode = (int)(intptr_t)arg1;
	if (keycode == XK_Escape || keycode == 'q')
	{
		va_start(ap, arg1);
		gc = va_arg(ap, t_gc *);
		va_end(ap);
		destroy_hook(gc);
	}
	else
		ft_printf("Pressed '%c' (keycode: %d)\n", keycode, keycode);
	return (0);
}
/* TODO: Remove 'pressed' debug output */

void	setup_hooks(t_gc *gc)
{
	mlx_hook(gc->win, DestroyNotify, NoEventMask, (t_hook)destroy_hook, gc);
	mlx_hook(gc->win, KeyPress, KeyPressMask, (t_hook)keydown_hook, gc);
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
