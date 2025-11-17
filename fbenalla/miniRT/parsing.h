/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 01:04:08 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/14 02:06:09 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../../MLX42/include/MLX42/MLX42.h"
# include "../get_next_line/get_next_line.h"

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <limits.h>
// # include <string.h>
# include <stdio.h>
# include <stdbool.h>
// test
typedef struct s_parse
{
	int						i;
	int						signe;
	unsigned long long		nb;
	double					frac;
	double					divisor;
}	t_parse;

typedef struct s_cleanup	t_cleanup;
typedef struct s_scene		t_scene;

typedef struct t_objects_fb
{
	char					*identifier;
	int						nb;
	void					(*assign_object)(char **data, t_scene *scene);
	struct t_objects_fb		*next;
}	t_objects_fb;

typedef struct s_container
{
	char				*line;
	struct s_container	*next;
}	t_container;

typedef struct s_color_fb
{
	int	r;
	int	g;
	int	b;
}	t_color_fb;

typedef enum e_type
{
	T_SPHERE,
	T_CYLINDRE,
	T_CONE,
	T_PLAN
}t_type;

typedef struct s_texture
{
	int				type;
	t_color_fb		color_solid;
	t_color_fb		color_checkerd;
	mlx_texture_t	*image;
	double			scale;
}	t_texture;

typedef struct s_ambient_light
{
	t_type					type;
	double					bright_ambient_light_fb;
	t_color_fb				*color_ambient_light;
	struct s_ambient_light	*next;
}	t_ambient_light_fb;

typedef struct s_texture_color
{
	double	x;
	double	y;
	double	z;
}	t_texture_color;

typedef struct s_camera_fb
{
	t_type				type;
	t_texture_color		*coor_camera;
	t_texture_color		*vector_camera;
	double				angle_view;
	struct s_camera_fb	*next;
}	t_camera_fb;

typedef struct s_light_fb
{
	t_type				type;
	t_texture_color		*coor_light;
	double				bright_light;
	t_color_fb			*color_light;
	struct s_light_fb	*next;
}	t_light_fb;

typedef struct s_plane_fb
{
	t_type				type;
	t_texture_color		*coor_plane;
	t_color_fb			*color_plane;
	t_texture_color		*vector_plane;
	t_texture			*bump_texture;
	char				*img_path;
	bool				flag_bump;
	struct s_plane_fb	*next;
}	t_plane_fb;

typedef struct s_sphere_fb
{
	t_type				type;
	t_texture_color		*coor_sphere;
	double				diameter_sphere;
	t_color_fb			*color_sphere;
	t_texture			*bump_texture;
	char				*img_path;
	bool				flag_bump;
	struct s_sphere_fb	*next;
}	t_sphere_fb;

typedef struct s_cylinder_fb
{
	t_type					type;
	t_texture_color			*coor_cylinder;
	t_texture_color			*vector_cylinder;
	double					diameter_cylinder;
	double					height_cylinder_fb;
	t_color_fb				*color_cylinder;
	t_texture				*bump_texture;
	char					*img_path;
	bool					flag_bump;
	bool					closed;
	struct s_cylinder_fb	*next;
}	t_cylinder_fb;

typedef struct s_cone_fb
{
	t_type				type;
	t_texture_color		*coor_cone;
	t_texture_color		*vector_cone;
	t_color_fb			*color_cone;
	t_texture			*bump_texture;
	char				*img_path;
	bool				flag_bump;
	double				minimum;
	double				maximum;
	double				angle;
	bool				closed_flag;
	struct s_cone_fb	*next;
}	t_cone_fb;

typedef struct s_scene
{
	int					obj_num;
	int					light_num;
	t_type				type;
	t_objects_fb		*object;
	t_ambient_light_fb	*ambient_light;
	t_camera_fb			*camera;
	t_light_fb			*light;
	t_plane_fb			*plane;
	t_sphere_fb			*sphere;
	t_cylinder_fb		*cylinder;
	t_cone_fb			*cone;
	t_cleanup			*cleaner;
}	t_scene;

typedef struct s_cleanup
{
	t_container		*container;
	t_scene			*scene;
	t_objects_fb	**dispatched_table;
	t_objects_fb	*input_data;
	char			***tokens;
	int				token_count;
	bool			flag_input;
	bool			flag_exit;
}t_cleanup;

/*-----------linked list----------------------*/
void				ft_lstadd_back(t_container **lst, t_container *new);
void				ft_lstadd_front(t_container **lst, t_container *new);
t_container			*ft_lstnew(void *content);
t_container			*ft_lstlast(t_container *lst);
void				ft_lstclear(void **lst, void (*del)(void *), char c);
void				ft_lstdelone(void *lst, void (*del)(void *), char c);
void				del(void *content);
/*------------------------------------------------------*/

