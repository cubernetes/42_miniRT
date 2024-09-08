#ifndef MINIRT_H
# define MINIRT_H 1

# include "libft.h"

# include <stdlib.h>

# define EPSILON 0.0001
# define EPSILON2 0.0000001

/* parse.c */
# define ANY_VEC 0
# define NORM_VEC 1

/* render.c */
# define X 0
# define Y 1
# define ROW_START_VEC 0
# define PIXEL 1

/********** enums **********/

enum						e_token_rt
{
	TOK_AMB_LIGHT = 0,
	TOK_CAMERA,
	TOK_LIGHT,
	TOK_SPHERE,
	TOK_PLANE,
	TOK_CYLINDER,
	TOK_ERROR,
};

enum						e_direction
{
	DIR_UP = 0,
	DIR_DOWN,
	DIR_FORWARD,
	DIR_BACKWARD,
	DIR_LEFT,
	DIR_RIGHT,
};

/************* typedefs ***************/
typedef enum e_token_rt		t_token_rt;
typedef enum e_direction	t_direction;
typedef int					(*t_hook)(void *, ...);

/* forward declarations */
typedef struct s_rt_img		t_rt_img;
typedef struct s_gc			t_gc;
typedef struct s_scene		t_scene;
typedef struct s_obj		t_obj;
typedef struct s_light		t_light;
typedef struct s_hit		t_hit;
typedef struct s_camera		t_camera;
typedef struct s_viewport	t_viewport;

/********** struct defintions **********/
struct						s_camera
{
	t_vec3					pos;
	t_vec3					dir;
	t_vec3					up;
	t_vec3					right;
};

struct						s_viewport
{
	double					width;
	double					height;
	t_vec3					right_step;
	t_vec3					down_step;
	t_vec3					top_left;
};

struct						s_rt_img
{
	void					*img;
	char					*addr;
	int						bpp;
	int						line_length;
	int						endian;
};

/* gc == graphics context */
struct						s_gc
{
	void					*mlx;
	void					*win;
	t_rt_img				img;
	t_scene					*scene;
};

struct						s_scene
{
	int						window_height;
	int						window_width;
	t_light					**lights;
	t_obj					**objects;
	int						nb_lights;
	int						nb_objs;
	t_camera				*camera;
	t_viewport				*viewport;
	double					fov;
};

struct						s_obj
{
	t_token_rt				type;
	t_color					color;
	union
	{
		t_plane				plane;
		t_sphere			sphere;
		t_cylinder			cylinder;
	};
};

struct						s_light
{
	t_vec3					*point;
	t_color					color;
	double					ratio;
};

struct						s_hit
{
	double					t;
	t_vec3					point;
	t_vec3					norm;
	t_vec3					ray_dir;
	t_obj					*object;
	t_color					color;
};

/***************** prototypes ****************/
/* main.c */
void						finish(int exit_status, t_gc *gc);

/* mlx_helpers.c */
void						mlx_pixel_put_buf(t_rt_img *data, int x, int y,
								t_color color);
// void	*mlx_new_resizable_window(t_xvar *xvar, int size_x, int size_y,
// 			char *title);

/* render.c */
int							cast_ray(t_hit *hit, t_ray *ray, t_scene *scene);
void						render(t_gc *gc, t_scene *scene);
void						assert_norm(t_hit *hit, int index);
/* lights.c */
void						apply_light(t_color *color, t_color light);
t_color						calculate_lighting(t_hit *hit, t_scene *scene);

/* parser.c */
int							parse_vec3(char *str, t_vec3 *vec, int flag);
int							parse_color(char *str, t_color *color);
int							read_rt_file(char *file, t_scene *scene);

/* parser_utils1.c */
int							parse_camera(char *line, t_scene *scene);
int							parse_lights(char *line, t_list *lights);
int							parse_sphere(char *line,
								t_list *objects, char **arr);
int							parse_plane(char *line,
								t_list *objects, char **arr);
int							parse_cylinder(char *line,
								t_list *objects, char **arr);

/* parser_utils2.c */
int							parse_line(char *line, t_scene *scene,
								t_list *objects, t_list *lights);
int							open_rt_file(char *file, int *fd);
bool						init_parse(t_list **objects,
								t_list **lights, char **line, int fd);
void						end_parse(t_scene *scene,
								t_list *objects, t_list *lights, int fd);

/* translate_camera.c */
void						translate_camera(t_scene *scene,
								t_direction direction, double amount);

#endif /* miniRT.h */
