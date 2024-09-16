/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:42:43 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/16 23:53:53 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static int	mlx_int_resizable_win(t_xvar *xvar, Window win, int w, int h)
{
	XSizeHints			hints;
	long				toto;
	XWindowAttributes	w_attr;

	XGetWMNormalHints(xvar->display, win, &hints, &toto);
	XGetWindowAttributes(xvar->display, xvar->root, &w_attr);
	hints.width = w;
	hints.height = h;
	hints.min_width = 10;
	hints.min_height = 10;
	hints.max_width = w_attr.width;
	hints.max_height = w_attr.height;
	hints.flags = USPosition | USSize | PMinSize | PMaxSize;
	hints.win_gravity = 1;
	hints.x = 0;
	hints.y = 0;
	hints.width_inc = 1;
	hints.height_inc = 1;
	hints.base_width = 100;
	hints.base_height = 100;
	hints.max_aspect.x = 2;
	hints.max_aspect.y = 1;
	hints.min_aspect.x = 1;
	hints.min_aspect.y = 2;
	return (XSetWMNormalHints(xvar->display, win, &hints), 0);
}

void	*mlx_new_resizable_window(t_xvar *xvar, int size_x, int size_y,
	char *title)
{
	t_win_list				*new_win;
	XSetWindowAttributes	xswa;
	XGCValues				xgcv;

	new_win = malloc((xswa.background_pixel = 0,
				xswa.border_pixel = (unsigned long)(-1), xswa.colormap
				= xvar->cmap, xswa.event_mask = 0xffffff, sizeof(*new_win)));
	if (!new_win)
		return (NULL);
	new_win->window = XCreateWindow(xvar->display, xvar->root, 0, 0,
			(unsigned int)size_x, (unsigned int)size_y, 0, CopyFromParent,
			InputOutput, xvar->visual,
			CWEventMask | CWBackPixel | CWBorderPixel | CWColormap, &xswa);
	mlx_int_resizable_win(xvar, new_win->window, size_x, size_y);
	XStoreName(xvar->display, new_win->window, title);
	XSetWMProtocols(xvar->display, new_win->window, &(xvar->wm_delete_window),
		(xgcv.foreground = (unsigned long)(-1), 1));
	(free(NULL), xgcv.function = GXcopy, xgcv.plane_mask = AllPlanes);
	new_win->gc = XCreateGC(xvar->display, new_win->window, GCFunction
			| GCPlaneMask | GCForeground, &xgcv);
	new_win->next = xvar->win_list;
	xvar->win_list = new_win;
	ft_bzero(&(new_win->hooks), sizeof(new_win->hooks));
	XMapRaised(xvar->display, new_win->window);
	return (mlx_int_wait_first_expose(xvar, new_win->window), new_win);
}

int	mlx_get_window_dim(void *mlx_ptr, void *win_ptr, int *width, int *height)
{
	XWindowAttributes	w_attr;
	int					status;

	status = XGetWindowAttributes(((t_xvar *)mlx_ptr)->display,
			((t_win_list *)win_ptr)->window, &w_attr);
	*width = w_attr.width;
	*height = w_attr.height;
	return (status);
}

void	mlx_pixel_put_buf(t_rt_img *data, int x, int y, t_color color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bpp / 8));
	*(unsigned int *)dst = color;
}
