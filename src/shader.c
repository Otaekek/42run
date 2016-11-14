/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcargou <rcargou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/11 11:13:11 by rcargou           #+#    #+#             */
/*   Updated: 2015/10/05 07:25:51 by rcargou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "run.h"

char		*filetobuff(char *path)
{
	int		fd;
	int		len;
	char	*new;

	len = 0;
	new = malloc(10000);
	if ((fd = open(path, O_RDONLY)) < 0)
		exit(0);
	while (read(fd, new, 1))
		len++;
	close(fd);
	fd = open(path, O_RDONLY);
	read(fd, new, len);
	new[len] = 0;
	return (new);
}

GLuint		loadshaders(char *fragshader, char *vertexshader)
{
	GLuint		vshaderid;
	GLuint		fshaderid;
	GLuint		progid;
	char		*buff;

	vshaderid = glCreateShader(GL_VERTEX_SHADER);
	fshaderid = glCreateShader(GL_FRAGMENT_SHADER);
	buff = filetobuff(vertexshader);
	glShaderSource(vshaderid, 1, (const char *const *)(&buff), NULL);
	glCompileShader(vshaderid);


	int Result;
	int InfoLogLength;
	char FragmentShaderErrorMessage[1000];

	glGetShaderiv(vshaderid, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(vshaderid, GL_INFO_LOG_LENGTH, &InfoLogLength);
    glGetShaderInfoLog(vshaderid, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    printf("%s\n", FragmentShaderErrorMessage);

	buff = filetobuff(fragshader);
	glShaderSource(fshaderid, 1, (const char *const *)(&buff), NULL);
	glCompileShader(fshaderid);

    glGetShaderiv(fshaderid, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(fshaderid, GL_INFO_LOG_LENGTH, &InfoLogLength);
    glGetShaderInfoLog(fshaderid, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    printf("%s\n", FragmentShaderErrorMessage);

	progid = glCreateProgram();
	glAttachShader(progid, vshaderid);
	glAttachShader(progid, fshaderid);
	glLinkProgram(progid);
	return (progid);
}
