/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbahmida <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 04:45:27 by jbahmida          #+#    #+#             */
/*   Updated: 2025/11/12 04:45:31 by jbahmida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MiniRt.h"

void	sort_intersctions(t_stack_intersections *inters)
{
	t_stack_intersections	*list;
	t_inters				tmp;
	int						i;
	int						j;

	if (!inters)
		return ;
	list = inters;
	i = 0;
	while (i < list->count)
	{
		j = 0;
		while (j < list->count - i - 1)
		{
			if (list->world_inters_list[j].inters_value
				> list->world_inters_list[j + 1].inters_value)
			{
				tmp = list->world_inters_list[j];
				list->world_inters_list[j] = list->world_inters_list[j + 1];
				list->world_inters_list[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	s_intersect_concatenate_1(t_stack_intersections *arr1,
	t_stack_intersections *arr2)
{
	t_inters	*tmp;
	int			count;
	int			i;
	int			j;

	count = arr1->count + arr2->count;
	tmp = arr1->world_inters_list;
	arr1->world_inters_list = malloc(sizeof(t_inters) * count);
	i = 0;
	j = 0;
	while (j < arr1->count)
	{
		arr1->world_inters_list[i] = tmp[j];
		i++;
		j++;
	}
	j = 0;
	while (j < arr2->count)
	{
		arr1->world_inters_list[i] = arr2->inters_list[j];
		i++;
		j++;
	}
	arr1->count = count;
	free(tmp);
}

void	s_intersect_concatenate(t_stack_intersections *arr1,
	t_stack_intersections *arr2)
{
	t_inters	*tmp;
	int			count;
	int			i;
	int			j;

	if (!arr2->count)
		return ;
	if (!arr1->count)
	{
		arr1->count = arr2->count;
		arr1->world_inters_list = malloc(sizeof(t_inters) * arr2->count);
		i = 0;
		while (i < arr2->count)
		{
			arr1->world_inters_list[i] = arr2->inters_list[i];
			i++;
		}
		return ;
	}
	s_intersect_concatenate_1(arr1, arr2);
}

static void	s_intersect_world_helper(t_object *object,
	t_stack_ray ray, t_stack_intersections *tmp1)
{
	if (object->type == T_CYLINDRE)
		cylinder_intersct(tmp1, object->obj->cylinder, &ray);
	else if (object->type == T_SPHERE)
		s_intersect(tmp1, ray, object->obj->sphere);
	else if (object->type == T_CONE)
		cone_intersct(tmp1, object->obj->cone, &ray);
	else if (object->type == T_PLAN)
		plane_intersct(tmp1, object->obj->plane, &ray);
}

void	s_intersect_world(t_stack_intersections *in,
	t_world world, t_stack_ray ray)
{
	t_stack_intersections	tmp;
	t_stack_intersections	tmp1;
	int						i;

	tmp = (t_stack_intersections){false, 0, (t_inters){0}};
	tmp1 = (t_stack_intersections){false, 0, (t_inters){0}};
	i = 0;
	while (i < world.obj_num)
	{
		s_intersect_world_helper(world.objects[i], ray, &tmp1);
		if (tmp1.existence)
		{
			s_intersect_concatenate(&tmp, &tmp1);
			tmp.existence = true;
		}
		i++;
	}
	sort_intersctions(&tmp);
	in->count = tmp.count;
	in->world_inters_list = tmp.world_inters_list;
	in->existence = tmp.existence;
	return ;
}
