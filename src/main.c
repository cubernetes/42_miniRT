#include "miniRT.h"
#include "libft.h"
#include "mlx.h"

#include <stdlib.h>
#include "float.h" // wtf norminette doesn't like angle brackets + float header

// todo: add quaternions
// todo: add transformations

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

int	main2(void)
{
	void			*mlx;
	void			*win;
	t_rt_img		img;
	t_sphere		sphere;
	t_vec3			center;
	const double	radius = 20;
	t_vec3			terminus;
	t_vec3			orientation;
	t_ray			ray;
	double			old_t;
	double			t;
	t_plane			plane;
	t_vec3			norm;
	t_vec3			point;
	t_plane			plane2;
	t_vec3			norm2;
	t_vec3			point2;

	new_vec3(&terminus, 0, 0, 0);
	new_vec3(&center, 0, 0, -25);
	new_sphere(&sphere, &center, radius);
	new_vec3(&norm, 0, 1, 0);
	new_vec3(&point, 0, -29, 0);
	new_plane(&plane, &point, &norm);
	new_vec3(&norm2, 0, 1, 2);
	new_vec3(&point2, 0, 1, 0);
	new_plane(&plane2, &point2, &norm2);
	mlx = mlx_init(); // TODO: check NULL
	img.img = mlx_new_image(mlx, 800, 600); // TODO: check NULL
	win = mlx_new_window(mlx, 800, 600, "miniRT"); // TODO: check NULL
	img.addr = mlx_get_data_addr(img.img, &img.bpp,
			&img.line_length, &img.endian); // TODO: check NULL
	for (int i = 0; i < 600; ++i)
	{
		for (int j = 0; j < 800; ++j)
		{
			new_vec3(&orientation, j / 4.0 - 800.0 / 8, i / 4.0 - 600.0 / 8, -10);
			new_ray(&ray, &terminus, &orientation);
			t = NO_ROOTS;
			mlx_pixel_put_buf(&img, j, 600 - i, 0x00000000);
			intersection_plane(&t, &plane, &ray);
			old_t = DBL_MAX;
			if ((t < old_t || old_t == NO_ROOTS) && t >= 0)
			{
				mlx_pixel_put_buf(&img, j, 600 - i, 0x00CE9D52);
				old_t = t;
			}
			intersection_plane(&t, &plane2, &ray);
			if ((t < old_t || old_t == NO_ROOTS) && t >= 0)
			{
				mlx_pixel_put_buf(&img, j, 600 - i, 0x00f68656);
				old_t = t;
			}
			intersection_sphere(&t, &sphere, &ray);
			if ((t < old_t || old_t == NO_ROOTS) && t >= 0)
			{
				mlx_pixel_put_buf(&img, j, 600 - i, 0x00da2b27);
				old_t = t;
			}
		}
	}
	ft_printf("DONE\n");
	mlx_put_image_to_window(mlx, win, img.img, 0, 0);
	mlx_loop(mlx);
	return (0);
}

int	destroy_hook(t_gc *gc)
{
	finish(0, gc);
	return (0);
}

#include <X11/X.h>

void	setup_hooks(t_gc *gc)
{
	mlx_hook(gc->win, DestroyNotify, NoEventMask, destroy_hook, gc);
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

void	test_render(t_gc *gc, t_scene *scene)
{
	int	x;
	int	y;

	y = -1;
	while (++y < scene->wheight)
	{
		x = -1;
		while (++x < scene->wwidth)
		{
			mlx_pixel_put_buf(&gc->img, x, scene->wheight - y, 0x00f68656);
		}
	}
	mlx_put_image_to_window(gc->mlx, gc->win, gc->img.img, 0, 0);
}

int	main(void)
{
	t_gc	gc;
	t_scene	scene;

	init();
	setup_scene(&scene);
	setup_mlx(&gc, &scene);
	test_render(&gc, &scene);
	mlx_loop(gc.mlx);
	finish(0, &gc);
}
