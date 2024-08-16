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
