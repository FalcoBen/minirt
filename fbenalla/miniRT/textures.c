/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 23:30:57 by fbenalla          #+#    #+#             */
/*   Updated: 2025/11/13 23:32:15 by fbenalla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include  <math.h>

t_texture_color	sampling_checker(double *u, double *v, t_texture *tex)
{
	double	freq;
	int		iu;
	int		iv;

	freq = 1.0 / tex->scale;
	*u = fmod(*u, 1.0);
	*v = fmod(*v, 1.0);
	if (*u < 0.0)
		*u += 1.0;
	if (*v < 0.0)
		*v += 1.0;
	iu = (int)(*u * freq);
	iv = (int)(*v * freq);
	if ((iu + iv) % 2 == 0)
		return ((t_texture_color){0.0, 0.0, 0.0});
	else
		return ((t_texture_color){1.0, 1.0, 1.0});
}

t_texture_color	sampling_texture(double *u, double *v, t_texture *tex)
{
	int		x;
	int		y;
	uint8_t	*pixel;

	*u = fmod(*u, 1.0);
	*v = fmod(*v, 1.0);
	if (*u < 0.0)
		*u += 1.0;
	if (*v < 0.0)
		*v += 1.0;
	x = (int)(*u * (tex->image->width - 1));
	y = (int)((1.0 - *v) * (tex->image->height - 1));
	x = fmax(0, fmin(x, (int)tex->image->width - 1));
	y = fmax(0, fmin(y, (int)tex->image->height - 1));
	pixel = &tex->image->pixels[(y * tex->image->width + x) * 4];
	return ((t_texture_color){
		pixel[0] / 255.0,
		pixel[1] / 255.0,
		pixel[2] / 255.0
	});
}

t_texture_color	sample_color(t_texture *tex, double u, double v)
{
	if (!tex)
		return ((t_texture_color){1.0, 1.0, 1.0});
	if (tex->type == 1)
		return (sampling_checker(&u, &v, tex));
	else if (tex->type == 2 && tex->image)
		return (sampling_texture(&u, &v, tex));
	return ((t_texture_color){1.0, 1.0, 1.0});
}
