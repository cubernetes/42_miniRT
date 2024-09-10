/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:51:20 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/10 20:51:30 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

// general TODO: add transformations
void	rotate_object(t_obj *obj, t_quat *quat)
{
	if (obj->type == TOK_SPHERE)
		return ;
	else if (obj->type == TOK_CYLINDER)
		rotate_cylinder(&obj->cylinder, quat);
	else if (obj->type == TOK_PLANE)
		rotate_plane(&obj->plane, quat);
	else
		ft_printf("Object type %d doesn't exit!\n", obj->type);
}
