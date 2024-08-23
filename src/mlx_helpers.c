#include "miniRT.h"

void	mlx_pixel_put_buf(t_rt_img *data, int x, int y, t_color color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bpp / 8));
	*(unsigned int *)dst = color;
}
