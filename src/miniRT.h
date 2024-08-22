#ifndef MINIRT_H
# define MINIRT_H	1

// # include "mlx_int.h"

/* forward declarations */
typedef struct s_rt_img	t_rt_img;
typedef struct s_gc		t_gc;
typedef struct s_scene	t_scene;

/* struct defintions */
struct s_rt_img
{
	void		*img;
	char		*addr;
	int			bpp;
	int			line_length;
	int			endian;
};

/* gc == graphics context */
struct s_gc
{
	void		*mlx;
	void		*win;
	t_rt_img	img;
};

struct s_scene
{
	int	wheight;
	int	wwidth;
};

/* main.c */
void	finish(int exit_status, t_gc *gc);

/* mlx_helpers.c */
void	mlx_pixel_put_buf(t_rt_img *data, int x, int y, int color);
// void	*mlx_new_resizable_window(t_xvar *xvar, int size_x, int size_y,
// 			char *title);

#endif /* miniRT.h */
