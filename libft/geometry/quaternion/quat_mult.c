/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quat_mult.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 21:52:38 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/19 10:31:44 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	quat_mult(t_quat *quat_a, t_quat *quat_b)
{
	double	scalar_a;
	t_vec3	tmp1;
	t_vec3	tmp2;
	t_vec3	tmp3;

	scalar_a = quat_a->scalar;
	quat_a->scalar = scalar_a * quat_b->scalar
		- dot_product_vec3(&quat_a->vector, &quat_b->vector);
	copy_vec3(&tmp1, &quat_b->vector);
	sc_mult_vec3(&tmp1, scalar_a);
	copy_vec3(&tmp2, &quat_a->vector);
	sc_mult_vec3(&tmp2, quat_b->scalar);
	copy_vec3(&tmp3, &quat_a->vector);
	cross_product_vec3(&tmp3, &quat_b->vector);
	add_vec3(&tmp1, &tmp2);
	add_vec3(&tmp1, &tmp3);
	copy_vec3(&quat_a->vector, &tmp1);
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

/* TODO unimportant: use quat_conj instead (might need to be normalized)
 * -> speeeeeeeed */
void	rotate_vec3(t_vec3 *vec, t_quat *quat)
{
	t_quat	res;
	t_quat	vec_q;
	t_quat	inverted;

	quat_copy(&res, quat);
	vec_q.scalar = 0;
	copy_vec3(&vec_q.vector, vec);
	quat_copy(&inverted, quat);
	quat_invert(&inverted);
	quat_mult(&res, &vec_q);
	quat_mult(&res, &inverted);
	copy_vec3(vec, &res.vector);
}

//todo unimportant: use the reciprocal trick to make this maybe faster
void	quat_unit(t_quat *quat)
{
	quat_sc_mult(quat, 1 / quat_len(quat));
}
