/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_opengl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcargou <rcargou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/30 18:04:28 by rcargou           #+#    #+#             */
/*   Updated: 2015/10/10 11:17:40 by rcargou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "run.h"

t_point converge_cam(t_point a, t_point b, t_env *env)
{
	float n;

	n = (WALL_SIZE * 17.0f)/ (env->speed * 18);
	n = 17;
	a.x += (b.x - a.x) / n;
	a.y += (b.y- a.y) / n;
	a.z += (b.z- a.z) / n;
	if (rcm_vecnorme(rcm_vecsum(a, rcm_vecneg(b))) < 0.001f)
		a = b;
	return (a);
}

float *transform_to_matrix_cam(t_point pos, t_point dir, float zoom, t_env *env, int id)
{
    float *new;

	int rotate;

	rotate = 0;
	new = rcm_identitymat();
	new = rcm_transmat(pos.x + env->camera.dir.x * WALL_SIZE,
		pos.y + env->camera.dir.y * WALL_SIZE, pos.z + env->camera.dir.z * WALL_SIZE);
    new = rcm_transmat(pos.x + env->camera.dir.x * WALL_SIZE,
					   0, pos.z + env->camera.dir.z * WALL_SIZE);
	dir = rcm_vecneg(env->cameraview.dir);
	if (id == CHARACTER_ID && 0)
		dir = rcm_vecneg(env->camera.dir);
	new = rcm_mulmat(new, rcm_rotmat(0,- atan2(dir.x, dir.z), 0));
	new[15] = 1;
	return (rcm_mulmat(new, rcm_scalemat(zoom)));
}

float *transform_to_matrix(t_point pos, t_point dir)
{
	float *new;

	new = rcm_rotmat(0, atan2(dir.x, dir.z), 0);
	new = rcm_mulmat(new, rcm_transmat(pos.x, pos.y, pos.z));
	new[15] = 1;
	return (new);
}


void fill_vertex(int index, int x, int y, int z, float sizex, float sizey, float sizez, GLfloat *vertex)
{
	index *= 3;
	vertex[index + 0] = sizex / 2 * x;
	vertex[index + 2] = sizez / 2 * y;
	vertex[index + 1] = sizey / 2 * z;
}

void fill_text_buffer(GLfloat *tBuffer)
{
	tBuffer[0] = 0.1;
	tBuffer[1] = 0;

	tBuffer[2] = 0.1;
	tBuffer[3] = 1;

	tBuffer[4] = 0.9;
	tBuffer[5] = 1;

	tBuffer[6] = 0.9;
	tBuffer[7] = 1;

	tBuffer[8] = 0.9;
	tBuffer[9] = 0;

	tBuffer[10] = 0.1;
	tBuffer[11] = 0;
}

void create_wall(t_env *env, t_wall wall)
{
	int		bitmask = 0;
	int		*text;

	text = malloc(sizeof(int) * 6);
	if (wall.type != TURN && wall.type != BOTTOM)
		bitmask = bitmask | 0x00FF00;
	if (wall.type == BOTTOM)
		bitmask = bitmask | 0x0000F0;
	if (wall.type == HOLE)
		bitmask = bitmask & 0xF0FFFF;
	bitmask = bitmask | 0xFF0000;
	text[1] = CEIL;
	text[0] = FLOOR;
	text[2] = wall.type;
	text[3] = wall.type;
	text[4] = BOTTOM;
	create_box(env, WALL_SIZE, WALL_SIZE, WALL_SIZE, bitmask, text, wall.boxid, wall.pos, wall.dir);
}

void create_box(t_env *env, float sizex, float sixey, float sizez, int bitmask, int *text,
		int boxid, t_point pos, t_point dir)
{
	GLfloat *vertex;
	GLuint *eBuffer;
	GLfloat *tBuffer;

	eBuffer = malloc(sizeof(GLuint) * 6);
	eBuffer[0] = 0;
	eBuffer[1] = 1;
	eBuffer[2] = 2;
	eBuffer[3] = 3;
	eBuffer[4] = 4;
	eBuffer[5] = 5;

	if (bitmask & 0x0F0000)
	{
		vertex = malloc(sizeof(GL_FLOAT) * 6 * 3);
		tBuffer = malloc(sizeof(GL_FLOAT) * 6 * 2);
		fill_vertex(0, -1, -1, -1, sizex, sixey, sizez, vertex);
		fill_vertex(1, -1, 1, -1, sizex, sixey, sizez, vertex);
		fill_vertex(2, 1, 1, -1, sizex, sixey, sizez, vertex);
		fill_vertex(3, 1, 1, -1, sizex, sixey, sizez, vertex);
		fill_vertex(4, 1, -1, -1, sizex, sixey, sizez, vertex);
		fill_vertex(5, -1, -1, -1, sizex, sixey, sizez, vertex);
		fill_text_buffer(tBuffer);
		create_object(transform_to_matrix(pos, dir), text[0], vertex, env, 18, eBuffer, boxid, tBuffer);
	}
	if (bitmask & 0xF00000)
	{
		vertex = malloc(sizeof(GL_FLOAT) * 6 * 3);
		tBuffer = malloc(sizeof(GL_FLOAT) * 6 * 2);
		fill_vertex(0, -1, -1, 1, sizex, sixey, sizez, vertex);
		fill_vertex(1, -1, 1, 1, sizex, sixey, sizez, vertex);
		fill_vertex(2, 1, 1, 1, sizex, sixey, sizez, vertex);
		fill_vertex(3, 1, 1, 1, sizex, sixey, sizez, vertex);
		fill_vertex(4, 1, -1, 1, sizex, sixey, sizez, vertex);
		fill_vertex(5, -1, -1, 1, sizex, sixey, sizez, vertex);
		fill_text_buffer(tBuffer);
		create_object(transform_to_matrix(pos, dir), text[1], vertex, env, 18, eBuffer, boxid, tBuffer);
	}

	if (bitmask & 0x00F000)
	{	
		vertex = malloc(sizeof(GL_FLOAT) * 6 * 3);
		tBuffer = malloc(sizeof(GL_FLOAT) * 6 * 2);
		fill_vertex(0, -1, -1, -1, sizex, sixey, sizez, vertex);
		fill_vertex(1, -1, -1, 1, sizex, sixey, sizez, vertex);
		fill_vertex(2, -1, 1, 1, sizex, sixey, sizez, vertex);
		fill_vertex(3, -1, 1, 1, sizex, sixey, sizez, vertex);
		fill_vertex(4, -1, 1, -1, sizex, sixey, sizez, vertex);
		fill_vertex(5, -1, -1, -1, sizex, sixey, sizez, vertex);
		fill_text_buffer(tBuffer);
		create_object(transform_to_matrix(pos, dir), text[2], vertex, env, 18, eBuffer, boxid, tBuffer);
	}

	if (bitmask & 0x000F00)
	{
		vertex = malloc(sizeof(GL_FLOAT) * 6 * 3);
		tBuffer = malloc(sizeof(GL_FLOAT) * 6 * 2);
		fill_vertex(0, 1, -1, -1, sizex, sixey, sizez, vertex);
		fill_vertex(1, 1, -1, 1, sizex, sixey, sizez, vertex);
		fill_vertex(2, 1, 1, 1, sizex, sixey, sizez, vertex);
		fill_vertex(3, 1, 1, 1, sizex, sixey, sizez, vertex);
		fill_vertex(4, 1, 1, -1, sizex, sixey, sizez, vertex);
		fill_vertex(5, 1, -1, -1, sizex, sixey, sizez, vertex);
		fill_text_buffer(tBuffer);
		create_object(transform_to_matrix(pos, dir), text[3], vertex, env, 18, eBuffer, boxid, tBuffer);
	}
	if (bitmask & 0x0000F0)
	{
		vertex = malloc(sizeof(GL_FLOAT) * 6 * 3);
		tBuffer = malloc(sizeof(GL_FLOAT) * 6 * 2);
		fill_vertex(0, -1, 1, -1, sizex, sixey, sizez, vertex);
		fill_vertex(1, -1, 1, 1, sizex, sixey, sizez, vertex);
		fill_vertex(2, 1, 1, 1, sizex, sixey, sizez, vertex);
		fill_vertex(3, 1, 1, 1, sizex, sixey, sizez, vertex);
		fill_vertex(4, 1, 1, -1, sizex, sixey, sizez, vertex);
		fill_vertex(5, -1, 1, -1, sizex, sixey, sizez, vertex);
		fill_text_buffer(tBuffer);
		create_object(transform_to_matrix(pos, dir), text[4], vertex, env, 18, eBuffer, boxid, tBuffer);
	}

	if (bitmask & 0x00000F)
	{
		vertex = malloc(sizeof(GL_FLOAT) * 6 * 3);
		tBuffer = malloc(sizeof(GL_FLOAT) * 6 * 2);
		fill_vertex(0, -1, -1, -1, sizex, sixey, sizez, vertex);
		fill_vertex(1, -1, -1, 1, sizex, sixey, sizez, vertex);
		fill_vertex(2, 1, -1, 1, sizex, sixey, sizez, vertex);
		fill_vertex(3, 1, -1, 1, sizex, sixey, sizez, vertex);
		fill_vertex(4, 1, -1, -1, sizex, sixey, sizez, vertex);
		fill_vertex(5, -1, -1, -1, sizex, sixey, sizez, vertex);
		fill_text_buffer(tBuffer);
		create_object(transform_to_matrix(pos, dir), text[5], vertex, env, 18, eBuffer, boxid, tBuffer);
	}
}

void create_object(float *model_matrix, GLuint textIndex, GLfloat *vertices, t_env *env,
				   GLuint vertex_size, GLuint *elementBuffer, int id, GLfloat *textBuffer)
{
	t_gl_object new;

	new.progid = env->progid;
	new.modelMat = model_matrix;
	new.textIndex = textIndex;

	glGenVertexArrays(1, &(new.vertexArrayID));
	glGenBuffers(1, &(new.vertexBufferID));

    glGenVertexArrays(1, &(new.textArrayID));
    glGenBuffers(1, &(new.textBufferID));

	glGenBuffers(1, &(new.elementBufferID));
	new.vertex_array = vertices;
	new.textBuffer = textBuffer;
	new.elementBuffer = elementBuffer;
	new.elementBufferSize = 6;
	new.vertex_size = vertex_size;
	new.boxid = id;
//	if (id == CHARACTER_ID)
	//	env->character.object = &new;
	ft_lstadd(&(env->to_render), ft_lstnew(&new, sizeof(t_gl_object)));
}
