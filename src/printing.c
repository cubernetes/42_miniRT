/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:49:34 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/10 20:49:42 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

void	print_light(t_light *light)
{
	ft_printf("SOURCE OF LIGHT ");
	print_vec3(light->point);
	ft_printf("RATIO: %f\n", light->ratio);
	print_color(&light->color);
}
