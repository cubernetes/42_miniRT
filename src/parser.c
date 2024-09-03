#include "miniRT.h"
#include "libft.h"
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#define ANY_VEC 0
#define NORM_VEC 1

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
		vec->e[i] = ft_atof(ft_strndup(ptr, next_ptr - ptr));
		if (flag == NORM_VEC && vec->e[i] > 1.0 ||vec->e[i] < -1.0)
			return (EXIT_FAILURE);
		if (vec->e[i] == NAN)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	parse_color(char *str, t_color *color)
{
	int		i;
	char	*ptr;
	char	*next_ptr;
	int		temp[3];

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
		temp[i] = ft_atof(ft_strndup(ptr, next_ptr - ptr));//todo: replace ft_atof with ft_atoi in parse_colors
		if (temp[i] == NAN || temp[i] > 255 || temp[i] < 0)
			return (EXIT_FAILURE);
	}
	set_red(color, temp[0]);
	set_green(color, temp[1]);
	set_blue(color, temp[2]);
	return (EXIT_SUCCESS);
}

int	parse_camera(char *line, t_scene *scene)
{
	char	**arr;

	arr = ft_split(line, ' ');
	if (arr[4])
		return (EXIT_FAILURE);
	if (parse_vec3(arr[0], &scene->camera_pos, ANY_VEC))
		return (EXIT_FAILURE);
	if (parse_vec3(arr[1], &scene->camera_dir, NORM_VEC))
		return (EXIT_FAILURE);
	scene->fov = ft_atof(arr[2]);
	if (scene->fov == NAN)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	parse_lights(char *line, t_list *lights, t_scene *scene)
{
	char	**arr;
	t_light	*temp;

	arr = ft_split(line, ' ');
	if (ft_strncmp(arr[0], "A", 1))
	{
		if (!arr[0] || !arr[1] || !arr[2] || arr[3])
			return (EXIT_FAILURE);
		scene->amb_light->point = ft_malloc(sizeof(t_vec3));
		new_vec3(scene->amb_light->point, 0, 0, 0);
		scene->amb_light->ratio = ft_atof(arr[1]);
		if (scene->amb_light->ratio == NAN)
			return (EXIT_FAILURE);
		if (parse_color(arr[2], &scene->amb_light->color))
			return (EXIT_FAILURE);
	}
	else if (ft_strncmp(arr[0], "L", 1))
	{
		if (!arr[0] || !arr[1] || !arr[2] || !arr[3] || arr[4])
			return (EXIT_FAILURE);
		temp = ft_malloc(sizeof(t_light));
		temp->point = ft_malloc(sizeof(t_vec3));
		if (parse_vec3(arr[1], temp->point, ANY_VEC))
			return (EXIT_FAILURE);
		temp->ratio = ft_atof(arr[1]);
		if (temp->ratio == NAN)
			return (EXIT_FAILURE);
		if (parse_color(arr[2], &temp->color))
			return (EXIT_FAILURE);
		lpush(lights, as_ptr(temp));
	}
	return (EXIT_SUCCESS);
}

int	parse_objects(char *line, t_list *objects)
{
	char	**arr;
	t_obj	*temp;

	arr = ft_split(line, ' ');
	if (ft_strncmp(arr[0], "sp", 2))
	{
		if (!arr[0] || !arr[1] || !arr[2] || !arr[3] || arr[4])
			return (EXIT_FAILURE);
		temp = ft_malloc(sizeof(t_obj));
		temp->type = TOK_SPHERE;
		temp->sphere.center = ft_malloc(sizeof(t_vec3));
		if (parse_vec3(arr[1], temp->sphere.center, ANY_VEC))
			return (EXIT_FAILURE);
		temp->sphere.radius = ft_atof(arr[2]) / 2;
		if (temp->sphere.radius == NAN || temp->sphere.radius <= 0)
			return (EXIT_FAILURE);
		if (parse_color(arr[3], &temp->color))
			return (EXIT_FAILURE);
	}
	else if (ft_strncmp(arr[0], "pl", 2))
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
	else if (ft_strncmp(arr[0], "cy", 2))
	{
		if (!arr[0] || !arr[1] || !arr[2] || !arr[3] || !arr[4] || !arr[5] || arr[6])
			return (EXIT_FAILURE);
		temp = ft_malloc(sizeof(t_obj));
		temp->type = TOK_CYLINDER;
		temp->cylinder.center = ft_malloc(sizeof(t_vec3));
		temp->cylinder.axis = ft_malloc(sizeof(t_vec3));
		if (parse_vec3(arr[1], temp->cylinder.center, ANY_VEC))
			return (EXIT_FAILURE);
		if (parse_vec3(arr[2], temp->cylinder.axis, NORM_VEC))
			return (EXIT_FAILURE);
		temp->cylinder.radius = ft_atof(arr[3]) / 2;
		if (temp->cylinder.radius == NAN || temp->cylinder.radius <= 0)
			return (EXIT_FAILURE);
		temp->cylinder.height = ft_atof(arr[4]) / 2;
		if (temp->cylinder.height == NAN || temp->cylinder.height <= 0)
			return (EXIT_FAILURE);
		if (parse_color(arr[5], &temp->color))
			return (EXIT_FAILURE);
	}
	lpush(objects, as_ptr(temp));
	return (EXIT_SUCCESS);
}

//todo: transform lists to arrays
//todo: add max limit for some elements
//todo: add permanent context for all the elements in the end
int	read_rt_file(char *filename, t_scene *scene)
{
	int			fd;
	char		*line;
	t_list		*objects;
	t_list		*lights;
	int			flag;

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
	line = get_next_line(fd);
	while (line)
	{
		if (line[0] != '\n')
		{
			if (!ft_strncmp(line, "C ", 2))
				flag = parse_camera(line, scene);
			else if (!ft_strncmp(line, "A ", 2) || !ft_strncmp(line, "L ", 2))
				flag = parse_lights(line, lights, scene);
			else if (!ft_strncmp(line, "sp ", 3) || !ft_strncmp(line, "pl ", 3) || !ft_strncmp(line, "cy ", 3))
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
	close(fd);
	return (EXIT_SUCCESS);
}
