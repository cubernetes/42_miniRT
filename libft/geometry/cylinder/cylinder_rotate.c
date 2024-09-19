/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_rotate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 22:16:34 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/18 06:40:01 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	rotate_cylinder(t_cylinder *cylinder, t_quat *quat)
{
	double	ratio;

	ratio = cylinder->height
		/ (2 * length_vec3(cylinder->axis));
	sc_mult_vec3(cylinder->axis, ratio);
	rotate_vec3(cylinder->axis, quat);
	copy_vec3(cylinder->base_top, cylinder->center);
	add_vec3(cylinder->base_top, cylinder->axis);
	copy_vec3(cylinder->base_bot, cylinder->center);
	substract_vec3(cylinder->base_bot, cylinder->axis);
	unit_vec3(cylinder->axis);
}
