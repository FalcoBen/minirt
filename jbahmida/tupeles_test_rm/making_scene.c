#include "../../MiniRt.h"

// t_world *world()
// {
//     t_world *w;
//     ld p_coor[3] = {-10, 10, -10};
//     ld light_color_coor[3] = {1, 1, 1};
//     t_color *light_color = create_color(light_color_coor);
//     t_tuple *light_position = create_point(p_coor);
//     ld s1_coor[4] = {0, 0.7, 0.2, 0};
//     ld color_coor[3] = {0.8, 1.0, 0.6};
//     t_color *s1_color = create_color(color_coor);
//     t_matrix *m = scaling(0.5, 0.5, 0.5);
//     w = alloc(sizeof(t_world), false);

//     w->light = point_light(light_color, light_position);
//     w->obj_num = 2;
//     w->objects = alloc(sizeof(t_object *) * 3, false);
//     int i = 0;
//     while (i < 2)
//     {
//         w->objects[i] = alloc(sizeof(t_object), false);
//         w->objects[i]->type = T_SPHERE;
//         w->objects[i]->obj = alloc(sizeof(t_obj), false);
//         sphere(&w->objects[i]->obj->sphere, NULL, false); 
//         i++;
//     }
//     w->objects[i] = NULL;
//     (w->objects[0]->obj->sphere)->material = material(s1_coor, s1_color);
//     (w->objects[1]->obj->sphere)->material = material(s1_coor, s1_color);
//     sphere(&(w->objects[1]->obj->sphere), m, true);

// //     dprintf(g_fd, "world_material s1 : {%Lf, %Lf, %Lf}\n", (w->objects[0]->obj->sphere)->material->color->r,
// // (w->objects[0]->obj->sphere)->material->color->g, (w->objects[0]->obj->sphere)->material->color->b);
// //     dprintf(g_fd, "world_material s2 : {%Lf, %Lf, %Lf}\n", (w->objects[1]->obj->sphere)->material->color->r,
// // (w->objects[1]->obj->sphere)->material->color->g, (w->objects[1]->obj->sphere)->material->color->b);
// //     dprintf(g_fd, "********************************************\n");
//     return w;
// }

// t_intersections *intersect_concatenate(t_intersections *arr1, t_intersections *arr2)
// {
//     t_intersections *new;

//     if (!arr1 || !arr2 || !arr1->inters_list || !arr2->inters_list)
//         return NULL;
//     if (!*arr1->inters_list && *arr2->inters_list)
//         return arr2;
//     else if (!*arr2->inters_list && *arr1->inters_list)
//         return arr1;
//     else if (!*arr2->inters_list && !*arr1->inters_list)
//         return NULL;
//     new = alloc(sizeof(t_intersections), false);
//     new->count = arr1->count + arr2->count;
//     new->inters_list = alloc(sizeof(t_inters *) * (new->count + 1), false);

//     int i = 0, j = 0;
//     while (j < arr1->count)
//         new->inters_list[i++] = arr1->inters_list[j++];
//     j = 0;
//     while (j < arr2->count)
//         new->inters_list[i++] = arr2->inters_list[j++];

//     new->inters_list[i] = NULL;
//     return new;
// }

// void sort_intersctions(t_intersections **inters)
// {
//     t_intersections *list;
//     t_inters *tmp;
//     if(!inters || !*inters)
//         return ;
//     list = *inters;
//     for(int i = 0; i < list->count; i++)
//     {
//         for(int j = 0; j < list->count - i - 1; j++)
//         {
//             if ( list->inters_list[j+1]
//                 && list->inters_list[j]->inters_value > list->inters_list[j+1]->inters_value)
//             {
//                 tmp = list->inters_list[j];
//                 list->inters_list[j] = list->inters_list[j+1];
//                 list->inters_list[j+1] = tmp;
//             }
//         }
//     }
// }

// t_intersections *intersect_world(t_world *world, t_ray *ray)
// {

//     t_intersections *inters = NULL;
//     t_intersections *tmp = NULL;
//     for(int i = 0; i < world->obj_num; i++)
//     {
//         tmp = intersect(ray, world->objects[i]->obj->sphere);
//         if (!inters)
//             inters = tmp;
//         else
//             inters = intersect_concatenate(inters, tmp);
//     }
//     sort_intersctions(&inters);
//     return inters;
// }

// When comps ← prepare_computations(i, r)
// Then comps.t = i.t
// And comps.object = i.object
// And comps.point = point(0, 0, -1)
// And comps.eyev = vector(0, 0, -1)
// And comps.normalv = vector(0, 0, -1)
// comps.point ← position(ray, comps.t)
// comps.eyev ← -ray.direction
// comps.normalv ← normal_at(comps.object, comps.point)

// if dot(comps.normalv, comps.eyev) < 0
// comps.inside ← true
// comps.normalv ← -comps.normalv
// else
// comps.inside ← false
// end if


// t_comp *prepare_computations(t_inters *intersection, t_ray *ray)
// {
//     t_comp *new;
//     if (!intersection || !ray)
//         return NULL;
//     new = alloc(sizeof(t_comp), false);
//     new->obj = intersection->object;
//     new->intersection = intersection;
//     new->point = *position(ray, intersection->inters_value);
//     new->eyev = *neg_t(&ray->vect);
//     new->normalv = *normal_at(intersection->object, &new->point);
//     if (dot_product(&new->normalv, &new->eyev))
//     {
//         new->inside = true;
//         new->normalv = *neg_t(&new->normalv);
//     }
//     else
//         new->inside = false;
//     return new;
// }

