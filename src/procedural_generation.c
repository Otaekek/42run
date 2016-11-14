/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   procedural_generation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcargou <rcargou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/29 12:17:10 by rcargou           #+#    #+#             */
/*   Updated: 2015/10/10 11:17:08 by rcargou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "run.h"

int					rand_range(int min, int max)
{
	float tmp;

	tmp = rand();
	tmp /= RAND_MAX;
	return (tmp * (float)(max - min) + min);
}

void				init_walls(t_env *env)
{
	for (int i = 0; i < 10; i++)
		env->wall[i].size_l = WALL_SIZE;
	env->wall[0].type = FLOOR;
	env->wall[1].type = CEIL;
	env->wall[2].type = BOTTOM;
	env->wall[3].type = TURN;
	env->wall[4].type = HOLE;
	env->wall[5].type = WALL1;
	env->wall[6].type = WALL2;
	env->wall[7].type = WALL3;
}

t_wall *is_in_list(t_point pos, t_list *start)
{
	t_wall *elem;

	while (start)
	{
		elem = start->content;
		if (rcm_veqec(elem->pos, pos))
			return (elem);
		start = start->next;
	}
	return (0);
}


static void			add_wall(t_point pos, t_point dir, t_list **wall,
							 int seed, float count, t_env *env, int no_obs)
{
	t_wall new;
	t_point up;
	t_wall *test;

	static int a = 0;

	if (a == CHARACTER_ID || a == CHARACTER_ID - 1)
		a += 2;
	up.x = 0;
	up.y = 1;
	up.z = 0;
    if (count <= 0)
        return ;
	if ((seed == TURN || seed == BOTTOM) && rand_range(0, 3) == 2)
		seed = WALL1;
	if (no_obs >= 0)
		seed = WALL1;
	if (seed == HOLE || seed == BOTTOM || seed == TURN)
		no_obs = 4;
	if ((test = is_in_list(pos, *wall)))
	{
		if (test->type == HOLE || test->type == BOTTOM || test->type == TURN)
			no_obs = 4;
		add_wall(rcm_vecsum(pos, rcm_vecscalarfactor(dir, WALL_SIZE)), dir, wall,
				 rand_range(2 + (test->type == TURN || test->type == BOTTOM) * 3, 7),
					count - WALL_SIZE, env, no_obs - 1);
		return ;
	}
	if (seed == TURN || seed == BOTTOM)
	{
		add_wall(rcm_vecsum(pos, rcm_vecscalarfactor((rcm_crossproduct(dir, up)), COR_SIZEX)),
				 rcm_crossproduct(dir, up), wall, rand_range(4, 7), WALL_SIZE, env, no_obs - 1);
		add_wall(rcm_vecsum(pos, rcm_vecscalarfactor((rcm_crossproduct(rcm_vecneg(dir), up)), COR_SIZEX)),
				 (rcm_crossproduct(rcm_vecneg(dir), up)), wall, rand_range(4, 7), WALL_SIZE, env, no_obs - 1);
	}
	new.pos = pos;
	new.dir = dir;
	new.type = seed;
	new.boxid = a;
	ft_lstadd(wall, ft_lstnew(&new, sizeof(t_wall)));
	if (seed != HOLE && seed != TURN && seed != BOTTOM && no_obs < 1 && !(rand_range(0, 3)))
		create_mac(env, pos, dir, a, rand_range(-2, 2));
	else if (seed != HOLE && seed != TURN && seed != BOTTOM && no_obs < 1 && !(rand_range(0, 3)))
		create_laveuse(env, pos, dir, a, rand_range(-2, 2));

	if (seed == HOLE)
	{
		create_hole(env, rcm_vecsum(pos, rcm_vecscalarfactor(dir, WALL_SIZE)), dir, a, 0);
		a++;
  		create_hole(env, pos, dir, a, 1);
		new.boxid = a;
		new.pos = rcm_vecsum(new.pos, rcm_vecscalarfactor(dir, WALL_SIZE));
		ft_lstadd(wall, ft_lstnew(&new, sizeof(t_wall)));
		add_wall(rcm_vecsum(pos, rcm_vecscalarfactor(dir, WALL_SIZE * 2)), dir, wall,
			 rand_range(2 + (seed == TURN || seed == BOTTOM) * 3, 7), count - WALL_SIZE, env, no_obs - 1);
		return ;
	}
	else
		create_wall(env, new);
	a++;
	add_wall(rcm_vecsum(pos, rcm_vecscalarfactor(dir, WALL_SIZE)), dir, wall,
		rand_range(2 + (seed == TURN || seed == BOTTOM) * 3, 7), count - WALL_SIZE, env, no_obs - 1);
}

 void del_to_render(t_env *env, int id)
{
	t_list *tmp;
	t_gl_object *elem;

	tmp = env->to_render;
	while (tmp)
	{
		elem = tmp->content;
		if (elem->boxid == id)
			ft_lstsup(&(env->to_render), tmp);
		tmp = tmp->next;
	}
}

void del_obstacle(t_env *env, int id)
{
    t_list		*tmp;
    t_obstacle	*elem;
  
	tmp = env->obstacle;
	while (tmp)
	{
		elem = tmp->content;
		if (elem->boxid == id)
			ft_lstsup(&(env->obstacle), tmp);
		tmp = tmp->next;
	}
}

int is_avoisinant(t_point pos, t_wall *elem, t_point up, t_env *env)
{
	return (!rcm_veqec(elem->pos, rcm_vecsum(pos, rcm_crossproduct(up, env->camera.dir)))
			&& !rcm_veqec(elem->pos, rcm_vecsum(pos, rcm_crossproduct(up, rcm_vecneg(env->camera.dir)))));
}

static void del_wall(t_env *env, int id)
{
	t_list		*tmp;
	t_wall		*elem;

	tmp = env->walls;
	while (tmp)
	{
		elem = tmp->content;
		if (elem->boxid == id)
			ft_lstsup(&(env->walls), tmp);
		tmp = tmp->next;
	}
}

void del(t_env *env, int id)
{
	del_wall(env,id);
	del_obstacle(env, id);
	del_to_render(env, id);
}

static int kick_overshooted(t_env *env, t_point pos, t_point dir)
{
	t_list	*tmp;
	t_wall	*elem;
	float	dist1;
	float	dist2;
	int		count;
	t_list	*to_be_freed;
	t_point up;

	up = dir;
	up.x = 0;
	up.y = 1;
	up.z = 0;
	count = 0;
	tmp = env->walls;
	while (tmp)
	{
		to_be_freed = NULL;
		elem = tmp->content;
		dist1 = rcm_vecnorme(rcm_vecsum(pos, rcm_vecneg(elem->pos)));
		dist2 = rcm_vecnorme(rcm_vecsum(rcm_vecsum(pos, rcm_vecscalarfactor(env->camera.dir, WALL_SIZE)), rcm_vecneg(elem->pos)));
		if (((dist2 >= dist1) && !rcm_veqec(elem->pos, pos)) && is_avoisinant(pos, elem, up, env))
		{
			del(env, elem->boxid);
			count++;
		}
		tmp = tmp->next;
	}
	count = count > VISION ? VISION : count;
	return (count);
}

void				procedural_gen(t_list **wall, t_env *env, t_point position, t_point dir)
{
	static int		a = 1;
	int				count;
	t_point			pos;
	int				wall_size;
 
	pos = position;
	pos = dir;
	wall_size = WALL_SIZE;
	pos.x = ((int)(env->camera.pos.x - env->camera.dir.x * WALL_SIZE * 4) / wall_size) * (int)wall_size;
	pos.y = ((int)(env->camera.pos.y - env->camera.dir.y * WALL_SIZE * 4)  / wall_size) * (int)wall_size;
	pos.z = ((int)(env->camera.pos.z - env->camera.dir.z * WALL_SIZE * 4)  / wall_size) * (int)wall_size;
	count = kick_overshooted(env, pos, env->camera.dir);
	pos.x = ((int)(env->camera.pos.x / wall_size)) * (int)wall_size;
    pos.y = ((int)(env->camera.pos.y / wall_size)) * (int)wall_size;
    pos.z = ((int)(env->camera.pos.z  / wall_size))* (int)wall_size;
	pos = box_position(env->camera.pos);
	if (a == 1)
	{
		add_wall(rcm_vecsum(pos, rcm_vecscalarfactor(env->camera.dir,
					0)),
				 env->camera.dir, wall, rand_range(4, 6), VISION, env, 10);
	}
	else if (count > 0 || env->turn)
	{
		add_wall(rcm_vecsum(pos, rcm_vecscalarfactor(env->camera.dir,
																 0)),
				env->camera.dir, wall, rand_range(5, 6), VISION, env, 4);
		env->turn = 0;
	}
	a = 0;
}
