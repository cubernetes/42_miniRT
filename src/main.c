#include "libft.h"
#include "mlx.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

// todo: add quaternions
// todo: add transformations

void	init(void)
{
	(void)set_allocator(gc_malloc);
	(void)gc_set_context("DEFAULT");
}

void	finish(int exit_status)
{
	gc_free_all();
	exit(exit_status);
}

int	main2(void)
{
	double		t;
	t_vec3		*center;
	t_vec3		*axis;
	double		radius;
	double		height;
	t_cylinder	cylinder;
	t_vec3		*terminus;
	t_vec3		*ray_vec;
	t_ray		*ray;
	t_vec3		intersection;

	init();
	axis = &(t_vec3){.x = 0, .y = 1, .z = 0};
	center = &(t_vec3){.x = 0, .y = 11, .z = 0};
	radius = 1;
	height = 1;
	new_cylinder(&cylinder, &(t_cylinder){
		.center = center,
		.axis = axis,
		.radius = radius,
		.height = height});
	terminus = &(t_vec3){.x = 0, .y = 0, .z = 0};
	ray_vec = &(t_vec3){.x = 1, .y = 0, .z = 0};
	ray = &(t_ray){.terminus = terminus, .vec = ray_vec};
	t = NO_ROOTS;
	if (intersection_cylinder(&t, &cylinder, ray))
		printf("No intersection are possible\n");
	else
	{
		printf("t = %f, intersection point: ", t);
		(void)ray_at(ray, t, &intersection);
		print_vec3(&intersection);
	}
	finish(0);
	return (0);
}

struct s_rt_img
{
	void		*img;
	char		*addr;
	int			bpp;
	int			line_length;
	int			endian;
};

typedef struct s_rt_img	t_rt_img;


void	mlx_pixel_put_buf(t_rt_img *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bpp / 8));
	*(unsigned int *)dst = (unsigned int)color;
}

int	main(void)
{
	void		*mlx;
	void		*win;
	t_rt_img	img;
	t_sphere	sphere;
	t_vec3		center;
	double		radius = 20;
	t_vec3		terminus;
	t_vec3		orientation;
	t_ray		ray;
	double		old_t;
	double		t;
	t_plane		plane;
	t_vec3		norm;
	t_vec3		point;
	t_plane		plane2;
	t_vec3		norm2;
	t_vec3		point2;

	new_vec3(&terminus, 0, 0, 0);
	new_vec3(&center, 0, 0, -25);
	new_sphere(&sphere, &center, radius);
	new_vec3(&norm, 0, 1, 0);
	new_vec3(&point, 0, -29, 0);
	new_plane(&plane, &point, &norm);
	new_vec3(&norm2, 0, 1, 2);
	new_vec3(&point2, 0, 1, 0);
	new_plane(&plane2, &point2, &norm2);
	mlx = mlx_init(); // TODO: check NULL
	img.img = mlx_new_image(mlx, 800, 600); // TODO: check NULL
	win = mlx_new_window(mlx, 800, 600, "miniRT"); // TODO: check NULL
	img.addr = mlx_get_data_addr(img.img, &img.bpp,
			&img.line_length, &img.endian); // TODO: check NULL
	for (int i = 0; i < 600; ++i)
	{
		for (int j = 0; j < 800; ++j)
		{
			new_vec3(&orientation, j / 4.0 - 800.0 / 8, i / 4.0 - 600.0 / 8, -10);
			new_ray(&ray, &terminus, &orientation);
			t = NO_ROOTS;
			mlx_pixel_put_buf(&img, j, 600 - i, 0x00000000);
			intersection_plane(&t, &plane, &ray);
			old_t = DBL_MAX;
			if ((t < old_t || old_t == NO_ROOTS) && t >= 0)
			{
				mlx_pixel_put_buf(&img, j, 600 - i, 0x00CE9D52);
				old_t = t;
			}
			intersection_plane(&t, &plane2, &ray);
			if ((t < old_t || old_t == NO_ROOTS) && t >= 0)
			{
				mlx_pixel_put_buf(&img, j, 600 - i, 0x00f68656);
				old_t = t;
			}
			intersection_sphere(&t, &sphere, &ray);
			if ((t < old_t || old_t == NO_ROOTS) && t >= 0)
			{
				mlx_pixel_put_buf(&img, j, 600 - i, 0x00da2b27);
				old_t = t;
			}
		}
	}
	ft_printf("DONE\n");
	mlx_put_image_to_window(mlx, win, img.img, 0, 0);
	mlx_loop(mlx);
	return (0);
}
