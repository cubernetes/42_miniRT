#include "miniRT.h"
#include "libft.h"

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int	parse_vec3(char *str, t_vec3 *vec, int flag)
{
	int		i;
	char	*ptr;
	char	*next_ptr;

	if (!str)
		return (EXIT_FAILURE);
	i = -1;
	ptr = str;
	while (++i < 3)
	{
		next_ptr = ft_strchr(ptr, ',');
		if (i < 2 && !next_ptr)
			return (EXIT_FAILURE);
		if (i == 2)
			vec->e[i] = ft_strtof(ft_strdup(ptr));
		else
			vec->e[i] = ft_strtof(ft_strndup(ptr, (size_t)(next_ptr - ptr)));
		if (isnan(vec->e[i]))
			return (EXIT_FAILURE);
		ptr = next_ptr + 1;
	}
	if (flag == NORM_VEC && (length_squared_vec3(vec) - 1.0) > EPSILON)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	parse_color(char *str, t_color *color)
{
	int		i;
	char	*ptr;
	char	*next_ptr;
	double	temp[3];

	*color = 0;
	if (!str)
		return (EXIT_FAILURE);
	i = -1;
	ptr = str;
	while (++i < 3)
	{
		next_ptr = ft_strchr(ptr, ',');
		if (i < 2 && !next_ptr)
			return (EXIT_FAILURE);
		if (i == 2)
			temp[i] = ft_strtof(ft_strdup(ptr));
		else
			temp[i] = ft_strtof(ft_strndup(ptr, (size_t)(next_ptr - ptr))); //todo: replace ft_strtof with ft_atoi in parse_colors
		if (isnan(temp[i]) || temp[i] > 255.0 || temp[i] < 0.0)
			return (EXIT_FAILURE);
		ptr = next_ptr + 1;
	}
	set_red(color, (unsigned int)(temp[0]));
	set_green(color, (unsigned int)(temp[1]));
	set_blue(color, (unsigned int)(temp[2]));
	return (EXIT_SUCCESS);
}

int	parse_camera(char *line, t_scene *scene)
{
	char	**arr;
	t_vec3	tmp;

	arr = ft_split(line, ' ');
	if (!arr[0] || !arr[1] || !arr[2] || !arr[3] || arr[4])
		return (EXIT_FAILURE);
	if (parse_vec3(arr[1], &scene->camera->pos, ANY_VEC))
		return (EXIT_FAILURE);
	if (parse_vec3(arr[2], &scene->camera->dir, NORM_VEC))
		return (EXIT_FAILURE);
	scene->fov = ft_strtof(arr[3]);
	if (isnan(scene->fov) || scene->fov < 0.0 || scene->fov > 180.0)
		return (EXIT_FAILURE);
	copy_vec3(&scene->camera->up, &scene->camera->dir);
	copy_vec3(&tmp, &scene->camera->dir);
	cross_product_vec3(&tmp, &(t_vec3){.x = 0, .y = -1, .z = 0});
	cross_product_vec3(&scene->camera->up, &tmp);
	sc_mult_vec3(&tmp, -1);
	copy_vec3(&scene->camera->right, &tmp);
	return (EXIT_SUCCESS);
}

int	parse_lights(char *line, t_list *lights)
{
	char	**arr;
	t_light	*temp;

	arr = ft_split(line, ' ');
	if (!ft_strncmp(arr[0], "A", 2))
	{
		if (!arr[0] || !arr[1] || !arr[2] || arr[3])
			return (EXIT_FAILURE);
		temp = ft_malloc(sizeof(t_light));
		temp->point = ft_malloc(sizeof(t_vec3));
		new_vec3(temp->point, 0, 0, 0);
		temp->ratio = ft_strtof(arr[1]);
		if (isnan(temp->ratio) || temp->ratio < 0.0 || temp->ratio > 1.0)
			return (EXIT_FAILURE);
		if (parse_color(arr[2], &temp->color))
			return (EXIT_FAILURE);
		lpush_left(lights, as_ptr(temp));
	}
	else if (!ft_strncmp(arr[0], "L", 2))
	{
		if (!arr[0] || !arr[1] || !arr[2] || !arr[3] || arr[4])
			return (EXIT_FAILURE);
		temp = ft_malloc(sizeof(t_light));
		temp->point = ft_malloc(sizeof(t_vec3));
		if (parse_vec3(arr[1], temp->point, ANY_VEC))
			return (EXIT_FAILURE);
		temp->ratio = ft_strtof(arr[2]);
		if (isnan(temp->ratio) || temp->ratio < 0.0 || temp->ratio > 1.0)
			return (EXIT_FAILURE);
		if (parse_color(arr[3], &temp->color))
			return (EXIT_FAILURE);
		lpush(lights, as_ptr(temp));
	}
	else
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	parse_objects(char *line, t_list *objects)
{
	char	**arr;
	t_obj	*temp;

	arr = ft_split(line, ' ');
	if (!ft_strncmp(arr[0], "sp", 3))
	{
		if (!arr[0] || !arr[1] || !arr[2] || !arr[3] || arr[4])
			return (EXIT_FAILURE);
		temp = ft_malloc(sizeof(t_obj));
		temp->type = TOK_SPHERE;
		temp->sphere.center = ft_malloc(sizeof(t_vec3));
		if (parse_vec3(arr[1], temp->sphere.center, ANY_VEC))
			return (EXIT_FAILURE);
		temp->sphere.radius = ft_strtof(arr[2]) / 2;
		if (isnan(temp->sphere.radius) || temp->sphere.radius <= 0)
			return (EXIT_FAILURE);
		if (parse_color(arr[3], &temp->color))
			return (EXIT_FAILURE);
	}
	else if (!ft_strncmp(arr[0], "pl", 3))
	{
		if (!arr[0] || !arr[1] || !arr[2] || !arr[3] || arr[4])
			return (EXIT_FAILURE);
		temp = ft_malloc(sizeof(t_obj));
		temp->type = TOK_PLANE;
		temp->plane.point = ft_malloc(sizeof(t_vec3));
		temp->plane.norm = ft_malloc(sizeof(t_vec3));
		if (parse_vec3(arr[1], temp->plane.point, ANY_VEC))
			return (EXIT_FAILURE);
		if (parse_vec3(arr[2], temp->plane.norm, NORM_VEC))
			return (EXIT_FAILURE);
		if (parse_color(arr[3], &temp->color))
			return (EXIT_FAILURE);
	}
	else if (!ft_strncmp(arr[0], "cy", 3))
	{
		if (!arr[0] || !arr[1] || !arr[2] || !arr[3] || !arr[4] || !arr[5]
			|| arr[6])
			return (EXIT_FAILURE);
		temp = ft_malloc(sizeof(t_obj));
		temp->type = TOK_CYLINDER;
		temp->cylinder.center = ft_malloc(sizeof(t_vec3));
		temp->cylinder.axis = ft_malloc(sizeof(t_vec3));
		if (parse_vec3(arr[1], temp->cylinder.center, ANY_VEC))
			return (EXIT_FAILURE);
		if (parse_vec3(arr[2], temp->cylinder.axis, NORM_VEC))
			return (EXIT_FAILURE);
		temp->cylinder.radius = ft_strtof(arr[3]) / 2;
		if (isnan(temp->cylinder.radius) || temp->cylinder.radius <= 0)
			return (EXIT_FAILURE);
		temp->cylinder.height = ft_strtof(arr[4]);
		if (isnan(temp->cylinder.height) || temp->cylinder.height <= 0)
			return (EXIT_FAILURE);
		if (parse_color(arr[5], &temp->color))
			return (EXIT_FAILURE);
		new_cylinder(&temp->cylinder, &temp->cylinder);
	}
	else
		return (EXIT_FAILURE);
	lpush(objects, as_ptr(temp));
	return (EXIT_SUCCESS);
}

//todo: add max limit for some elements
//todo: add permanent context for all the elements in the end
//todo: add FOV to C (just move the viewport but keep # of pixel same?)
//      focal_distance = viewport_width / (2 * tan(fov_theta / 2))
int	read_rt_file(char *filename, t_scene *scene)
{
	int			fd;
	char		*line;
	t_list		*objects;
	t_list		*lights;
	int			flag;
	bool		has_cam;

	if (ft_strncmp(filename + ft_strlen(filename) - 3, ".rt", 4))
	{
		ft_dprintf(STDERR_FILENO, "Error: wrong extension\n");
		return (EXIT_FAILURE);
	}
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_dprintf(STDERR_FILENO, "Error: wrong file\n");
		return (EXIT_FAILURE);
	}
	objects = lnew();
	lights = lnew();
	line = get_next_line(fd);
	has_cam = false;
	while (line)
	{
		if (line[0] != '\n' && line[0] != '#')
		{
			line[ft_strlen(line) - 1] = 0;
			if (!ft_strncmp(line, "C ", 2))
			{
				flag = parse_camera(line, scene);
				has_cam = true;
			}
			else if (!ft_strncmp(line, "A ", 2) || !ft_strncmp(line, "L ", 2))
				flag = parse_lights(line, lights);
			else if (!ft_strncmp(line, "sp ", 3) || !ft_strncmp(line, "pl ", 3)
				|| !ft_strncmp(line, "cy ", 3))
				flag = parse_objects(line, objects);
			else
				flag = EXIT_FAILURE;
			if (flag)
			{
				close(fd);
				return (EXIT_FAILURE);
			}
		}
		line = get_next_line(fd);
	}
	if (!has_cam)
		return (EXIT_FAILURE);
	scene->objects = ltoarr(objects);
	scene->nb_objs = (int)objects->len;
	scene->lights = ltoarr(lights);
	scene->nb_lights = (int)lights->len;
	close(fd);
	return (EXIT_SUCCESS);
}
