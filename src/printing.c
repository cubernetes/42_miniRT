/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:49:34 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/19 03:15:14 by nam-vu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

#include <stdio.h>

void	print_light(t_light *light)
{
	printf("SOURCE OF LIGHT ");
	print_vec3(light->point);
	printf("RATIO: %f\n", light->ratio);
	print_color(&light->color);
}