// t_color *shade_hit(t_world *world, t_comp *comp)
// {
// //     dprintf(g_fd, "********************************************\n");
// //     dprintf(g_fd, "form shade_hit : {%Lf, %Lf, %Lf}\n", (world->objects[0]->obj->sphere)->material->color->r,
// // (world->objects[0]->obj->sphere)->material->color->g, (world->objects[0]->obj->sphere)->material->color->b);
// //     dprintf(g_fd, "from shade_hit : {%Lf, %Lf, %Lf}\n", (world->objects[1]->obj->sphere)->material->color->r,
// // (world->objects[1]->obj->sphere)->material->color->g, (world->objects[1]->obj->sphere)->material->color->b);
// //     dprintf(g_fd, "********************************************\n");
//     return (lighting(world->objects[0]->obj->sphere->material, world->light, &comp->point, &comp->eyev, &comp->normalv));
// }
// t_color *color_at(t_world *w, t_ray *ray)
// {
//     // dprintf(g_fd, "********************************************\n\n\n");
//     // dprintf(g_fd, "color_at befor intersctions: world_material s1 : {%Lf, %Lf, %Lf}\n", (w->objects[0]->obj->sphere)->material->color->r,
//     // (w->objects[0]->obj->sphere)->material->color->g, (w->objects[0]->obj->sphere)->material->color->b);
//     // dprintf(g_fd, "color_at befor intersction: world_material s2 : {%Lf, %Lf, %Lf}\n", (w->objects[1]->obj->sphere)->material->color->r,
//     // (w->objects[1]->obj->sphere)->material->color->g, (w->objects[1]->obj->sphere)->material->color->b);
//     // dprintf(g_fd, "********************************************\n\n\n");
//     t_intersections *xs = intersect_world(w, ray);
//     t_inters *h = hit(xs);
//     if (h)
//     {
//         t_comp *comp = prepare_computations(h, ray);
//         // dprintf(g_fd, "********************************************\n\n\n");
//         // dprintf(g_fd, "color_at: world_material s1 : {%Lf, %Lf, %Lf}\n", (w->objects[0]->obj->sphere)->material->color->r,
//         // (w->objects[0]->obj->sphere)->material->color->g, (w->objects[0]->obj->sphere)->material->color->b);
//         // dprintf(g_fd, "color_at: world_material s2 : {%Lf, %Lf, %Lf}\n", (w->objects[1]->obj->sphere)->material->color->r,
//         // (w->objects[1]->obj->sphere)->material->color->g, (w->objects[1]->obj->sphere)->material->color->b);
//         // dprintf(g_fd, "********************************************\n\n\n");
//         return (shade_hit(w, comp));
//     }
//     else
//     {
//         ld default_color[3] = {0,0,0}; 
//         return (create_color(default_color));
//     }
// }

// int main()
// {
//     struct rlimit rl;
    
//     /* Limit virtual memory (heap + mmap etc.) to 1 GiB */
//     rl.rlim_cur = rl.rlim_max = (rlim_t)2 << 30;  // 1 GiB
//     if (setrlimit(RLIMIT_AS, &rl) != 0) {
//         perror("setrlimit RLIMIT_AS");
//     }
    
//     /* Limit stack size to 1 GiB */
//     rl.rlim_cur = rl.rlim_max = (rlim_t)1 << 30;  // 1 GiB
//     if (setrlimit(RLIMIT_STACK, &rl) != 0) {
//         perror("setrlimit RLIMIT_STACK");
//     }
    
//     int fd = open("color_at.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
//     if (fd < 0) {
//         perror("open failed");
//         exit(1);
//     }
//     // int fd = open("wold.txt", O_CREAT | 0644);
//     ld coor_origin[3] = {0, 0, -5};
//     ld coor_vec[3] = {0, 0, 1};
//     t_tuple *origin = create_point(coor_origin);
//     t_tuple *vec = create_vector(coor_vec);
//     t_ray *ray = ray_constrctor(origin, vec);
//     t_world *w = world();
//     // t_sphere *s = NULL;
//     // sphere(&s, NULL, NULL);
//     // t_inters *inters = intersection(4, s);
// //     dprintf(g_fd, "********************************************\n");
// //      dprintf(g_fd, "MAIN: world_material s1 : {%Lf, %Lf, %Lf}\n", (w->objects[0]->obj->sphere)->material->color->r,
// // (w->objects[0]->obj->sphere)->material->color->g, (w->objects[0]->obj->sphere)->material->color->b);
// //     dprintf(g_fd, "MAIN: world_material s2 : {%Lf, %Lf, %Lf}\n", (w->objects[1]->obj->sphere)->material->color->r,
// // (w->objects[1]->obj->sphere)->material->color->g, (w->objects[1]->obj->sphere)->material->color->b);
// //     dprintf(g_fd, "********************************************\n");
//     t_color *color = color_at(w, ray);

//     // t_intersections *xs;
//     // xs = intersect_world(w, ray);
//     // t_comp *comp = prepare_computations(inters, ray);
//     // printf("spher_orige ={%Lf, %Lf, %Lf}, spher_raduis= %Lf, sphere_id= %d\n",
//     // comp->obj->origin->x, comp->obj->origin->y, comp->obj->origin->z, comp->obj->radius, comp->obj->id);
//     printf("%Lf, %Lf, %Lf\n", color->r, color->g, color->b);
// }