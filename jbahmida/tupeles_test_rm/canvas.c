#include "../../MiniRt.h"


// typedef struct projectile {
//     t_tuple position;
//     t_tuple velocity;
// }pro;

// typedef struct environment {
//     t_tuple gravity;
//     t_tuple wind;
// }env;

// pro *tick(env *envi, pro *proj){
//     pro *new_proj;

//     if (!envi || !proj)
//         return (NULL);
//     new_proj = alloc(sizeof(pro), false);
//     new_proj->position = *(add_t(&proj->position, &proj->velocity));
//     new_proj->velocity = *(add_t(add_t(&envi->gravity, &envi->wind), &proj->velocity));
//     return (new_proj);
// }





/*
    X= x − 2/width
	​
    Y=height / 2 − y
*/


t_tuple *m_t_p(t_matrix *m)
{
    t_tuple *p;

    p = alloc(sizeof(t_tuple), false);
    p->x = m->matrix[0][0];
    p->y = m->matrix[1][0];
    p->y = m->matrix[2][0];
    p->w = 1;

    return p;
}

t_canva *create_canvas(mlx_t *p, unsigned long width, unsigned long height, )
{
    t_canva *canva;

    canva = alloc(sizeof(t_canva), false);
    canva->mlx = p;
    // canva->width = width;
    // canva->height = height;
    canva->image =  mlx_new_image(p, width, height);
    return canva;
}


int r(ld p, char c)
{
    if (c == 'x')
        return (p +(WIDTH / 2));
    else
        return ((HEIGHT / 2) - p);
}

// int t_x(ld x) { return (int)((x + 100) * (WIDTH-1) / 2); }
// int t_y(ld y) { return (int)((100 - y) * (HEIGHT-1) / 2); }
#define x_max 1
#define x_min -1
#define y_max 1
#define y_min -1

/*
[a, b] ===> [d, c]
map(x) = (x - a) (d - c) / (b - a)
map(y) = (y - a) (d - c) / (b - a)

[-100, 100] ===> [0, width]
[-100, 100] ===> [0, hieght]

map(x) = (x - x_min) * (-WIDTH) / (x_max - x_min);
map(y) = (y - y_min) * (-HEIGHT) / (y_max - y_min);

*/

// void draw_line(t_canva *canva, int x0, int y0, int x1, int y1, int color) {
//     int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
//     int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
//     int err = dx + dy, e2;

//     while (1) {
//         mlx_put_pixel(canva->image, x0, y0, color);
//         if (x0 == x1 && y0 == y1) break;
//         e2 = 2 * err;
//         if (e2 >= dy) { err += dy; x0 += sx; }
//         if (e2 <= dx) { err += dx; y0 += sy; }
//     }
// }
// void draw_square_wireframe(t_canva *canva, t_matrix *m, int color) {
//     int Ax = t_x(m->matrix[0][0]);
//     int Ay = t_y(m->matrix[1][0]);

//     int Bx = t_x(m->matrix[0][1]);
//     int By = t_y(m->matrix[1][1]);

//     int Cx = t_x(m->matrix[0][2]);
//     int Cy = t_y(m->matrix[1][2]);

//     int Dx = t_x(m->matrix[0][3]);
//     int Dy = t_y(m->matrix[1][3]);

//     draw_line(canva, Ax, Ay, Bx, By, color);
//     draw_line(canva, Bx, By, Cx, Cy, color);
//     draw_line(canva, Cx, Cy, Dx, Dy, color);
//     draw_line(canva, Dx, Dy, Ax, Ay, color);
// }

int t_x(ld x) {
    return (int)(((x + 300) * WIDTH) / 600);
}

int t_y(ld y) {
    return (int)(((300 - y) * HEIGHT) / 600);
}
typedef struct t_c {

    double x;
    double y;
    double r;
}t_cer;


void write_pixel(void *ptr)
{
    t_color color = {215,0,0};
    t_canva *canva = (t_canva *)ptr;
    // memset(canva->image->pixels, 255,   WIDTH * HEIGHT * sizeof(int32_t));

    double pixel_ratio_x = 600/WIDTH;
    double pixel_ratio_y = 600/HEIGHT;
    t_cer cerc = {0, 0 , 100};
    double radius_s = pow(cerc.r, 2);
    for (double x = cerc.x - cerc.r; x <= cerc.x + cerc.r; x += 0.6)
    {
        for (double y = cerc.y - cerc.r; y <= cerc.y + cerc.r; y += 0.6)
        {
            double dist = pow(x - cerc.x, 2) + pow(y - cerc.y, 2);
            if (dist < radius_s)
                mlx_put_pixel(canva->image, t_x(x), t_y(y), combine_color(color));
        }
    }


}

// int main()
// {
//     mlx_t *mlx;
//     mlx = mlx_init(WIDTH, HEIGHT, "canavas", true);
//     if (!mlx)
//     {
//         printf("%s", mlx_strerror(mlx_errno));
 //         return (EXIT_FAILURE);
//     }
//     t_canva *canva = create_canvas(mlx, WIDTH, HEIGHT);
//     mlx_image_to_window(canva->mlx, canva->image, 0, 0);
//     mlx_loop_hook(mlx, write_pixel, (void *)canva);
//     mlx_loop(mlx);
// 	mlx_terminate(mlx);
// 	return (EXIT_SUCCESS);
// }