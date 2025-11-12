#include "../../MiniRt.h"
int g_fd;
void sort_intersctions(t_stack_intersections *inters)
{
    t_stack_intersections *list;
    t_inters tmp;
    if (!inters)
        return;
    list = inters;

    for (int i = 0; i < list->count; i++)
    {
        for (int j = 0; j < list->count - i - 1; j++)
        {
            if (list->world_inters_list[j].inters_value > list->world_inters_list[j + 1].inters_value)
            {
                tmp = list->world_inters_list[j];
                list->world_inters_list[j] = list->world_inters_list[j + 1];
                list->world_inters_list[j + 1] = tmp;
            }
        }
    }
}

void s_intersect_concatenate(t_stack_intersections *arr1, t_stack_intersections *arr2)
{
    t_inters *tmp;
    if (!arr2->count)
        return;
    if (!arr1->count)
    {
        arr1->count = arr2->count;
        arr1->world_inters_list = malloc(sizeof(t_inters) * arr2->count);
        for (int i = 0; i < arr2->count; i++)
            arr1->world_inters_list[i] = arr2->inters_list[i];
        return;
    }
    int count = arr1->count + arr2->count;
    tmp = arr1->world_inters_list;
    arr1->world_inters_list = malloc(sizeof(t_inters) * count);
    int i = 0;
    int j = 0;
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
    free(tmp);
    arr1->count = count;
}

void s_intersect_world(t_stack_intersections *in, t_world world, t_stack_ray ray)
{

    // t_stack_intersections tmp = (t_stack_intersections){false, 0, (t_inters){0, 0}, NULL};
    // t_stack_intersections tmp1 = (t_stack_intersections){false, 0, (t_inters){0, 0}, NULL};
    t_stack_intersections tmp;
    t_stack_intersections tmp1;
    tmp.count = tmp1.count = 0;
    tmp.existence = tmp1.existence = false;
    tmp.world_inters_list = tmp1.world_inters_list = NULL;
    for (int i = 0; i < world.obj_num; i++)
    {
        if (world.objects[i]->type == T_CYLINDRE)
            cylinder_intersct(&tmp1, world.objects[i]->obj->cylinder, &ray);
        else if (world.objects[i]->type == T_SPHERE)
            s_intersect(&tmp1, ray, world.objects[i]->obj->sphere);
        else if (world.objects[i]->type == T_CONE)
            cone_intersct(&tmp1, world.objects[i]->obj->cone, &ray);
        else if (world.objects[i]->type == T_PLAN)
            plane_intersct(&tmp1, world.objects[i]->obj->plane, &ray);
        if (tmp1.existence)
        {
            s_intersect_concatenate(&tmp, &tmp1);
            tmp.existence = true;
        }
    }
    sort_intersctions(&tmp);
    in->count = tmp.count;
    in->world_inters_list = tmp.world_inters_list;
    in->existence = tmp.existence;
    return;
}

// void s_intersect_world(t_stack_intersections *in, t_world world, t_stack_ray ray)
// {
//     t_stack_intersections tmp = {false, 0, (t_inters){0, 0}, NULL};

//     for(int i = 0; i < world.obj_num; i++)
//     {
//         t_stack_intersections tmp1 = {false, 0, (t_inters){0, 0}, NULL};
//         s_intersect(&tmp1, ray, world.objects[i]->obj->sphere);
//         if (tmp1.existence)
//         {
//             s_intersect_concatenate(&tmp, &tmp1);
//             tmp.existence = true;
//             // Free the temporary intersections after concatenation
//             free(tmp1.world_inters_list);
//         }
//     }

//     sort_intersctions(&tmp);
//     in->count = tmp.count;
//     in->world_inters_list = tmp.world_inters_list;  // Transfer ownership
//     in->existence = tmp.existence;
// }

void copy_matrix_contant(t_matrix *m)
{
    if (!m || !m->matrix)
        return;
    for (int i = 0; i < m->row; i++)
        for (int j = 0; j < m->col; j++)
            m->stack_matrix[i][j] = m->matrix[i][j];
    return;
}

