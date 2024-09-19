/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_static.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 15:27:49 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/19 15:28:00 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa_static(int n)
{
	static char		buf[20];
	size_t			n_len;
	unsigned int	n_cpy;
	int				is_neg;
	char			*str;

	is_neg = n < 0;
	n_len = 0;
	n_cpy = ft_abs(n) + !n;
	while (n_cpy * ++n_len != 0)
		n_cpy /= 10;
	--n_len;
	str = buf;
	str += (int)n_len + is_neg;
	n_cpy = ft_abs(n);
	*str-- = 0;
	while (n_len--)
	{
		*str-- = (char)(n_cpy % 10 + '0');
		n_cpy /= 10;
	}
	if (is_neg)
		*str-- = '-';
	return (++str);
}
