/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hoarding_disorder.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 00:04:58 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/19 12:59:44 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

#include <math.h>
#include <stdio.h>

void	assert_norm(t_hit *hit, int index)
{
	if ((fabs(hit->point.x + 1.537377) <= EPSILON)
		&& (fabs(hit->point.y - 17.103315) <= EPSILON)
		&& (fabs(hit->point.z + 30.747532) <= EPSILON))
	{
		if (hit->object->type != TOK_SPHERE)
			exit(printf("NOT A SPHERE\n"));
		if (fabs(length_squared_vec3(&hit->norm) - hit->object->sphere.radius
				* hit->object->sphere.radius) > EPSILON)
		{
			printf("NORM IS WRONG ");
			if (index == 1)
				printf("IN CAST_RAY ");
			else
				printf("IN RENDER ");
			print_vec3(&hit->norm);
			printf("LENGTH SQUARED = %f\n", length_squared_vec3(&hit->norm));
			printf("RADIUS SQUARED = %f\n",
				hit->object->sphere.radius * hit->object->sphere.radius);
			printf("ASSERT %i\n", index);
		}
	}
}

void	print_double_byte_by_byte(double value)
{
	unsigned char	*byte_ptr;
	int				i;

	byte_ptr = (unsigned char *)&value;
	i = -1;
	while ((unsigned int)(++i) < sizeof(double))
		printf("%02X ", byte_ptr[i]);
	printf(" : %f\n", value);
}
