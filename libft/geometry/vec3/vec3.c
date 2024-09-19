/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 21:55:43 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/19 08:28:34 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdio.h>
#include <math.h>

void	new_vec3(t_vec3 *this, float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void	copy_vec3(t_vec3 *this, t_vec3 *old)
{
	this->x = old->x;
	this->y = old->y;
	this->z = old->z;
}

void	print_vec3(t_vec3 *this)
{
	printf("Vec3: [%f, %f, %f]\n",
		this->x, this->y, this->z);
}

/*
 implementation of Gaussian elimination
*/
//void	rebase_vec3(t_vec3 *this, t_vec3 **new_basis)
//{
//	t_vec3	eq[3];
//	t_vec3	b;
//	float	coefficient;
//	t_vec3	new_substraction;
//	t_vec3	temp;
//	float	t;
//	float	rhs;
//
//	copy_vec3(&b, this);
//	for (int i = 0; i < 3; i++)
//	{
//		copy_vec3(&eq[i], new_basis[i]);
//	}
//	copy_vec3(&b, this);
//	for (int i = 0; i < 3; i++)
//	{
//		for (int j = i; j < 3; j++)
//		{
//			if (eq[j].e[i] != 0)
//			{
//				copy_vec3(&temp, &eq[i]);
//				copy_vec3(&eq[i], &eq[j]);
//				copy_vec3(&eq[j], &temp);
//				t = b.e[i];
//				b.e[i] = b.e[j];
//				b.e[j] = t;
//				break ;
//			}
//			if (j == 2)
//			{
//				ft_dprintf(2, "Error: new basis is not linear independent\n");
//				return ;
//			}
//		}
//		for (int j = i + 1; j < 3; j++)
//		{
//			coefficient = eq[j].e[i] / eq[i].e[i];
//			copy_vec3(&new_substraction, &eq[i]);
//			sc_mult_vec3(&new_substraction, coefficient);
//			substract_vec3(&eq[j], &new_substraction);
//			b.e[j] -= b.e[i] * coefficient;
//		}
//	}
//	for (int i = 2; i >= 0; i--) // calculate the roots
//	{
//		rhs = b.e[i];
//		for (int j = i + 1; j < 3; j++)
//		{
//			rhs -= eq[i].e[j] * this->e[j];
//		}
//		this->e[i] = rhs / eq[i].e[i];
//	}
//}

float	cos_vec3(t_vec3 *a, t_vec3 *b)
{
	return (dot_product_vec3(a, b)
		/ sqrtf(length_squared_vec3(a) * length_squared_vec3(b)));
}
