#include "../../MiniRt.h"

typedef struct projectile {
    t_tuple position;
    t_tuple velocity;
}pro;

typedef struct environment {
    t_tuple gravity;
    t_tuple wind;
}env;

pro *tick(env *envi, pro *proj){
    pro *new_proj;

    if (!envi || !proj)
        return (NULL);
    new_proj = alloc(sizeof(pro), false);
    new_proj->position = *(add_t(&proj->position, &proj->velocity));
    new_proj->velocity = *(add_t(add_t(&envi->gravity, &envi->wind), &proj->velocity));
    return (new_proj);
}

// int main(){
//     env envi;
//     pro proj;
//     t_tuple t1 = {0, 1, 0, 0};
//     t_tuple t2 = {1, 1, 0, 0};
//     t_tuple t3 = {0, -0.1, 0, 0};
//     t_tuple t4 = {0.01, 0, 0, 0};
//     proj.position = t2;
//     proj.velocity= *(vec_norm(&t2));
//     envi.gravity = t3;
//     envi.wind = t4;
//     printf("proj y position is : %Lf\n", proj.position.y);
//     for(; (proj.position.y > 0 && !ldbl_cmp(0, proj.position.y)); proj = *tick(&envi, &proj))
//         printf("proj y position is : %Lf\n", proj.position.y);
//     return 0;
// }

