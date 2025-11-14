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
t_vec3 sample_color(t_texture *tex, double u, double v) 
{
	if (!tex)
		return (t_vec3){1.0, 1.0, 1.0};

	if (tex->type == 1)
	{
		double freq = 1.0 / tex->scale;
		u = fmod(u, 1.0);
		v = fmod(v, 1.0);
		if (u < 0.0)
			u += 1.0;
		if (v < 0.0)
			v += 1.0;
		int iu;
		int iv;
		iu = (int)(u * freq);
		iv = (int)(v * freq);

		if ((iu + iv) % 2 == 0)
		{
			// puts("-------black\n");
			return (t_vec3){0.0, 0.0, 0.0};  // Pure black
		}
		else
		{
			// puts("-------white\n");
			return (t_vec3){1.0, 1.0, 1.0};  // Pure white
		}
	} 
	else if (tex->type == 2 && tex->image)
	{
		u = fmod(u, 1.0);
		v = fmod(v, 1.0);
		if (u < 0.0)
			u += 1.0;
		if (v < 0.0)
			v += 1.0;

		int x = (int)(u * (tex->image->width - 1));
		int y = (int)((1.0 - v) * (tex->image->height - 1));  // Flip V

		x = fmax(0, fmin(x, (int)tex->image->width - 1));
		y = fmax(0, fmin(y, (int)tex->image->height - 1));

		uint8_t *pixel = &tex->image->pixels[(y * tex->image->width + x) * 4];
		
		return (t_vec3){
			pixel[0] / 255.0,
			pixel[1] / 255.0,
			pixel[2] / 255.0
		};
	}
	
	return (t_vec3){1.0, 1.0, 1.0};
}
