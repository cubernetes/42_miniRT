#include "libft.h"

void	quat_mult(t_quat *quat_a, t_quat *quat_b)
{
	double	real_a;
	t_vec3	tmp1;
	t_vec3	tmp2;
	t_vec3	tmp3;

	real_a = quat_a->real;
	quat_a->real = real_a * quat_b->real
		- dot_product_vec3(&quat_a->axis, &quat_b->axis);
	copy_vec3(&tmp1, &quat_b->axis);
	sc_mult_vec3(&tmp1, real_a);
	copy_vec3(&tmp2, &quat_a->axis);
	sc_mult_vec3(&tmp2, quat_b->real);
	copy_vec3(&tmp3, &quat_a->axis);
	cross_product_vec3(&tmp3, &quat_b->axis);
	add_vec3(&tmp1, &tmp2);
	add_vec3(&tmp1, &tmp3);
	copy_vec3(&quat_a->axis, &tmp1);
}

int	quat_invert(t_quat *quat)
{
	double	tmp;

	quat_conj(quat);
	tmp = quat_len_squared(quat);
	if (tmp == 0)
	{
		ft_dprintf(2, "cannot invert 0-quaternion\n");
		return (1);
	}
	quat_sc_mult(quat, 1 / tmp);
	return (0);
}

int	quat_div(t_quat *quat_a, t_quat *quat_b)
{
	t_quat	tmp;

	quat_copy(&tmp, quat_b);
	if (quat_invert(&tmp))
	{
		ft_dprintf(2, "cannot divide by 0-quaternion\n");
		return (1);
	}
	quat_mult(quat_a, &tmp);
	return (0);
}

/* TODO: Use quat_conj instead -> speeeeeeeed */
void	rotate_vec3(t_vec3 *vec, t_quat *quat)
{
	t_quat	res;
	t_quat	vec_q;
	t_quat	inverted;

	quat_copy(&res, quat);
	vec_q.real = 0;
	copy_vec3(&vec_q.axis, vec);
	quat_copy(&inverted, quat);
	quat_invert(&inverted);
	quat_mult(&res, &vec_q);
	quat_mult(&res, &inverted);
	copy_vec3(vec, &res.axis);
}
