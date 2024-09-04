/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fast_sqrtf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 21:19:28 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/04 16:15:50 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

float	fast_sqrtf(float x)
{
	union u_fast_sqrtf_vars	u_z;

	u_z.f = x;
	u_z.i = 0x5f3759df - (u_z.i >> 1);
	u_z.f *= (1.5f - (x * 0.5f * u_z.f * u_z.f));
	u_z.i = 0x7EEEEEEE - u_z.i;
	return (u_z.f);
}
