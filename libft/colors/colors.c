/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 18:22:39 by nam-vu            #+#    #+#             */
/*   Updated: 2024/08/23 18:22:39 by nam-vu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	set_alpha(t_color *color, unsigned int value)
{
	if (value < 0 || value > 255)
	{
		ft_dprintf(2, "Error: color ratio should be in the range [0-255]\n");
		return ;
	}
	*color = ((*color) & 0x00FFFFFF) | (value << 24);
}

void	set_red(t_color *color, unsigned int value)
{
	if (value < 0 || value > 255)
	{
		ft_dprintf(2, "Error: color ratio should be in the range [0-255]\n");
		return ;
	}
	*color = ((*color) & 0xFF00FFFF) | (value << 16);
}

void	set_green(t_color *color, unsigned int value)
{
	if (value < 0 || value > 255)
	{
		ft_dprintf(2, "Error: color ratio should be in the range [0-255]\n");
		return ;
	}
	*color = ((*color) & 0xFFFF00FF) | (value << 8);
}

void	set_blue(t_color *color, unsigned int value)
{
	if (value < 0 || value > 255)
	{
		ft_dprintf(2, "Error: color ratio should be in the range [0-255]\n");
		return ;
	}
	*color = ((*color) & 0xFFFFFF00) | (value);
}

void	apply_light(t_color *color, double ratio)
{
	if (ratio < 0.0 || ratio > 1.0)
	{
		ft_dprintf(2, "Error: lighting ratio "
			"should be in the range [0.0, 1.0]\n");
		return ;
	}
	set_red(color, (unsigned int)(get_red(color) * ratio));
	set_green(color, (unsigned int)(get_green(color) * ratio));
	set_blue(color, (unsigned int)(get_blue(color) * ratio));
}
