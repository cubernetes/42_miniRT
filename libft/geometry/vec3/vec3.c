/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 20:34:09 by nam-vu            #+#    #+#             */
/*   Updated: 2024/08/20 23:15:18 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

void	new_vec3(t_vec3 *this, double x, double y, double z)
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
