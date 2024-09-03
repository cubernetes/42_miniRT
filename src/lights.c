#include "libft.h"
#include "miniRT.h"
#include <stdio.h>
#include <math.h>

void	apply_light(t_color *color, t_color ratio)
{
	set_red(color, (unsigned int)(get_red(color) * get_red(&ratio) / 256));
	set_green(color, (unsigned int)(get_green(color) * get_green(&ratio) / 256));
	set_blue(color, (unsigned int)(get_blue(color) * get_blue(&ratio) / 256));
}

#include <stdlib.h>

void	combine_light(t_color *color, t_light *light, double diffusion_factor)
{
	const unsigned int	new_red = (unsigned int)(get_red(color) + get_red(&light->color) * light->ratio * diffusion_factor);
	const unsigned int	new_green = (unsigned int)(get_green(color) + get_green(&light->color) * light->ratio * diffusion_factor);
	const unsigned int	new_blue = (unsigned int)(get_blue(color) + get_blue(&light->color) * light->ratio * diffusion_factor);

	if (new_red > 255)
		set_red(color, 255);
	else
		set_red(color, new_red);
	if (new_green > 255)
		set_green(color, 255);
	else
		set_green(color, new_green);
	if (new_blue > 255)
		set_blue(color, 255);
	else
		set_blue(color, new_blue);
}

void print_double_byte_by_byte(double value) {
	// Cast the double to a pointer to a char (byte)
	unsigned char *bytePtr = (unsigned char*)&value;

	// Print each byte in hexadecimal format
	for (int i = 0; (unsigned int)i < sizeof(double); i++) {
		printf("%02X ", bytePtr[i]);
	}
	printf(" : %f\n", value);
}

/* expensive function, is run:
 *     wwidth * wheight * nb_lights * nb_objs times (roughly 3 mil.)
 */
//todo: remove diffussion from the ambient lighting
t_color	calculate_lighting(t_hit *hit, t_scene *scene)
{
	t_ray	ray;
	t_vec3	orientation;
	int		i;
	t_hit	shadow_hit;
	t_vec3	first;
	t_color	res;
	int		same_half_space;

	res = 0;
	i = -1;
	while (++i < scene->nb_lights)
	{
		shadow_hit.t = NO_ROOTS;
		copy_vec3(&orientation, &hit->point);
		substract_vec3(&orientation, scene->lights[i]->point);
		unit_vec3(&orientation);
		new_ray(&ray, scene->lights[i]->point, &orientation);
		same_half_space = dot_product_vec3(&hit->norm, ray.vec) * dot_product_vec3(&hit->norm, &hit->ray_dir) > 0.0;
		if (cast_ray(&shadow_hit, &ray, scene) && same_half_space)
				combine_light(&res, scene->lights[i], fabs(cos_vec3(&hit->norm, ray.vec)));
		else
		{
			copy_vec3(&first, &(shadow_hit.point));
			substract_vec3(&first, &hit->point);
			if (length_squared_vec3(&first) <= 0.0000001  && same_half_space)
					combine_light(&res, scene->lights[i], fabs (cos_vec3(&hit->norm, ray.vec)));
		}
//		else
//			print_double_byte_by_byte(t);
	}
	return (res);
}
