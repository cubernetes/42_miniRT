/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 21:48:04 by tischmid          #+#    #+#             */
/*   Updated: 2024/09/19 08:17:10 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H 1

# include "libft.h"
# include "mlx_int.h"
# include <stdlib.h>

# define EPSILON 0.0001
# define EPSILON2 0.0000001

/* parse.c */
# define ANY_VEC 0
# define NORM_VEC 1

/* render.c */
# define X 0
# define Y 1
# define I 2
# define J 3
# define ROW_START_VEC 0
# define PIXEL 1

# define WINDOW_WIDTH 1920
# define WINDOW_HEIGHT 1080

# define SAMPLE_SIZE 7
# define RESOLUTION 8

# define CAM_ROTATE_FACTOR 10.0
# define MOVE_DELAY 1.0
# define MOVE_STEP 80

# define CURSOR_SIZE 10
# define CURSOR_CLR 0x00FFFFFF

# define MIN_FPS 30

# define ANTIALIASING 0

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
typedef struct s_control	t_control;

/********** struct defintions **********/
struct						s_camera
{
	t_vec3					pos;
	t_vec3					dir;
	t_vec3					up;
	t_vec3					right;
	/* int						center_x; */
	/* int						center_y; */
};

struct						s_viewport
{
	float					width;
	float					height;
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
	t_rt_img				img;//colors with after lighting
	t_rt_img				img2;//colors after antialiasing
	t_rt_img				img3;//colors before lighting
	t_scene					*scene;
	int						sample;
	int						sample_size;
	int						resolution;
	int						ideal_resolution;
	float					last_moved;
	bool					fully_rendered;
	int						x;
	int						y;
	int						frames_rendered;
	float					fps_start;
	float					fps;
	char					*fps_string;
	bool					mouse_hidden;
	int						antialiasing;
	int						interpolation;
};
// NOTE: last_moved is float because ft_uptime_linux returns float
// thereforce, this feature exists only for linux AT THE MOMENT

struct						s_control
{
	bool					w_pressed;
	bool					a_pressed;
	bool					s_pressed;
	bool					d_pressed;
	bool					space_pressed;
	bool					lshift_pressed;
	bool					lctrl_pressed;
	bool					f_pressed;
	enum
	{
		MENU,
		CAMERA,
		OBJECT,
	} e_control_type;
	union
	{
		t_camera			*camera;
		t_obj				*object;
	} u_control_object;
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
	float					fov;
	t_control				control;
};

struct						s_obj
{
	t_token_rt				type;
	t_color					color;
	bool					selected;
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
	float					ratio;
};

struct						s_hit
{
	float					t;
	t_vec3					point;
	t_vec3					norm;
	t_vec3					ray_dir;
	t_obj					*object;
	t_color					color;
};

/***************** prototypes ****************/
/* main.c */
void						finish(int exit_status, t_gc *gc);

/* render.c */
int							cast_ray(t_hit *hit, t_ray *ray, t_scene *scene);
int							render(void *arg);
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
int							parse_sphere(t_list *objects, char **arr);
int							parse_plane(t_list *objects, char **arr);
int							parse_cylinder(t_list *objects, char **arr);

/* parser_utils2.c */
int							parse_line(char *line, t_scene *scene,
								t_list *objects, t_list *lights);
int							open_rt_file(char *file, int *fd);
int							init_parse(t_list **objects, t_list **lights,
								char **line, int fd);
void						end_parse(t_scene *scene, t_list *objects,
								t_list *lights, int fd);

/* mlx_helpers.c */
void						mlx_pixel_put_buf(t_rt_img *data, int x, int y,
								t_color color);
int							mlx_get_window_dim(void *mlx_ptr, void *win_ptr,
								int *width, int *height);
void						*mlx_new_resizable_window(t_xvar *xvar, int size_x,
								int size_y, char *title);
unsigned int				mlx_pixel_get_buf(t_rt_img *data, int x, int y);

/* printing.c */
void						print_light(t_light *light);

/* setup_mlx.c */
void						setup_mlx(t_gc *gc, t_scene *scene);
void						setup_hooks(t_gc *gc);

/* mlx_hooks.c */
int							mouse_down_hook(void *arg1, ...);
int							keydown_hook(void *arg1, ...);
int							keyup_hook(void *arg1, ...);
int							destroy_hook(void *arg1, ...);
int							move_hook(void *arg1, ...);
int							update_window(void *arg1, ...);

/* rotate_object.c */
void						rotate_object(t_obj *obj, t_quat *quat);

/* orient_camera.c */
void						rotate_camera(t_camera *camera,
								t_direction direction, float degrees);
void						camera_yaw(t_scene *scene, int amount);
void						camera_pitch(t_scene *scene, int amount);

/* translate_camera.c */
bool						translate_camera(t_camera *camera,
								t_direction direction, float amount);

/* antialiasing.c */
int							edge_detection(t_rt_img *img, int x, int y,
								int resolution);
void						apply_random_antialiasing(t_gc *gc, int width,
								int height);
void						apply_pattern_antialiasing(t_gc *gc, int width,
								int height, int resolution);
/* interpolation.c */
void						interpolation(t_gc *gc, unsigned int width,
								unsigned int height, unsigned int resolution);

/* todo norminette: currently in src/lights.c, put in separate file */
t_vec3						*get_object_pos(t_obj *obj);
char						*obj_type_to_str(t_obj *obj);

#endif /* miniRT.h */