/*----------------------------parsing------------------------------------*/
void				init_objects_fb(t_objects_fb **input_data, \
					char ***tokens, int counter);
double				ft_atoi_double(char *str, t_cleanup *clean);
int					ft_atoi_color(char *str, char *scene);
bool				verify_color(char *str);
bool				verify_number(char *str);
bool				non_num_chara(char *str, int i);
void				skip_spaces(char *str, int *i);
int					ft_isdigit(int c);
bool				verify_data_plane(char **data);
bool				verify_data_sphere(char **data, t_cleanup *clean);
bool				verify_data_cylinder(char **data, t_cleanup *clean);
bool				verify_data_cone(char **data, t_cleanup *clean);
bool				check_extension(char *str);
void				initialize_scenes(t_scene *scene);
void				parsing(int fd);
void				set_scene(t_scene *scene, bool flag);
bool				is_coordinate(char *coor);
void				exit_error(char *str, char *scene_name, t_cleanup *cleanup);

bool				check_after_split(char **str);
int					count_comma(char *str);
bool				verify_color(char *str);
char				**ft_split(char const *s, char c);
char				**ft_split_white(char *str);
void				process_scene_tokens(char ***tokens, int counter, \
					t_scene *scene);
void				init_object_dispatch_table(t_objects_fb **dispatch_table);
void				add_object(t_objects_fb **head, char *identifier, \
					void (*assign_object)(char **, t_scene *));
/*----------------------------objects------------------------------------*/

void				ft_ambient_light_fb(char **data, t_scene *scene);
void				ft_cylinder_fb(char **data, t_scene *scene);
void				ft_camera_fb(char **data, t_scene *scene);
void				ft_sphere_fb(char **data, t_scene *scene);
void				ft_plane_fb(char **data, t_scene *scene);
void				ft_light(char **data, t_scene *scene);
void				ft_cone_fb(char **data, t_scene *scene);
/*********initiaie_scenes***************/
void				initialize_ambient_light(t_scene *scene);
void				initialize_light(t_scene *scene);
void				initialize_plane(t_scene *scene);
void				initialize_spher(t_scene *scene);
void				initialize_cylinder(t_scene *scene);
void				initialize_camera(t_scene *scene);
bool				verify_data_cylinder(char **data, t_cleanup *clean);
void				cylinder_linked_list(t_cylinder_fb *new_cylinder, \
					t_scene *scene);
void				init_new_cylinder(t_cylinder_fb *new_cylinder);
/*------------------------Textures------------------------*/
void				bump_mapping_cylinder_constractor(t_cylinder_fb \
					*new_cylinder, char **data, t_scene *scene, int i);
void				sphere_linked_list(t_sphere_fb *new_sphere, t_scene *scene);
void				init_new_sphere(t_sphere_fb *new_sphere);
void				bump_mapping_sphere_constractor(t_sphere_fb *new_sphere, \
					char **data, t_scene *scene, int i);
void				bump_mapping_plane_constractor(t_plane_fb *new_plane, \
					char **data, t_scene *scene, int i);
void				init_new_plane(t_plane_fb *new_plane);
void				plane_linked_list(t_plane_fb *new_plane, t_scene *scene);
void				bump_mapping_cone_constractor(t_cone_fb *new_cone, \
					char **data, t_scene *scene, int i);
void				init_new_cone(t_cone_fb *new_cone);
bool				verify_data_cone(char **data, t_cleanup *clean);
t_texture_color		sample_color(t_texture *tex, double u, double v);
void				close_window(mlx_key_data_t keydata, void *param);
void				cone_linked_list(t_cone_fb *new_cone, t_scene *scene);
/*------------------------free------------------------*/
void				ft_free_split(char **str);
void				free_scene(t_scene *scene);
void				free_cone(t_scene *scene);
void				free_cylinder(t_scene *scene);
void				free_plane(t_scene *scene);
void				free_sphere(t_scene *scene);
void				free_params_co(t_cone_fb *co);
void				free_params_cy(t_cylinder_fb *cy);
void				free_params_pl(t_plane_fb *pl);
void				free_params_sp(t_sphere_fb *sp);
void				check_valid_args(char **data, t_scene *scene, char object);
bool				verify_data_ambient_light(char **data, t_cleanup *clean);
void				ft_camera_fb(char **data, t_scene *scene);
int					ft_atoi(char *str);
int					ft_check(int signe);
char				*ft_strdup(const char *s1);
void				make_sure_of_objects(t_scene *scene, t_cleanup *cleaner);
int					ft_strcmp(const char *s1, const char *s2);
char				*ft_strrchr(const char *s, int c);
bool				count_dot(char *str, int i);
#endif