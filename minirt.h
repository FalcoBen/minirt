#ifndef MINI_RT_H
#define MINI_RT_H

#include "MLX42/include/MLX42/MLX42.h"
#include "get_next_line/get_next_line.h"
// #include "libft/libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <libc.h>
#include <stdbool.h>
typedef struct s_scene t_scene;
typedef struct s_objects
{
	char *identifier;
	void (*assign_object)(char **data, t_scene *scene);
	struct s_objects *next;
} t_objects;

typedef struct s_container
{
	char *line;
	struct s_container	*next;
}	t_container;

typedef struct s_color
{
	int r;
	int g;
	int b;
}	t_color;

typedef struct s_ambient_light
{
	double bright_ambient_light;
	t_color *color_ambient_light;
}	t_ambient_light;

typedef struct s_coordonnate
{
	double x;
	double y;
	double z;
}	t_coordonnate;

typedef struct s_camera
{
	t_coordonnate *coor_camera;
	t_coordonnate *vector_camera;
	double angle_view;
}	t_camera;

typedef struct s_light
{
	t_coordonnate *coor_light;
	double bright_light;
	t_color *color_light;
}	t_light;

typedef struct s_plane
{
	t_coordonnate *coor_plane;
	t_color *color_plane;
	t_coordonnate *vector_plane;
}	t_plane;

typedef struct s_sphere
{
	t_coordonnate *coor_sphere;
	double diameter_sphere;
	t_color *color_sphere;
}	t_sphere;

typedef struct s_cylinder
{
	t_coordonnate *coor_cylinder;
	t_coordonnate *vector_cylinder;
	double diameter_cylinder;
	double height_cylinder;
	t_color *color_cylinder;
}	t_cylinder;

typedef struct s_scene
{
	t_ambient_light *ambient_light;
	t_camera *camera;
	t_light *light;
	t_plane *plane;
	t_sphere *sphere;
	t_cylinder *cylinder;
	struct s_scene *next;
}	t_scene;
/*-----------linked list----------------------*/

void	ft_lstadd_back(t_container **lst, t_container *new);
void	ft_lstadd_front(t_container **lst, t_container *new);
t_container	*ft_lstnew(void *content);
t_container	*ft_lstlast(t_container *lst);
void	ft_lstclear(t_container **lst, void (*del)(void *));
void	ft_lstdelone(t_container *lst, void (*del)(void *));
/*------------------------------------------------------*/
char	**ft_split(char const *s, char c);
char **ft_split_white(char *str);
/*-------------objects--------------*/
void	init_objects(t_objects *input_data, char ***tokens,int counter);
double	ft_atoi_double(char *str);
int		ft_atoi_color(char *str, char *scene);
bool	non_num_chara(char *str, int i);
void	skip_spaces(char *str, int *i);
void	exit_error(char *str, char *scene);


void	ft_ambient_light(char **data, t_scene *scene);
void	ft_cylinder(char **data, t_scene *scene);
void	ft_camera(char **data, t_scene *scene);
void	ft_sphere(char **data, t_scene *scene);
void	ft_plane(char **data, t_scene *scene);
void	ft_light(char **data, t_scene *scene);
/*********initiaie_scenes***************/
void	initialize_ambient_light(t_scene *scene);
void	initialize_light(t_scene *scene);
void	initialize_plane(t_scene *scene);
void	initialize_spher(t_scene *scene);
void	initialize_cylinder(t_scene *scene);
void	initialize_camera(t_scene *scene);

/************************/
void	printer(t_scene *scene);
#endif    