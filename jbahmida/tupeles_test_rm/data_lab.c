#include "../../MiniRt.h"
// typedef enum e_type
// {
// 	T_SPHER,
// 	T_CYLINDRE,
// 	T_PLAN
// }t_type;

// // typedef struct s_sphere{
// // 	int s1;
// // 	int s2;
// // 	int s3;
// // }t_sphere;

// typedef struct s_cylinder{
// 	int c1;
// 	int c2;
// 	int c3;
// }t_cylinder;

// typedef struct s_plan{
// 	int p1;
// 	int p2;
// 	int p3;
// }t_plan;



// int main()
// {
// 	int n = 4;
// 	t_world *world = malloc(sizeof(t_world));
// 	world->obj_num = n;
// 	world->objects = malloc(sizeof(t_object) * n);
// 	for(int i = 0; i < n; i++)
// 	{
// 		world->objects[i] = malloc(sizeof(t_object));
// 		world->objects[i]->obj = malloc(sizeof(t_obj));
// 	}
// 	sphere(&world->objects[0]->obj->sphere, NULL, false);
// 	sphere(&world->objects[1]->obj->sphere, NULL, false);
// 	sphere(&world->objects[2]->obj->sphere, NULL, false);
// 	sphere(&world->objects[3]->obj->sphere, NULL, false);
// 	printf("%d\n", world->objects[0]->obj->sphere->id);
// 	printf("%d\n", world->objects[2]->obj->sphere->id);
// }

t_tuple c_tuple1(ld *coor)
{
	t_tuple local = {0, 0, 0, 0};
	if (!coor)
	{
		return local;
	}
	local.x = coor[0];
	local.y = coor[1];
	local.z= coor[2];
	local.w= 1;
	return local;
}
t_tuple c_tuple(ld *coor)
{
	int xx = 5;
	return (c_tuple1(coor));
}
// int main()
// {
// 	ld coor[3] = {1, 2, 3};
// 	t_tuple p = c_tuple(coor);
// 	if (((t_tuple *)&p))
// 		printf("%Lf, %Lf, %Lf, %Lf\n", p.x, p.y, p.z, p.w);
// }