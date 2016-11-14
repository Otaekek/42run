/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_opengl.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcargou <rcargou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/02 11:42:37 by rcargou           #+#    #+#             */
/*   Updated: 2015/10/10 10:53:57 by rcargou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "run.h"

static void			load_textures(t_env *env)
{
	if (env->text == 0)
	{
		load_bmp("textures/plancher.bmp", 0, env, FLOOR);
		load_bmp("textures/plafond.bmp", 0, env, CEIL);
		load_bmp("textures/wall.bmp", 0, env, WALL1);
		load_bmp("textures/wall3.bmp", 0, env, WALL2);
		load_bmp("textures/wall3.bmp", 0, env, WALL3);
		load_bmp("textures/bottom.bmp", 0, env, BOTTOM);
		load_bmp("textures/hole.bmp", 0, env, HOLE);
		load_bmp("textures/test3.bmp", 0, env, 10);
		load_bmp("textures/mac.bmp", 0, env, MAC);
		load_bmp("textures/white.bmp", 0, env, 20);
		load_bmp("textures/laveuse.bmp", 0, env, LAVEUSE);
	}
	else
	{
		load_bmp("textures/hellwall.bmp", 0, env, FLOOR);
		load_bmp("textures/hellwall.bmp", 0, env, CEIL);
		load_bmp("textures/hellwall.bmp", 0, env, WALL1);
		load_bmp("textures/hellwall.bmp", 0, env, WALL2);
		load_bmp("textures/wall3.bmp", 0, env, WALL3);
		load_bmp("textures/spikes.bmp", 0, env, BOTTOM);
		load_bmp("textures/lave.bmp", 0, env, HOLE);
		load_bmp("textures/test3.bmp", 0, env, 10);
		load_bmp("textures/maitre.bmp", 0, env, MAC);
		load_bmp("textures/white.bmp", 0, env, 20);
		load_bmp("textures/maitre.bmp", 0, env, LAVEUSE);
	}
}

static t_env		set_mlx(int n)
{
	t_env new;

	ft_bzero(&new, sizeof(t_env));
	new.trebuch = 0;
	new.dont_turn = 0;
	new.obstacle = NULL;
	new.pause = 0;
	new.walls = NULL;
	new.to_render = NULL;
	new.turn = 0;
	new.vision = VISION;
	new.trans = 0;
	new.model = NULL;
	new.mlx = mlx_init();
	bzero((&new.ctrl), sizeof(t_ctrl));
	new.win = mlx_new_opengl_window(new.mlx, 1100, 900, "Meuh Meuh");
	new.camera.pos = rcm_setpoint(0, 0, 0);
	new.camera.dir = rcm_vecnormalize(rcm_setpoint(0, 0, 1));
	new.cameraview.pos = rcm_setpoint(0, 0, 100);
	new.cameraview.dir = rcm_vecnormalize(rcm_setpoint(0, 0, 1));
	mlx_opengl_window_set_context(new.win);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0, 0, 0, 1);
	new.camera.zoom = 1.0 / 2000.0f;
	mlx_opengl_swap_buffers(new.win);
	new.progid = loadshaders("shader/frag.shader", "shader/vertex.shader");
	glUseProgram(new.progid);
	new.Mid = glGetUniformLocation(new.progid, "M");
	new.Vid = glGetUniformLocation(new.progid, "V");
	new.Pid = glGetUniformLocation(new.progid, "P");
	new.textID = glGetUniformLocation(new.progid, "text");
	new.alphaID = glGetUniformLocation(new.progid, "alpha");
	new.speed = WALL_SIZE / 18;
	new.speed_to_add = new.speed / 1000;
	new.jumpspeed = 0;
	new.text = n;
	load_textures(&new);
	create_character(&new);
	glEnable(GL_BLEND);
	new.character.animation.character = 1;
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE, GL_ZERO);
//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc (GL_ZERO,GL_ONE);
//	glBlendFunc(GL_SRC_ALPHA ,GL_ONE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return (new);

}

void				mlx(int n)
{
	t_env *env;

	env = malloc(sizeof(t_env));
	*env = set_mlx(n);
	init_walls(env);
	procedural_gen(&(env->walls), env, env->camera.pos, env->camera.dir);
	mlx_do_key_autorepeatoff(env->mlx);
    mlx_hook(env->win, 2, 1L<<0, key_hook, env);
    mlx_hook(env->win, 3, 1L<<1, key_release_hook, env);
	//mlx_key_hook (env->win, key_hook, env);
	mlx_loop_hook(env->mlx, render, env);
	mlx_loop(env->mlx);
}
