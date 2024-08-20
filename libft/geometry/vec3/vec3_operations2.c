#include "libft.h"

/* TODO: maybe there's some bitmagic to do this faster? */
void	add_vec3(t_vec3 *this, t_vec3 *vec)
{
	this->x += vec->x;
	this->y += vec->y;
	this->z += vec->z;
}

void	substract_vec3(t_vec3 *this, t_vec3 *vec)
{
	this->x -= vec->x;
	this->y -= vec->y;
	this->z -= vec->z;
}

double	dot_product_vec3(t_vec3 *this, t_vec3 *vec)
{
	return (this->x * vec->x
		+ this->y * vec->y
		+ this->z * vec->z);
}

void	cross_product_vec3(t_vec3 *this, t_vec3 *vec)
{
	new_vec3(this, this->y * vec->z - this->z * vec->y,
		this->z * vec->x - this->x * vec->z,
		this->x * vec->y - this->y * vec->x);
}

void	unit_vec3(t_vec3 *this)
{
	div_vec3(this, length_vec3(this));
}
