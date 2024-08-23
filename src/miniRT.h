#ifndef MINIRT_H
# define MINIRT_H 1

# include "libft.h"

/********** enums **********/
enum					e_obj_type
{
	PLANE,
	SPHERE,
	CYLINDER,
};


/************* typedefs ***************/
typedef enum e_obj_type	t_obj_type;
typedef int				(*t_hook)(void *);

/* forward declarations */
typedef struct s_rt_img	t_rt_img;
typedef struct s_gc		t_gc;
typedef struct s_scene	t_scene;
typedef struct s_obj	t_obj;


/********** struct defintions **********/
struct					s_rt_img
{
	void				*img;
	char				*addr;
	int					bpp;
	int					line_length;
	int					endian;
};

/* gc == graphics context */
struct					s_gc
{
	void				*mlx;
	void				*win;
	t_rt_img			img;
};

struct					s_scene
{
	int					wheight;
	int					wwidth;
	int					nb_objs;
	double				amb_light;
};

struct					s_obj
{
	t_obj_type			type;
	t_color				color;
	union
	{
		t_plane			plane;
		t_sphere		sphere;
		t_cylinder		cylinder;
	};
};


/***************** prototypes ****************/
/* main.c */
void					finish(int exit_status, t_gc *gc);

/* mlx_helpers.c */
void					mlx_pixel_put_buf(t_rt_img *data, int x, int y,
							t_color color);
// void	*mlx_new_resizable_window(t_xvar *xvar, int size_x, int size_y,
// 			char *title);

/* render.c */
t_color					cast_ray(double *t, t_ray *ray, t_obj *objects,
							int nb_objs);
void					render(t_gc *gc, t_scene *scene, t_obj *objects);
int						parse_input(t_obj **objects, t_scene *scene);

#endif /* miniRT.h */
