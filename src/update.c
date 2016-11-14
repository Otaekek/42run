/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcargou <rcargou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/06 13:31:56 by rcargou           #+#    #+#             */
/*   Updated: 2015/10/09 19:03:46 by rcargou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "run.h"

void game_over(t_env *env)
{
	printf("GAME OVER !\nDistance parcourue: %lu\nNombre d'obstacle percutes: %lu\nVitesse maximale atteinte: %f boites par seconde\n"
			, env->dist, env->percuted, env->speed * 60.0f / WALL_SIZE);
	exit(0);
}

t_point box_position(t_point pos)
{
	return (rcm_setpoint(((int)pos.x / WALL_SIZE) * WALL_SIZE,
		((int)pos.y / WALL_SIZE) * WALL_SIZE, ((int)pos.z / WALL_SIZE) * WALL_SIZE));
}

t_wall *in_wich_box_am_i(t_point p, t_list *walls)
{
	t_list *tmp;
	t_wall *elem;
	t_point pos;

	pos = box_position(p);
	tmp = walls;
	pos = p;
	while (tmp)
	{
		elem = tmp->content;
		if (pos.x <= elem->pos.x + WALL_SIZE / 2 && pos.x >= elem->pos.x -WALL_SIZE / 2 &&
			pos.z <= elem->pos.z + WALL_SIZE / 2 && pos.z >= elem->pos.z -WALL_SIZE / 2)
			return (elem);
		tmp = tmp->next;
	}
	return (NULL);
}

int			key_hook(int keycode, t_env *env)
{
	if (keycode == 53)
		exit(0);
	if (keycode == UP)
		env->ctrl.up = 1;
    if (keycode == DOWN)
		env->ctrl.down = 1;
    if (keycode == RIGHT)
		env->ctrl.right = 1;
    if (keycode == LEFT)
		env->ctrl.left = 1;
	if (keycode == 49)
		env->ctrl.space = 1;
	if (keycode == 8)
		env->ctrl.c = 1;
	return (0);
}

int         key_release_hook(int keycode, t_env *env)
{
	if (keycode == UP)
		env->ctrl.up = 0;
	if (keycode == DOWN)
		env->ctrl.down = 0;
	if (keycode == RIGHT)
		env->ctrl.right = 0;
	if (keycode == LEFT)
		env->ctrl.left = 0;
    if (keycode == 49)
		env->ctrl.space= 0;
    if (keycode == 8)
		env->ctrl.c = 0;
	return (0);
}

void		handle_input(t_env *env)
{
	t_point cmp;
	t_point up;
	t_wall *box;

	if (env->pause && !env->ctrl.space)
		return ;
	box = in_wich_box_am_i(env->camera.pos, env->walls);
	up = rcm_setpoint(0, 1, 0);
	cmp = box_position(env->camera.pos);
	if (env->ctrl.c)
	{
		env->character.animation.character += 1;
		env->character.animation.character %= 8;
		env->ctrl.c = 0;
	}
	if (env->ctrl.right)
	{
		box = in_wich_box_am_i(rcm_vecsum(env->camera.pos,
			rcm_vecscalarfactor(env->camera.dir, 0)), env->walls);
		box = in_wich_box_am_i(box_position(env->camera.pos), env->walls);
		if (box && (box->type == TURN || box->type == BOTTOM) && !env->dont_turn)
		{
			env->camera.dir = rcm_crossproduct((env->camera.dir), up);
			env->turn = 1;
			env->ctrl.right = 0;
		}
		else if (env->trans < 14)
		{
			env->trans++;
		}
	}
	else if (env->trans > 0 && !env->ctrl.left)
		env->trans--;
	if (env->ctrl.left)
	{
		box = in_wich_box_am_i(rcm_vecsum(env->camera.pos,
				rcm_vecscalarfactor(env->camera.dir, 0)), env->walls);
		box = in_wich_box_am_i(box_position(env->camera.pos), env->walls);
		if (box && (box->type == TURN || box->type == BOTTOM) && !env->dont_turn)
		{
			env->camera.dir = rcm_vecneg(rcm_crossproduct((env->camera.dir), up));
			env->turn = 1;
			env->ctrl.left = 0;
		}
		else if (env->trans > -14)
			env->trans--;
    }
	else if(env->trans < 0 && !env->ctrl.right)
		env->trans++;
	box = in_wich_box_am_i(env->camera.pos, env->walls);
	if (!box)
		return ;
    if (env->camera.pos.y == 0 && box->type != HOLE)
	{
		env->jumpspeed = 0;
		env->character.animation.animID = 0;
	}
	if (env->ctrl.up && env->camera.pos.y == 0 && box->type != HOLE)
	{
		env->character.animation.animID = 1;
		env->jumpspeed = 74;
	}
	if ((box && box->type == HOLE) || env->jumpspeed > 0 || env->camera.pos.y > 0)
	{
		env->camera.pos.y += env->jumpspeed;
	}
	if (env->jumpspeed + 10.0f > 0 && env->camera.pos.y < 30)
		env->camera.pos.y = 0;
	if (env->camera.pos.y != 0 || box->type == HOLE)
		env->jumpspeed -= 4;
	if (env->ctrl.space)
	{
		env->ctrl.space = 0;
		env->pause = !env->pause;
	}
//	printf("%f\n", env->camera.pos.y);
}

void update_obstacle(t_env *env)
{
	t_wall *box;
	t_list *tmp;
	t_obstacle *elem;

	if (env->trebuch > 0|| env->jumpspeed)
		env->character.animation.animID = 1;
	else
		env->character.animation.animID = 0;
	box = in_wich_box_am_i(env->camera.pos, env->walls);
   	if ((box = in_wich_box_am_i(rcm_vecsum(env->camera.pos,
										   rcm_vecneg(rcm_vecscalarfactor(env->camera.dir,
											WALL_SIZE * 1))), env->walls)))
	{

		if (box && box->type != BOTTOM && box->type != TURN && (box = in_wich_box_am_i(env->camera.pos, env->walls)) && box->type != BOTTOM && box->type != TURN)

			env->dont_turn = 0;
			box = in_wich_box_am_i(rcm_vecsum(env->camera.pos,
			rcm_vecneg(rcm_vecscalarfactor(env->camera.dir, WALL_SIZE * 1))), env->walls);
		if (box && box->type == BOTTOM && rcm_veqec(box->dir, (env->camera.dir)))
			game_over(env);
	}
	box = in_wich_box_am_i(env->camera.pos, env->walls);
	if (env->camera.pos.y < -(WALL_SIZE) / 2 || (box->type != HOLE && env->camera.pos.y < 0))
		game_over(env);
	tmp = env->obstacle;
	while (tmp)
	{
		elem = tmp->content;
		if (!(env->character.pos.x - env->character.sizex / 2 > elem->pos.x + elem->sizex / 2) &&
			!(env->character.pos.x + env->character.sizex / 2 < elem->pos.x - elem->sizex / 2) &&
			!(env->character.pos.y + env->character.sizey / 2 > elem->pos.y + elem->sizey / 2) &&
			!(env->character.pos.y - env->character.sizey / 2 < elem->pos.y - elem->sizey / 2) &&
			!(env->character.pos.z - 400 > elem->pos.z - elem->sizez / 2) &&
			!(env->character.pos.z + 400 < elem->pos.z + elem->sizez / 2))
		{
			if (elem->type == 0)
			{
				env->percuted++;
				if (env->trebuch > 0)
					game_over(env);
				del_obstacle(env, elem->boxid);
				env->trebuch = 100;
			}
		}
		tmp = tmp->next;
	}
	env->trebuch--;
}

void		update(t_env *env)
{
	float y;

	update_character(env);
	handle_input(env);
	if (env->pause)
		return ;
	env->dist++;
	update_obstacle(env);
	if (env->turn)
	{
		env->dont_turn = 1;
		y = env->camera.pos.y;
		env->camera.pos = box_position(env->camera.pos);
		env->camera.pos.y = y;
	}
	procedural_gen(&(env->walls), env, env->camera.pos, env->camera.dir);
	env->speed += env->speed_to_add;
	env->speed_to_add /= 1.0008;
	env->camera.pos = rcm_vecsum(env->camera.pos, rcm_vecscalarfactor(env->camera.dir, env->speed));
}
