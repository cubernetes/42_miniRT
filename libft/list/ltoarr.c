/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ltoarr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nam-vu <nam-vu@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 08:43:48 by nam-vu            #+#    #+#             */
/*   Updated: 2024/09/03 08:43:48 by nam-vu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	**ltoarr(t_list *list)
{
	void	**arr;
	int		i;

	arr = ft_calloc(list->len + 1, sizeof(void *));
	liter(list);
	i = 0;
	while (lnext(list))
		arr[i++] = list->current->as_ptr;
	return (arr);
}
