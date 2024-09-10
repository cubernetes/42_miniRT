#include "miniRT.h"
#include "libft.h"
#include "mlx.h"

#include <math.h>
#include <stdlib.h>
#include "float.h" // wtf norminette doesn't like angle brackets + float header
#include <stdarg.h>

void	rotate_cylinder(t_cylinder *cylinder, t_quat *quat)
{
	double	ratio;

	ratio = cylinder->height
		/ (2 * length_vec3(cylinder->axis));
	sc_mult_vec3(cylinder->axis, ratio);
	rotate_vec3(cylinder->axis, quat);
	copy_vec3(cylinder->base_top, cylinder->center);
	add_vec3(cylinder->base_top, cylinder->axis);
	copy_vec3(cylinder->base_bot, cylinder->center);
	substract_vec3(cylinder->base_bot, cylinder->axis);
	unit_vec3(cylinder->axis);
}

void	rotate_plane(t_plane *plane, t_quat *quat)
{
	rotate_vec3(plane->norm, quat);
}

// <START>: put this stuff in separate file
void	rotate_object(t_obj *obj, t_quat *quat)
{
	if (obj->type == TOK_SPHERE)
		return ;
	else if (obj->type == TOK_CYLINDER)
		rotate_cylinder(&obj->cylinder, quat);
	else if (obj->type == TOK_PLANE)
		rotate_plane(&obj->plane, quat);
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

/* todo: either rotate view direction vector and then recalculate the other 2
 * OR: rotate each vector using 3 different quaternions? */
void	rotate_camera(t_camera *camera, t_direction direction, double degrees)
{
	t_quat	quat;
	t_vec3	left_vec;

	(void)direction;
	if (direction == DIR_LEFT)
		new_unit_quat(&quat, degrees, &(t_vec3){.x = 0, .y = 1, .z = 0});
	else
		new_unit_quat(&quat, -degrees, &(t_vec3){.x = 0, .y = 1, .z = 0});
	rotate_vec3(&camera->dir, &quat);
	copy_vec3(&left_vec, &camera->dir);
	cross_product_vec3(&left_vec, &(t_vec3){.x = 0, .y = -1, .z = 0});
	copy_vec3(&camera->right, &left_vec);
	sc_mult_vec3(&camera->right, -1);
	copy_vec3(&camera->up, &camera->dir);
	cross_product_vec3(&camera->up, &left_vec);
}

#include <stdint.h> /* needed for intptr_t */

/* cast to intptr_t and then int needed. only way to make this generic */
int	keydown_hook(void *arg1, ...)
{
	int		keycode;
	va_list	ap;
	t_gc	*gc;
	t_quat	quat;
	t_quat	quat2;

	new_unit_quat(&quat, 10, &(t_vec3){.x = 0, .y = 1, .z = 0});
	new_unit_quat(&quat2, -10, &(t_vec3){.x = 0, .y = 1, .z = 0});
	keycode = (int)(intptr_t)arg1;
	va_start(ap, arg1);
	gc = va_arg(ap, t_gc *);
	va_end(ap);
	if (keycode == XK_Escape || keycode == 'q')
		destroy_hook(gc);
	else if (keycode == 'w')
		translate_camera(gc->scene, DIR_FORWARD, 1);
	else if (keycode == 's')
		translate_camera(gc->scene, DIR_BACKWARD, 1);
	else if (keycode == 'a')
		translate_camera(gc->scene, DIR_LEFT, 1);
	else if (keycode == 'd')
		translate_camera(gc->scene, DIR_RIGHT, 1);
	else if (keycode == ' ')
		translate_camera(gc->scene, DIR_UP, 1);
	else if (keycode == XK_Shift_L)
		translate_camera(gc->scene, DIR_DOWN, 1);
	else if (keycode == 'h')
		rotate_camera(gc->scene->camera, DIR_LEFT, 15);
	else if (keycode == 'l')
		rotate_camera(gc->scene->camera, DIR_RIGHT, 15);
	else if (keycode == '9')
		rotate_object(gc->scene->objects[0], &quat);
	else if (keycode == '0')
		rotate_object(gc->scene->objects[0], &quat2);
	else if (keycode == '7')
		gc->scene->fov -= 1;
	else if (keycode == '8')
		gc->scene->fov += 1;
	else
	{
		ft_printf("Pressed '%c' (keycode: %d)\n", keycode, keycode);
		return (0);
	}
	render(gc, gc->scene);
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
	gc->win = mlx_new_window(gc->mlx, scene->window_width, scene->window_height, "miniRT"); // TODO: check NULL
	gc->img.img = mlx_new_image(gc->mlx, scene->window_width, scene->window_height); // TODO: check NULL
	gc->img.addr = mlx_get_data_addr(
			gc->img.img, &gc->img.bpp,
			&gc->img.line_length,
			&gc->img.endian); // TODO: check NULL
	setup_hooks(gc);
}

void	setup_scene(t_scene *scene)
{
	t_viewport	*viewport;
	t_camera	*camera;

	scene->window_width = 200;
	scene->window_height = 150;
	viewport = ft_malloc(sizeof(*viewport));
	viewport->width = scene->window_width;
	viewport->height = scene->window_height;
	camera = ft_malloc(sizeof(*camera));
	scene->viewport = viewport;
	scene->camera = camera;
}

void	print_light(t_light *light)
{
	ft_printf("SOURCE OF LIGHT ");
	print_vec3(light->point);
	ft_printf("RATIO: %f\n", light->ratio);
	print_color(&light->color);
}

#include <unistd.h>

int	main(int ac, char **av)
{
	t_gc			gc;
	t_scene			scene;
	const double	angle = 2;
	const double	amount = 2 * 200 * sin(angle * PI / 180);

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
	for (int i = 0; i < 24 * 2; ++i)
	{
		render(&gc, &scene);
		rotate_camera(scene.camera, DIR_RIGHT, angle);
		translate_camera(&scene, DIR_LEFT, amount);
		rotate_camera(scene.camera, DIR_RIGHT, angle);
		usleep(10000);
	}
	mlx_loop(gc.mlx);
	finish(0, &gc);
	return (EXIT_SUCCESS);
}
