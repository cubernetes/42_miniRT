/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 20:34:09 by nam-vu            #+#    #+#             */
/*   Updated: 2024/08/16 20:34:09 by nam-vu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

void	new_vec3(t_vec3 *this, double x, double y, double z)
{
	this->e[0] = x;
	this->e[1] = y;
	this->e[2] = z;
}

void	copy_vec3(t_vec3 *this, t_vec3 *old)
{
	this->e[0] = old->e[0];
	this->e[1] = old->e[1];
	this->e[2] = old->e[2];
}

void	print_vec3(t_vec3 *this)
{
	printf("Vec3: [%f, %f, %f]\n",
		this->e[0], this->e[1], this->e[2]);
}

void	rebase_vec3(t_vec3 *this, t_vec3 **new_basis)
{
/*
 implementation of Gaussian elimination
*/
	t_vec3	eq[3];
	t_vec3	b;
	double	coefficient;
	t_vec3	new_substraction;
	t_vec3	temp;
	double	t;
	double	rhs;

	copy_vec3(&b, this);
	for (int i = 0; i < 3; i++)
	{
		copy_vec3(&eq[i], new_basis[i]);
	}
	copy_vec3(&b, this);
	for (int i = 0; i < 3; i++)
	{
		for (int j = i; j < 3; j++)//find a vector with non-zero element on the current position and swap it
		{
			if (eq[j].e[i] != 0)
			{
				copy_vec3(&temp, &eq[i]);
				copy_vec3(&eq[i], &eq[j]);
				copy_vec3(&eq[j], &temp);
				t = b.e[i];
				b.e[i] = b.e[j];
				b.e[j] = t;
				break ;
			}
			if (j == 2)//if there is none, means that basis wasnt linear independent
			{
				ft_dprintf(2, "Error: new basis is not linear independent\n");
				return ;
			}
		}
		for (int j = i + 1; j < 3; j++)//substract current vector from next vectors
		{
			coefficient = eq[j].e[i] / eq[i].e[i];
			copy_vec3(&new_substraction, &eq[i]);
			sc_mult_vec3(&new_substraction, coefficient);
			substract_vec3(&eq[j], &new_substraction);
			b.e[j] -= b.e[i] * coefficient;
		}
	}
	for (int i = 2; i >= 0; i--)//calculate the roots
	{
		rhs = b.e[i];
		for (int j = i + 1; j < 3; j++)
		{
			rhs -= eq[i].e[j] * this->e[j];
		}
		this->e[i] = rhs / eq[i].e[i];
	}
}
