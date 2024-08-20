#include "libft.h"
#include <math.h>

void	reverse_vec3(t_vec3 *this)
{
	this->x *= -1;
	this->y *= -1;
	this->z *= -1;
}

void	sc_mult_vec3(t_vec3 *this, double k)
{
	this->x *= k;
	this->y *= k;
	this->z *= k;
}

void	div_vec3(t_vec3 *this, double k)
{
	if (k == 0)
	{
		ft_dprintf(2, "division of vec3 by 0\n");
		return ;
	}
	this->x /= k;
	this->y /= k;
	this->z /= k;
}

double	length_vec3(t_vec3 *this)
{
	return (sqrt(this->x * this->x
			+ this->y * this->y
			+ this->z * this->z));
}

double	length_squared_vec3(t_vec3 *this)
{
	return (this->x * this->x
		+ this->y * this->y
		+ this->z * this->z);
}
