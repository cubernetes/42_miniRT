/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors_getters.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 18:57:30 by nam-vu            #+#    #+#             */
/*   Updated: 2024/08/23 18:57:30 by nam-vu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	get_alpha(t_color *color)
{
	return ((*color >> 24) & 0xFF);
}

int	get_red(t_color *color)
{
	return ((*color >> 16) & 0xFF);
}

int	get_green(t_color *color)
{
	return ((*color >> 8) & 0xFF);
}

int	get_blue(t_color *color)
{
	return (*color & 0xFF);
}
