/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcargou <rcargou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/02 13:57:59 by rcargou           #+#    #+#             */
/*   Updated: 2015/10/10 11:18:25 by rcargou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "run.h"

static void		fill_vbo(t_env *env, t_gl_object object)
{
	GLfloat		*vertex_array;

	env = env + 1;
	vertex_array = object.vertex_array;
	glBindVertexArray(object.vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, object.vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * object.vertex_size, vertex_array, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//glBindVertexArray(object.textArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, object.textBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, object.textBuffer, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
}

float   *matrix_projection(float fov, float near, float far, float aspect)
{
    float   *matrix;
    float   x_scale;
    float   y_scale;
    float   frustrum;

    if ((matrix = rcm_identitymat()) == NULL)
        return (NULL);
    y_scale = (float)((1.0f / tan((fov / 2.0f) / 57.0f)) * aspect);
    x_scale = y_scale / aspect;
    frustrum = far - near;
	matrix[0] = x_scale;
    matrix[5] = y_scale;
    matrix[10] = -((far + near)) / frustrum;
    matrix[11] = -((2 * near * far)) / frustrum;
    matrix[14] = -1;
    matrix[15] = 0;

    return (matrix);
}

static void		add_var(t_env *env, t_gl_object object)
{
	float alpha;

	glUniform1i(env->textID, 0);
	glBindTexture(GL_TEXTURE_2D, env->textIDs[object.textIndex]);
	glActiveTexture(GL_TEXTURE0);
	glUniformMatrix4fv(env->Mid, 1, GL_FALSE, object.modelMat);
	glUniformMatrix4fv(env->Vid, 1, GL_FALSE,
					   transform_to_matrix_cam(rcm_vecneg(env->camera.pos),
											   rcm_vecneg(env->camera.dir), env->camera.zoom, env, object.boxid));
	glUniformMatrix4fv(env->Pid, 1, GL_FALSE, matrix_projection(60, 0.1f, 100, 1));
	alpha = 1;
	if (object.boxid == CHARACTER_ID && !(env->trebuch % 2) && env->trebuch > 0)
		alpha = 0.3;
	glUniform1f(env->alphaID, alpha);
}

int				render(t_env *env)
{
	t_list			*tmp;
	t_gl_object		*elem;
    static float    time = 0;

	tmp = env->to_render;
	while (tmp)
	{
		elem = tmp->content;
		if (elem->boxid == CHARACTER_ID)
		{
			env->character.object = elem;
			break ;
		}
		tmp = tmp->next;
	}

	elem = tmp->content;
	if (1 / ((clock() - time) / CLOCKS_PER_SEC) > 60)
	   return (0);
	time = clock();
	update(env);
	glClear((GL_COLOR_BUFFER_BIT)| GL_DEPTH_BUFFER_BIT);
	tmp = env->to_render;
	env->camera.dir = rcm_vecnormalize(env->camera.dir);
    if (!rcm_veqec(env->cameraview.dir, env->camera.dir))
		env->cameraview.dir = converge_cam(env->cameraview.dir, env->camera.dir, env);

	while (tmp)
	{
		elem=tmp->content;
		if (elem->boxid == CHARACTER_ID)
				glEnable(GL_BLEND);
		fill_vbo(env, *elem);
		add_var(env, *elem);
//		if (elem->boxid == CHARACTER_ID)
		glDrawArrays(GL_TRIANGLES, 0, elem->vertex_size / 3);
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDisable(GL_BLEND);
		tmp = tmp->next;
	}
	mlx_opengl_swap_buffers(env->win);
	return (0);
}
