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
	int nb;
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
	struct s_ambient_light *next;
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
	struct s_camera *next;
}	t_camera;

typedef struct s_light
{
	t_coordonnate *coor_light;
	double bright_light;
	t_color *color_light;
	struct s_light *next;
}	t_light;

typedef struct s_plane
{
	t_coordonnate *coor_plane;
	t_color *color_plane;
	t_coordonnate *vector_plane;
	struct s_plane *next;
}	t_plane;

typedef struct s_sphere
{
	t_coordonnate *coor_sphere;
	double diameter_sphere;
	t_color *color_sphere;
	struct s_sphere *next;
}	t_sphere;

typedef struct s_cylinder
{
	t_coordonnate *coor_cylinder;
	t_coordonnate *vector_cylinder;
	double diameter_cylinder;
	double height_cylinder;
	t_color *color_cylinder;
	struct s_cylinder *next;
}	t_cylinder;

typedef struct s_scene
{
	t_objects *object;
	t_ambient_light *ambient_light; //single instance
	t_camera *camera; //single instance
	t_light *light; // llinked list
	t_plane *plane; // llinked list
	t_sphere *sphere; // llinked list
	t_cylinder *cylinder;  // llinked list
	// struct s_scene *next;
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
void	init_objects(t_objects **input_data, char ***tokens,int counter);
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
void process_scene_tokens(char ***tokens, int counter, t_scene *scene);
void init_object_dispatch_table(t_objects **dispatch_table);
void add_object(t_objects **head, char *identifier, void (*assign_object)(char **, t_scene *));

#endif    