t_color s_create_color(ld *vals)
{
    t_color color;

    color = (t_color){0, 0, 0};
    color.r = vals[0];
    color.g = vals[1];
    color.b = vals[2];
    return color;
}

void print_matrix(t_matrix *m, const char *name)
{
    printf("%s:\n", name);
    for (int i = 0; i < m->row - 1; i++)
    {
        for (int j = 0; j < m->col - 1; j++)
        {
                printf("%f ", m->matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// t_world s_world()
// {
//     t_world world;

//     world.light = point_light(create_color((ld[3]){1, 1, 1}),
//                               create_point((ld[3]){-10, 10, -10}));
//     world.obj_num = 2;  // Plane + one object
//     world.objects = alloc(sizeof(t_object *) * world.obj_num, false);

//     // Initialize objects
//     for (int i = 0; i < world.obj_num; i++)
//     {
//         world.objects[i] = alloc(sizeof(t_object), false);
//         world.objects[i]->obj = alloc(sizeof(t_obj), false);
//     }

//     /* Floor Plane (Object 0) */
//     world.objects[0]->type = T_PLAN;
//     world.objects[0]->obj->plane = alloc(sizeof(t_plane), false);
//     plane(&world.objects[0]->obj->plane, NULL, false);

//     // Optional: Transform the plane if needed (e.g., rotate or translate)
//     // For a floor, identity matrix is fine (plane at y=0)
//     t_matrix *plane_transform = identity_matrix(4, 4);
//     plane(&world.objects[0]->obj->plane, plane_transform, true);

//     world.objects[0]->obj->plane->material = material();
//     world.objects[0]->obj->plane->material->color = create_color((ld[3]){1, 0.9, 0.9});
//     world.objects[0]->obj->plane->material->specular = 0;

//     /* Red Cylinder (Object 1) - Standing on the plane */
//     world.objects[1]->type = T_CYLINDRE;
//     world.objects[1]->obj->cylinder = alloc(sizeof(t_cylinder), false);
//     cylinder(&world.objects[1]->obj->cylinder, NULL, false);

//     // Cylinder from y=0 to y=2, sitting on the plane
//     world.objects[1]->obj->cylinder->minimum = 0;
//     world.objects[1]->obj->cylinder->maximum = 2;
//     world.objects[1]->obj->cylinder->closed = true;

//     // Scale it smaller and position it
//     t_matrix *cyl_scale = scaling(0.5, 1, 0.5);
//     t_matrix *cyl_trans = translation(0, 0, 0);
//     t_matrix *cyl_transform = matrix_multi(cyl_trans, cyl_scale);
//     cylinder(&world.objects[1]->obj->cylinder, cyl_transform, true);

//     world.objects[1]->obj->cylinder->material = material();
//     world.objects[1]->obj->cylinder->material->color = create_color((ld[3]){1, 0, 0}); // Red
//     world.objects[1]->obj->cylinder->material->diffuse = 0.7;
//     world.objects[1]->obj->cylinder->material->specular = 0.3;

//     /* Copy inverse matrices */
//     copy_matrix_contant(world.objects[0]->obj->plane->inv);
//     copy_matrix_contant(world.objects[0]->obj->plane->transform);
//     copy_matrix_contant(world.objects[1]->obj->cylinder->inv);
//     copy_matrix_contant(world.objects[1]->obj->cylinder->transform);

//     return world;
// }

// t_world s_world()
// {
//     t_world world;

//     world.light = point_light(create_color((ld[3]){1, 1, 1}), create_point((ld[3]){-10, 10, -10}));
//     world.obj_num = 1;  // Just one cylinder
//     world.objects = alloc(sizeof(t_object *) * world.obj_num, false);

//     // Initialize the object
//     world.objects[0] = alloc(sizeof(t_object), false);
//     world.objects[0]->obj = alloc(sizeof(t_obj), false);

//     /* Red Cylinder (Object 0) - Closed, truncated cylinder */
//     world.objects[0]->type = T_CYLINDRE;
//     world.objects[0]->obj->cylinder = alloc(sizeof(t_cylinder), false);
//     cylinder(&world.objects[0]->obj->cylinder, NULL, false);

//     // Set cylinder bounds and close it
//     world.objects[0]->obj->cylinder->minimum = -1;
//     world.objects[0]->obj->cylinder->maximum = 1;
//     world.objects[0]->obj->cylinder->closed = true;

//     // Optional: You can add transformations if needed
//     // For now, keep it at origin with identity transform
//     t_matrix *cyl_transform = identity_matrix(4, 4);
//     cylinder(&world.objects[0]->obj->cylinder, cyl_transform, true);

//     world.objects[0]->obj->cylinder->material = material();
//     world.objects[0]->obj->cylinder->material->color = create_color((ld[3]){1, 0, 0}); // Red
//     world.objects[0]->obj->cylinder->material->diffuse = 0.7;
//     world.objects[0]->obj->cylinder->material->specular = 0.3;

//     /* Copy inverse matrices */
//     copy_matrix_contant(world.objects[0]->obj->cylinder->inv);
//     copy_matrix_contant(world.objects[0]->obj->cylinder->transform);

//     return world;
// }
// The default world contains two spheres, s1 (outer sphere) and s2 (inner sphere).
// Sphere 1 (s1) Parameters (Outer Sphere)
// • Color: color(0.8, 1.0, 0.6).
// • Material Diffuse Component: 0.7.
// • Material Specular Component: 0.2.
// • Size/Position: This is a unit sphere (radius 1) lying at the origin, as it is created with s1 ← sphere() and has no transform explicitly set in the definition.
// t_world s_world()
// {
//     t_world world;
//     world.light = point_light(create_color((ld[3]){1, 1, 1}), create_point((ld[3]){-10, 10, -10}));
//     world.obj_num = 7;
//     world.objects = alloc(sizeof(t_object *) * world.obj_num, false);

//     // Initialize all objects
//     for (int i = 0; i < world.obj_num; i++)
//     {
//         world.objects[i] = alloc(sizeof(t_object), false);
//         world.objects[i]->obj = alloc(sizeof(t_obj), false);
//     }

    /* Floor (Plane 0) */
    // world.objects[0]->type = T_PLAN;
    // world.objects[0]->obj->plane = alloc(sizeof(t_plane), false);
    // plane(&world.objects[0]->obj->plane, NULL, false);
    // // Floor at y=0 (identity transform)
    // plane(&world.objects[0]->obj->plane, identity_matrix(4, 4), true);
    // world.objects[0]->obj->plane->material = material();
    // world.objects[0]->obj->plane->material->color = create_color((ld[3]){1, 0.9, 0.9});
    // world.objects[0]->obj->plane->material->specular = 0;

//     /* Left Wall (Plane 1) */
//     world.objects[1]->type = T_PLAN;
//     world.objects[1]->obj->plane = alloc(sizeof(t_plane), false);
//     plane(&world.objects[1]->obj->plane, NULL, false);
//     // Rotate 90° around X, then -45° around Y, then translate to z=5
//     t_matrix *rx1 = rotation_x(M_PI/2);
//     t_matrix *ry1 = rotation_y(-M_PI/4);
//     t_matrix *trans1 = translation(0, 0, 5);
//     t_matrix *tmp1 = matrix_multi(ry1, rx1);
//     t_matrix *tran1 = matrix_multi(trans1, tmp1);
//     plane(&world.objects[1]->obj->plane, tran1, true);
//     world.objects[1]->obj->plane->material = material();
//     world.objects[1]->obj->plane->material->color = create_color((ld[3]){1, 0.9, 0.9});
//     world.objects[1]->obj->plane->material->specular = 0;

//     /* Right Wall (Plane 2) */
//     world.objects[2]->type = T_PLAN;
//     world.objects[2]->obj->plane = alloc(sizeof(t_plane), false);
//     plane(&world.objects[2]->obj->plane, NULL, false);
//     // Rotate 90° around X, then 45° around Y, then translate to z=5
//     t_matrix *rx2 = rotation_x(M_PI/2);
//     t_matrix *ry2 = rotation_y(M_PI/4);
//     t_matrix *trans2 = translation(0, 0, 5);
//     t_matrix *tmp2 = matrix_multi(ry2, rx2);
//     t_matrix *tran2 = matrix_multi(trans2, tmp2);
//     plane(&world.objects[2]->obj->plane, tran2, true);
//     world.objects[2]->obj->plane->material = material();
//     world.objects[2]->obj->plane->material->color = create_color((ld[3]){1, 0.9, 0.9});
//     world.objects[2]->obj->plane->material->specular = 0;

//     /* Middle Sphere (Sphere 3) */
//     world.objects[3]->type = T_SPHERE;
//     world.objects[3]->obj->sphere = alloc(sizeof(t_sphere), false);
//     t_matrix *k = matrix_multi(scaling(2,2,2), translation(0,-0.5,0));
//     sphere(&world.objects[3]->obj->sphere, NULL, false);
//     sphere(&world.objects[3]->obj->sphere, k, true);

//     world.objects[3]->obj->sphere->material = material();
//     world.objects[3]->obj->sphere->material->color = create_color((ld[3]){0.1, 1, 0.5});
//     world.objects[3]->obj->sphere->material->diffuse = 0.7;
//     world.objects[3]->obj->sphere->material->specular = 0.3;

//     /* Right Sphere (Sphere 4) */
//     world.objects[4]->type = T_SPHERE;
//     world.objects[4]->obj->sphere = alloc(sizeof(t_sphere), false);
//     t_matrix *scale_right = scaling(0.5, 0.5, 0.5);
//     t_matrix *trans_right = translation(1.5, 0.5, -0.5);
//     t_matrix *tran_right = matrix_multi(trans_right, scale_right);
//     sphere(&world.objects[4]->obj->sphere, NULL, false);
//     sphere(&world.objects[4]->obj->sphere, tran_right, true);
//     world.objects[4]->obj->sphere->material = material();
//     world.objects[4]->obj->sphere->material->color = create_color((ld[3]){0.5, 1, 0.1});
//     world.objects[4]->obj->sphere->material->diffuse = 0.7;
//     world.objects[4]->obj->sphere->material->specular = 0.3;

//     /* Left Sphere (Sphere 5) */
//     world.objects[5]->type = T_SPHERE;
//     world.objects[5]->obj->sphere = alloc(sizeof(t_sphere), false);
//     t_matrix *scale_left = scaling(0.33, 0.33, 0.33);
//     t_matrix *trans_left = translation(-1.5, 0.33, -0.75);
//     t_matrix *tran_left = matrix_multi(trans_left, scale_left);
//     sphere(&world.objects[5]->obj->sphere, NULL, false);
//     sphere(&world.objects[5]->obj->sphere, tran_left, true);
//     world.objects[5]->obj->sphere->material = material();
//     world.objects[5]->obj->sphere->material->color = create_color((ld[3]){1, 0.8, 0.1});
//     world.objects[5]->obj->sphere->material->diffuse = 0.7;
//     world.objects[5]->obj->sphere->material->specular = 0.3;

//     /* Cylinder (Object 6) */
//     world.objects[6]->type = T_CYLINDRE;
//     world.objects[6]->obj->cylinder = alloc(sizeof(t_cylinder), false);
//     cylinder(&world.objects[6]->obj->cylinder, NULL, false);
//     world.objects[6]->obj->cylinder->minimum = -INFINITY;
//     world.objects[6]->obj->cylinder->maximum = INFINITY;
//     world.objects[6]->obj->cylinder->closed = true;
//     t_matrix *cyl_scale = scaling(0.5, 1, 0.5);
//     t_matrix *cyl_trans = translation(-1, 0, 1);
//     t_matrix *cyl_transform = matrix_multi(cyl_trans, cyl_scale);
//     cylinder(&world.objects[6]->obj->cylinder, cyl_transform, true);
//     world.objects[6]->obj->cylinder->material = material();
//     world.objects[6]->obj->cylinder->material->color = create_color((ld[3]){1, 0, 0});
//     world.objects[6]->obj->cylinder->material->diffuse = 0.7;
//     world.objects[6]->obj->cylinder->material->specular = 0.3;

//     /* Copy inverse matrices */
//     copy_matrix_contant(world.objects[0]->obj->plane->inv);
//     copy_matrix_contant(world.objects[0]->obj->plane->transform);
//     copy_matrix_contant(world.objects[1]->obj->plane->inv);
//     copy_matrix_contant(world.objects[1]->obj->plane->transform);
//     copy_matrix_contant(world.objects[2]->obj->plane->inv);
//     copy_matrix_contant(world.objects[2]->obj->plane->transform);
//     copy_matrix_contant(world.objects[3]->obj->sphere->inv);
//     copy_matrix_contant(world.objects[3]->obj->sphere->transform);
//     copy_matrix_contant(world.objects[4]->obj->sphere->inv);
//     copy_matrix_contant(world.objects[4]->obj->sphere->transform);
//     copy_matrix_contant(world.objects[5]->obj->sphere->inv);
//     copy_matrix_contant(world.objects[5]->obj->sphere->transform);
//     copy_matrix_contant(world.objects[6]->obj->cylinder->inv);
//     copy_matrix_contant(world.objects[6]->obj->cylinder->transform);

//     return world;
// }
// t_world s_world()
// {
//     t_world world;

//     // Light positioned above and to the side
//     world.light = point_light(create_color((ld[3]){1, 1, 1}),
//                               create_point((ld[3]){0, 10, -5}));

//     world.obj_num = 2;  // Floor plane + cone
//     world.objects = alloc(sizeof(t_object *) * world.obj_num, false);

//     // Initialize all objects
//     for (int i = 0; i < world.obj_num; i++)
//     {
//         world.objects[i] = alloc(sizeof(t_object), false);
//         world.objects[i]->obj = alloc(sizeof(t_obj), false);
//     }

//     /* Floor Plane (Object 0) - Blue */
//     world.objects[0]->type = T_PLAN;
//     world.objects[0]->obj->plane = alloc(sizeof(t_plane), false);
//     plane(&world.objects[0]->obj->plane, NULL, false);

//     // Floor at y=0
//     t_matrix *floor_transform = identity_matrix(4, 4);
//     plane(&world.objects[0]->obj->plane, floor_transform, true);

//     world.objects[0]->obj->plane->material = material();
//     world.objects[0]->obj->plane->material->color = create_color((ld[3]){0.2, 0.2, 0.8}); // Blue
//     world.objects[0]->obj->plane->material->diffuse = 0.8;
//     world.objects[0]->obj->plane->material->specular = 0.2;
//     world.objects[0]->obj->plane->material->ambient = 0.3;

//     /* Yellow Cone (Object 1) - Double-napped cone */
//     world.objects[1]->type = T_CONE;
//     world.objects[1]->obj->cone = alloc(sizeof(t_cone), false);
//     cone(&world.objects[1]->obj->cone, NULL, false);

//     // Cone extending from -2 to +2 (double-napped: both up and down)
//     world.objects[1]->obj->cone->minimum = -2;
//     world.objects[1]->obj->cone->maximum = 2;
//     world.objects[1]->obj->cone->closed = false;  // Open ends for double-napped effect

//     // Position cone slightly above floor, no scaling
//     t_matrix *cone_trans = translation(0, 0, 0);
//     cone(&world.objects[1]->obj->cone, cone_trans, true);

//     world.objects[1]->obj->cone->material = material();
//     world.objects[1]->obj->cone->material->color = create_color((ld[3]){0.9, 0.9, 0.2}); // Yellow
//     world.objects[1]->obj->cone->material->diffuse = 0.8;
//     world.objects[1]->obj->cone->material->specular = 0.6;
//     world.objects[1]->obj->cone->material->shininess = 100;
//     world.objects[1]->obj->cone->material->ambient = 0.1;

//     /* Copy inverse matrices */
//     copy_matrix_contant(world.objects[0]->obj->plane->inv);
//     copy_matrix_contant(world.objects[0]->obj->plane->transform);
//     copy_matrix_contant(world.objects[1]->obj->cone->inv);
//     copy_matrix_contant(world.objects[1]->obj->cone->transform);

//     return world;
// }

// t_world s_world()
// {
//     t_world world;

//     world.light = point_light(create_color((ld[3]){1, 1, 1}), create_point((ld[3]){-10, 10, -10}));
//     world.obj_num = 2;
//     world.objects = alloc(sizeof(t_object *) * world.obj_num, false);

//     for (int i = 0; i < world.obj_num; i++)
//     {
//         world.objects[i] = alloc(sizeof(t_object), false);
//         world.objects[i]->obj = alloc(sizeof(t_obj), false);
//         world.objects[i]->obj->sphere = alloc(sizeof(t_sphere), false);
//         world.objects[i]->obj->sphere->material = material();
//         world.objects[i]->type = T_SPHERE;
//     }

//     printf("=== CREATING SIMPLIFIED WORLD ===\n");

//     /* Simple floor - moved down and smaller */
//     printf("Creating floor sphere...\n");
//     sphere(&world.objects[0]->obj->sphere, NULL, false);
//     // Move floor down to y=-1 and make it smaller
//     t_matrix *floor_trans = translation(0, -1, 0);
//     t_matrix *floor_scale = scaling(5, 0.01, 5);  // Smaller scale
//     t_matrix *floor_transform = matrix_multi(floor_trans, floor_scale);

//     sphere(&world.objects[0]->obj->sphere, floor_transform, true);
//     world.objects[0]->obj->sphere->material->color = create_color((ld[3]){1, 0.9, 0.9});
//     world.objects[0]->obj->sphere->material->specular = 0;
//     world.objects[0]->obj->sphere->id = 0;
//     printf("Floor sphere created at (0, -1, 0) with scale (5, 0.01, 5)\n");

//     /* Simple sphere at origin - make it brighter red */
//     printf("Creating red sphere...\n");
//     sphere(&world.objects[1]->obj->sphere, NULL, false);
//     t_matrix *sphere_trans = translation(0, 0, 0);  // At origin now
//     sphere(&world.objects[1]->obj->sphere, sphere_trans, true);
//     world.objects[1]->obj->sphere->material->color = create_color((ld[3]){1, 0, 0});  // Bright red
//     world.objects[1]->obj->sphere->material->diffuse = 0.9;  // Higher diffuse
//     world.objects[1]->obj->sphere->material->specular = 0.1;  // Lower specular
//     world.objects[1]->obj->sphere->id = 1;
//     printf("Red sphere created at (0, 0, 0)\n");

//     copy_matrix_contant(world.objects[0]->obj->sphere->inv);
//     copy_matrix_contant(world.objects[0]->obj->sphere->transform);
//     copy_matrix_contant(world.objects[1]->obj->sphere->inv);
//     copy_matrix_contant(world.objects[1]->obj->sphere->transform);

//     return world;
// }

// Then c = color(0.38066, 0.47583, 0.2855)

t_color color_at(t_world *w, t_stack_ray *ray)
{

    t_color res;

    res = (t_color){0, 0, 0};
    t_stack_intersections xs;
    s_intersect_world(&xs, *w, *ray);
    t_inters h = s_hit(xs);
    free(xs.world_inters_list);
    if (h.existence)
    {
        // puts("hello\n");
        t_comp comp = prepare_computations(&h, ray);
        return (shade_hit(w, &comp));
    }
    else
        return (res);
}



// t_color color_at(t_world *w, t_stack_ray *ray)
// {
//     t_color res = (t_color){0,0,0};
//     t_stack_intersections xs;

//     // printf("=== Testing ray from (%f, %f, %f) direction (%f, %f, %f)\n",
//     //        ray->point.x, ray->point.y, ray->point.z,
//     //        ray->vect.x, ray->vect.y, ray->vect.z);

//     s_intersect_world(&xs, *w, *ray);

//     // printf("Found %d intersections\n", xs.count);
//     // for (int i = 0; i < xs.count; i++) {
//     //     printf("  Intersection %d: t=%f",
//     //            i, xs.world_inters_list[i].inters_value);
//     // }

//     t_inters h = s_hit(xs);
//     if (h.existence)
//     {
//         // printf("HIT at t=%f", h.inters_value);
//         t_comp comp = prepare_computations(&h, ray);
//         return (shade_hit(w, &comp));
//     }
//     else
//     {
//         // printf("NO HIT\n");
//         return (res);
//     }
// }

// t_color color_at(t_world *w, t_stack_ray *ray)
// {
//     t_color res = (t_color){0,0,0};
//     t_stack_intersections xs;

//     printf("=== RAY INTERSECTION DEBUG ===\n");
//     printf("Ray origin: (%f, %f, %f) direction: (%f, %f, %f)\n",
//            ray->point.x, ray->point.y, ray->point.z,
//            ray->vect.x, ray->vect.y, ray->vect.z);

//     s_intersect_world(&xs, *w, *ray);

//     printf("Found %d intersections\n", xs.count);
//     for (int i = 0; i < xs.count; i++) {
//         printf("  Intersection %d: t=%f, sphere_id=%d\n",
//                i, xs.world_inters_list[i].inters_value,
//                xs.world_inters_list[i].object->id);
//     }

//     t_inters h = s_hit(xs);
//     if (h.existence)
//     {
//         printf("HIT: t=%f, sphere_id=%d\n", h.inters_value, h.object->id);
//         t_comp comp = prepare_computations(&h, ray);
//         t_color result = shade_hit(w, &comp);
//         printf("Final color: (%f, %f, %f)\n", result.r, result.g, result.b);
//         free(xs.world_inters_list);  // Don't forget to free!
//         return result;
//     }
//     else
//     {
//         printf("NO HIT\n");
//         free(xs.world_inters_list);  // Don't forget to free!
//         return (res);
//     }
// }

t_canva s_canva_create(mlx_t *p, int w, int h)
{
    t_canva local;

    local = (t_canva){NULL, NULL, NULL, NULL};
    local.mlx = p;
    local.image = mlx_new_image(p, w, h);
    return local;
}
int combine_color(t_color color)
{
    color.r = (color.r > 1) ? 1 : (color.r < 0) ? 0
                                                : color.r;
    color.g = (color.g > 1) ? 1 : (color.g < 0) ? 0
                                                : color.g;
    color.b = (color.b > 1) ? 1 : (color.b < 0) ? 0
                                                : color.b;
    int c = ((int)(color.b * 255) << 16) | ((int)(color.g * 255) << 8) | ((int)(color.r * 255)) | (255 << 24);
    return c;
    // return (255 << 24) | ((int)(color.r * 255) << 16) | ((int)(color.g * 255) << 8) | (int)(color.b * 255);
}
// int combine_color(t_color color)
// {
//     color.r = (color.r > 1) ? 1 : (color.r < 0) ? 0 : color.r;
//     color.g = (color.g > 1) ? 1 : (color.g < 0) ? 0 : color.g;
//     color.b = (color.b > 1) ? 1 : (color.b < 0) ? 0 : color.b;

//     // FIXED: Swap R and B channels for MLX42
//     // MLX42 expects: (R << 24) | (G << 16) | (B << 8) | A
//     return ((int)(color.r * 255) << 24) |
//            ((int)(color.g * 255) << 16) |
//            ((int)(color.b * 255) << 8) |
//            255;
// }

// void render(t_camera *cam, t_world *world, t_canva *canva)
// {
//     for(ld y = 0; y < HEIGHT - 1; y+=1)
//     {
//         for(ld x = 0; x < WIDTH - 1; x+=1)
//         {
//             t_stack_ray ray = s_ray_for_camera(cam, x, y);
//             t_color color = color_at(world, &ray);
//             printf("%f, %f, %f\n", color.r, color.g, color.b);
//             int a = (y * WIDTH + x) * BPP;
//             printf("=====> %d\n", a);
//             *((int32_t *)(canva->image->pixels + a)) = combine_color(color);
//         }
//     }
//     printf("hello");
// }

// Point 1: (50, 50) (center of canvas, likely hits the middle sphere, not in shadow).
// Point 2: (75, 25) (offset, likely hits the floor or right sphere, possibly in shadow).
// Point 3: (10, 10) (corner, likely misses objects, hitting the background or a wall).

void write_pixel(void *c)
{
    t_canva *canva = (t_canva *)c;
    for (ld y = 0; y < canva->cam->vsize; y++)
    {
        for (ld x = 0; x < canva->cam->hsize; x++)
        {
            t_stack_ray ray = s_ray_for_camera(canva->cam, x, y);
            t_color color = color_at(canva->world, &ray);
            int a = (y * WIDTH + x) * BPP;
            // printf("===> %d\n", a);
            *((int32_t *)(canva->image->pixels + a)) = combine_color(color);
        }
    }
    printf("done\n");
}

t_tuple get_up_vector(t_tuple *orientation)
{
    t_tuple world_up = (t_tuple){0, 1, 0, 0};
    if (fabsf(orientation->x) < 0.001 && fabsf(orientation->z) < 0.001)
    {
        return (t_tuple){0, 0, 1, 0};
    }
    return world_up;
}

void    close_window(mlx_key_data_t keydata, void* param)
{
    if(keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
    {
        t_scene *scene;
        set_scene(scene, true);
        // exit_error("window", "closed", NULL);
    }
}


void jassim_mlx(t_scene *scene)
{
    mlx_t *mlx = mlx_init(WIDTH, HEIGHT, "MiniRT", true);
    if (!mlx)
    {
        printf("%s\n", mlx_strerror(mlx_errno));
        return;
    }
    
    t_canva canva = s_canva_create(mlx, WIDTH, HEIGHT);
    t_world w = s_world(scene);
    canva.world = &w;
    
    
    t_tuple cam_pos;
    cam_pos.x = scene->camera->coor_camera->x;
    cam_pos.y = scene->camera->coor_camera->y;
    cam_pos.z = scene->camera->coor_camera->z;
    cam_pos.w = 1;
    
    t_tuple cam_dir;
    cam_dir.x = scene->camera->vector_camera->x;
    cam_dir.y = scene->camera->vector_camera->y;
    cam_dir.z = scene->camera->vector_camera->z;
    cam_dir.w = 0;
    
    
    ld length = sqrtl(cam_dir.x * cam_dir.x + cam_dir.y * cam_dir.y + cam_dir.z * cam_dir.z);
    if (length > 0.0001)
    {
        cam_dir.x /= length;
        cam_dir.y /= length;
        cam_dir.z /= length;
    }

    t_tuple to;
    to.x = cam_pos.x + cam_dir.x;
    to.y = cam_pos.y + cam_dir.y;
    to.z = cam_pos.z + cam_dir.z;
    to.w = 1;
    
    t_tuple up = get_up_vector(&cam_dir);
    
    ld fov_radians = (scene->camera->angle_view * M_PI) / 180.0;
    
    t_matrix tran = view_transformation(&cam_pos, &to, &up);
    t_camera *cam = camera(WIDTH, HEIGHT, fov_radians, &tran);
    canva.cam = cam;
    
    printf("=== Starting render ===\n");
    
    write_pixel((void *)&canva);
    mlx_image_to_window(canva.mlx, canva.image, 0, 0);
    mlx_key_hook(mlx, &close_window, mlx);
    mlx_loop(mlx);
    mlx_terminate(mlx);
}
