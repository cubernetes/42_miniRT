#include "miniRT.h"
#include "libft.h"

#include <math.h>

static void	translate_camera_u_d(t_scene *scene, t_direction direction)
{
	if (direction == DIR_UP)
		scene->camera_pos.y += 1;
	else
		scene->camera_pos.y -= 1;
}

static void	translate_camera_f_b(t_scene *scene, t_direction direction)
{
	t_vec3	direction_vector;

	if (scene->camera_dir.x == 0 && scene->camera_dir.z == 0)
	{
		ft_printf("Looking straight up, cannot move forward/backward\n");
		return ;
	}
	copy_vec3(&direction_vector, &scene->camera_dir);
	direction_vector.y = 0;
	unit_vec3(&direction_vector);
	if (direction == DIR_FORWARD)
		add_vec3(&scene->camera_pos, &direction_vector);
	else
		substract_vec3(&scene->camera_pos, &direction_vector);
}

static void	translate_camera_l_r(t_scene *scene, t_direction direction)
{
	t_vec3			direction_vector;
	static t_vec3	upwards_vector = {.x = 0, .y = 1, .z = 0};

	if (scene->camera_dir.x == 0 && scene->camera_dir.z == 0)
	{
		ft_printf("Looking straight up, cannot move left/right\n");
		return ;
	}
	copy_vec3(&direction_vector, &scene->camera_dir);
	direction_vector.y = 0;
	unit_vec3(&direction_vector);
	cross_product_vec3(&direction_vector, &upwards_vector);
	if (direction == DIR_LEFT)
		substract_vec3(&scene->camera_pos, &direction_vector);
	else
		add_vec3(&scene->camera_pos, &direction_vector);
}

void	translate_camera(t_scene *scene, t_direction direction)
{
	if (direction == DIR_UP || direction == DIR_DOWN)
		translate_camera_u_d(scene, direction);
	else if (direction == DIR_FORWARD || direction == DIR_BACKWARD)
		translate_camera_f_b(scene, direction);
	else
		translate_camera_l_r(scene, direction);
	print_vec3(&scene->camera_dir);
	print_vec3(&scene->camera_pos);
}
