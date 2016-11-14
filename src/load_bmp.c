/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_bmp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcargou <rcargou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/12 17:25:40 by rcargou           #+#    #+#             */
/*   Updated: 2015/10/10 11:18:49 by rcargou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "run.h"

void load_bmp(char *imagepath, int fd, t_env *env, int index)
{
	unsigned char	header[54];
	unsigned int	size[3];
	unsigned char	*data;

	if ((fd = open(imagepath, O_RDONLY)) < 0 ||
		ft_strcmp(ft_strchr(imagepath, '.'), ".bmp"))
		exit(0);
	read(fd, header, 54);
	size[0] = *(int*)&(header[0x22]);
	size[1] = *(int*)&(header[0x12]);
	size[2] = *(int*)&(header[0x16]);
	data = malloc(size[0] * sizeof(unsigned char));
	read(fd, data, size[0]);
	glGenTextures(1, &(env->textIDs[index]));
	glBindTexture(GL_TEXTURE_2D, (env->textIDs[index]));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		size[1], size[2], 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
