#include "miniRT.h"
#include "libft.h"
#include "mlx.h"

#include <math.h>
#include <stdlib.h>
#include "float.h" // wtf norminette doesn't like angle brackets + float header
#include <stdarg.h>

// <START>: put this stuff in separate file
void	rotate_object(t_obj *obj, t_quat *quat)
{
	if (obj->type == TOK_SPHERE)
		return ;
	else if (obj->type == TOK_CYLINDER)
	{
		rotate_vec3(obj->cylinder.axis, quat);
		copy_vec3(obj->cylinder.base_top, obj->cylinder.center);
		add_vec3(obj->cylinder.base_top, obj->cylinder.axis);
		copy_vec3(obj->cylinder.base_bot, obj->cylinder.center);
		substract_vec3(obj->cylinder.base_bot, obj->cylinder.axis);
	}
	else if (obj->type == TOK_PLANE)
	{
		ft_printf("Not implemented\n");
	}
	else
		ft_printf("Object type %d doesn't exit!\n", obj->type);
}

// potential speedup possible
// 1 == sqrt(cos(degrees * PI / 360)**2 + length_squared_vec3(x, y, z));
// 1 == cos(degrees * PI / 360)**2 + length_squared_vec3(x, y, z);
// 1 - cos(degrees * PI / 360)**2 == length_squared_vec3(x_new, y_new, z_new);
// curr_length == length_squared_vec3(x_curr, y_curr, z_curr);
// x_new / x_curr = sin(degrees * PI / 360) / sqrt(curr_length);

//todo: maybe use cosf
// expects a unit vector!!!
// scales the axis!!! don't use it afterwards
// WATCH OUT
void	new_unit_quat(t_quat *quat, double degrees, t_vec3 *axis)
{
	t_vec3	unit_axis;

	copy_vec3(&unit_axis, axis);
	/* unit_vec3(&unit_axis); */
	sc_mult_vec3(&unit_axis, sin(degrees * PI / 360));
	quat_copy(quat, &(t_quat){.scalar = cos(degrees * PI / 360), .x = unit_axis.x, .y = unit_axis.y, .z = unit_axis.z});
}
// <END>

// TODO: add transformations

void	print_double_byte_by_byte(double value);

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
	va_start(ap, arg1);
	gc = va_arg(ap, t_gc *);
	va_end(ap);
	if (keycode == XK_Escape || keycode == 'q')
		destroy_hook(gc);
	else if (keycode == 'd')
	{
		gc->scene->lights[1]->point->x += 0.00000000001;
		render(gc, gc->scene);
	}
	else if (keycode == 'a')
	{
		gc->scene->lights[1]->point->x -= 0.00000000001;
		render(gc, gc->scene);
	}
	else if (keycode == 'w')
	{
		gc->scene->lights[1]->point->z -= 0.00000000001;
		render(gc, gc->scene);
	}
	else if (keycode == 's')
	{
		gc->scene->lights[1]->point->z += 0.00000000001;
		render(gc, gc->scene);
	}
	else if (keycode == 'l')
	{
		gc->scene->lights[1]->point->x += 0.00000001;
		render(gc, gc->scene);
	}
	else if (keycode == 'h')
	{
		gc->scene->lights[1]->point->x -= 0.00000001;
		render(gc, gc->scene);
	}
	else if (keycode == 'k')
	{
		gc->scene->lights[1]->point->z -= 0.00000001;
		render(gc, gc->scene);
	}
	else if (keycode == 'j')
	{
		gc->scene->lights[1]->point->z += 0.00000001;
		render(gc, gc->scene);
	}
	else if (keycode == 'p')
	{
		gc->scene->lights[1]->point->x += 0.01;
		render(gc, gc->scene);
	}
	else if (keycode == 'o')
	{
		gc->scene->lights[1]->point->x -= 0.01;
		render(gc, gc->scene);
	}
	else if (keycode == '2')
	{
		gc->scene->lights[1]->point->x += 1000000;
		render(gc, gc->scene);
	}
	else if (keycode == '1')
	{
		gc->scene->lights[1]->point->x -= 1000000;
		render(gc, gc->scene);
	}
	else if (keycode == ' ')
	{
		t_quat	quat;
		new_unit_quat(&quat, 30, &(t_vec3){.x = 0, .y = 1, .z = 0});
		/* new_unit_quat(&quat, 90, &(t_vec3){.x = 0, .y = 0, .z = 1}); */
		rotate_object(gc->scene->objects[0], &quat);
		print_cylinder(gc->scene->objects[0]);
		render(gc, gc->scene);
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
	scene->wwidth = 400;
	scene->wheight = 300;
}

# include <stdio.h>
void	print_light(t_light *light)
{
	printf("SOURCE OF LIGHT ");
	print_vec3(light->point);
	printf("RATIO: %f\n", light->ratio);
	print_color(&light->color);
}

int	main(int ac, char **av)
{
	t_gc	gc;
	t_scene	scene;

	init();
	setup_scene(&scene);
	setup_mlx(&gc, &scene);
	if (ac != 2 || read_rt_file(av[1], &scene))
	{
		ft_dprintf(2, "Error: wrong input format\n");
		finish(EXIT_FAILURE, &gc);
	} // put this is dedicated function or so
	gc.scene = &scene; // put this is dedicated function or so
	render(&gc, &scene);
	mlx_loop(gc.mlx);
	finish(0, &gc);
	return (EXIT_SUCCESS);
}
