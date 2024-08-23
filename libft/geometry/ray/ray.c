#include "libft.h"

#include <stdio.h>

void	new_ray(t_ray *this, t_vec3 *terminus, t_vec3 *vec)
{
	this->terminus = terminus;
	this->vec = vec;
}

void	copy_ray(t_ray *this, t_ray *ray)
{
	copy_vec3(this->vec, ray->vec);
	copy_vec3(this->terminus, ray->terminus);
}

int	ray_at(t_ray *this, double t, t_vec3 *res)
{
	t_vec3	offset;

	if (t < 0)
	{
//		ft_dprintf(2, "Error: 't' cannot be negative (sometimes can tho)\n");
		res->x = 0;
		res->y = 0;
		res->z = 0;
		return (1);
	}
	copy_vec3(res, this->terminus);
	copy_vec3(&offset, this->vec);
	sc_mult_vec3(&offset, t);
	add_vec3(res, &offset);
	return (0);
}

void	print_ray(t_ray *this)
{
	printf("Ray:\n");
	ft_printf("\tTerminus: ");
	print_vec3(this->terminus);
	ft_printf("\tDirection vector: ");
	print_vec3(this->vec);
}
