/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_entity.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcargou <rcargou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/07 12:33:58 by rcargou           #+#    #+#             */
/*   Updated: 2015/10/10 11:19:03 by rcargou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "run.h"

void create_hole(t_env *env, t_point pos, t_point dir, int boxid, int side)
{
	int		bitmask = 0;
	int		*text;
	t_point down;

	down = rcm_setpoint(0, -1, 0);
	text = malloc(sizeof(int) * 6);
	bitmask = bitmask | 0xF0FF00;
	text[1] = CEIL;
	text[0] = FLOOR;
	text[2] = WALL1;
	text[3] = WALL1;
	text[4] = BOTTOM;
	create_box(env, WALL_SIZE, WALL_SIZE, WALL_SIZE, bitmask, text, boxid, pos, dir);

	bitmask = 0;
	text = malloc(sizeof(int) * 6);
	bitmask = bitmask | 0x0FFF00;
	if (side == 1)
		bitmask = bitmask | 0x0FFF0F;
    if (side == 0)
        bitmask = bitmask | 0x0FFFF0;
	text[1] = CEIL;
	text[0] = HOLE;
	text[2] = HOLE;
	text[3] = HOLE;
	text[4] = HOLE;
	text[5] = HOLE;
	create_box(env, WALL_SIZE, WALL_SIZE, WALL_SIZE, bitmask, text, boxid,
			   rcm_vecsum(pos, rcm_vecscalarfactor(down, WALL_SIZE)), dir);
}

void create_laveuse(t_env *env, t_point pos, t_point dir, int boxid, int side)
{
	int				bitmask = 0;
	int				*text;
	t_point			up;
	t_obstacle		new;

	up = rcm_setpoint(0, 1, 0);
	pos = rcm_vecsum(pos, rcm_vecscalarfactor(rcm_crossproduct(dir, up), WALL_SIZE / 4 * side));
	pos.y -= WALL_SIZE / 2 - WALL_SIZE / 5;
	text = malloc(sizeof(int) * 6);
	bitmask = bitmask | 0x00000F;
	text[2] = WALL1;
	text[3] = WALL1;
	text[4]  = LAVEUSE;
	text[5] = LAVEUSE;
	new.type = 0;
	new.boxid = boxid;
	new.pos = pos;
	new.sizex = WALL_SIZE / 4;
	new.sizey = WALL_SIZE;
	new.sizez = WALL_SIZE / 16;
	new.dir = dir;
	ft_lstadd(&(env->obstacle), ft_lstnew(&new, sizeof(t_obstacle)));
	create_box(env, WALL_SIZE / 4, WALL_SIZE / 2, WALL_SIZE / 16, bitmask, text, boxid,
               pos, dir);

}

void create_mac(t_env *env, t_point pos, t_point dir, int boxid, int side)
{
	int				bitmask = 0;
	int				*text;
	t_point			up;
	t_obstacle		new;

	up = rcm_setpoint(0, 1, 0);
	pos = rcm_vecsum(pos, rcm_vecscalarfactor(rcm_crossproduct(dir, up), WALL_SIZE / 4 * side));
	pos.y -= WALL_SIZE / 2 - WALL_SIZE / 8;
	text = malloc(sizeof(int) * 6);
	bitmask = bitmask | 0x00000F;
	text[2] = WALL1;
	text[3] = WALL1;
	text[4]  = MAC;
	text[5] = MAC;
	new.type = 0;
	new.boxid = boxid;
	new.pos = pos;
	new.sizex = WALL_SIZE / 4;
	new.sizey = WALL_SIZE / 4;
	new.sizez = WALL_SIZE / 16;
	new.dir = dir;
	ft_lstadd(&(env->obstacle), ft_lstnew(&new, sizeof(t_obstacle)));
	create_box(env, WALL_SIZE / 4, WALL_SIZE / 4, WALL_SIZE / 16, bitmask, text, boxid,
			   pos, dir);

}

void update_animation(t_env *env)
{
	static int a = 0;
	t_animation *anim;
	GLfloat *tBuffer;
	float	x;
	float	y;

	if (env->pause)
		return ;
	tBuffer = malloc(1000);
	anim = &(env->character.animation);
	tBuffer = env->character.object->textBuffer;
	if (a > anim->animIDSpeed[anim->animID])
	{
		a = 0;
		anim->state = (anim->state + 1) % anim->animIDLenght[anim->animID];
	}
	else
		a++;
	//anim->state = 0;
	x = anim->sizeSpriteX - 0.01;
	y = anim->sizeSpriteY - 0.01;
//	x = 1.0f / 6;
	//y = 1.0f / 6;
	//return ;

	/*  tBuffer[0] = 0;
    tBuffer[1] = 0;
    tBuffer[2] = 0;
    tBuffer[3] = 1;
    tBuffer[4] = 1;
    tBuffer[5] = 1;
    tBuffer[6] = 1;
    tBuffer[7] = 1;
    tBuffer[8] = 1;
    tBuffer[9] = 0;
    tBuffer[10] = 0;
    tBuffer[11] = 0;*/
	float offset;
	float offsety;
	float carofx, carofy;
	int b;
	int c;

	c = env->character.animation.character;

	b = c % 4;
	b *= 3;

	if (c >= 4)
		c = 4;
	else
		c = 0;
	carofx = b;
	carofy = c;
	offsety = 0.008;
	offset = 0;
	if (c == 4)
		offsety = 0.0212;
	y *= -1;
	tBuffer[0] = 0 * x + (carofx + anim->animIDOffsetX[anim->animID][anim->state]) * anim->sizeSpriteX + offset;
	tBuffer[1] = 1 * y + (carofy + anim->animIDOffsetY[anim->animID][anim->state]) * anim->sizeSpriteY + offsety;

	tBuffer[2] = 0 * x + (carofx + anim->animIDOffsetX[anim->animID][anim->state]) * anim->sizeSpriteX + offset;
	tBuffer[3] = 0 * y + (carofy + anim->animIDOffsetY[anim->animID][anim->state]) * anim->sizeSpriteY + offsety;

	tBuffer[4] = 1 * x + (carofx + anim->animIDOffsetX[anim->animID][anim->state]) * anim->sizeSpriteX + offset;
	tBuffer[5] = 0 * y + (carofy + anim->animIDOffsetY[anim->animID][anim->state]) * anim->sizeSpriteY + offsety;

	tBuffer[6] = 1 * x + (carofx + anim->animIDOffsetX[anim->animID][anim->state]) * anim->sizeSpriteX + offset;
	tBuffer[7] = 0 * y + (carofy + anim->animIDOffsetY[anim->animID][anim->state]) * anim->sizeSpriteY + offsety;

	tBuffer[8] = 1 * x + (carofx + anim->animIDOffsetX[anim->animID][anim->state]) * anim->sizeSpriteX + offset;
	tBuffer[9] = 1 * y + (carofy + anim->animIDOffsetY[anim->animID][anim->state]) * anim->sizeSpriteY + offsety;

	tBuffer[10] = 0 * x + (carofx + anim->animIDOffsetX[anim->animID][anim->state]) * anim->sizeSpriteX + offset;
	tBuffer[11] = 1 * y + (carofy + anim->animIDOffsetY[anim->animID][anim->state]) * anim->sizeSpriteY + offsety;

	//env->character.object->textBuffer = tBuffer;
}

void update_character(t_env *env)
{

	t_wall *box;
	t_point up;

	up = rcm_setpoint(0, 1, 0);
	box = in_wich_box_am_i(env->camera.pos, env->walls);
	update_animation(env);
	env->character.pos = env->camera.pos;
	env->character.pos.y += -WALL_SIZE / 2 + WALL_SIZE / 10;
	env->character.pos = rcm_vecsum(env->character.pos,
									rcm_vecscalarfactor(rcm_crossproduct(env->camera.dir, up), env->trans * 50));
	env->character.pos = rcm_vecsum(env->character.pos, rcm_vecscalarfactor(env->camera.dir, -400));
	env->character.dir = env->camera.dir;
	env->character.object->modelMat = transform_to_matrix(env->character.pos,
								rcm_vecneg((env->camera.dir)));
}

void create_character(t_env *env)
{
	int				bitmask = 0;
	int				*textures;
	env->character.pos = rcm_setpoint(0, 0, 0);
	env->character.dir = rcm_setpoint(0, 0, -1);
	env->character.sizex = WALL_SIZE / 5;
	env->character.sizey = WALL_SIZE / 5;
	textures = malloc(sizeof(int) * 6);
	for (int i = 0; i < 6; i++)
		textures[i] = 10;
	bitmask |= 0x00000F;
	create_box(env, WALL_SIZE / 5 , WALL_SIZE / 5, WALL_SIZE / 5, bitmask, textures,
		CHARACTER_ID, env->character.pos, env->character.dir);
	env->character.animation.animID = 1;
	env->character.animation.animIDSpeed[0] = 10;
	env->character.animation.animIDLenght[0] = 4;
	env->character.animation.animIDOffsetY[0][0] = -3;
	env->character.animation.animIDOffsetX[0][0] = 1;
    env->character.animation.animIDOffsetY[0][1] = -3;
    env->character.animation.animIDOffsetX[0][1] = 2;
    env->character.animation.animIDOffsetY[0][2] = -3;
    env->character.animation.animIDOffsetX[0][2] = 1;
    env->character.animation.animIDOffsetY[0][3] = -3;
    env->character.animation.animIDOffsetX[0][3] = 2;
	env->character.animation.animIDSpeed[1] = 1;
	env->character.animation.animIDLenght[1] = 4;
	env->character.animation.animIDOffsetY[1][0] = -3;
	env->character.animation.animIDOffsetX[1][0] = 0;
	env->character.animation.animIDOffsetY[1][1] = -2;
	env->character.animation.animIDOffsetX[1][1] = 0;
	env->character.animation.animIDOffsetY[1][2] = -1;
	env->character.animation.animIDOffsetX[1][2] = 0;
    env->character.animation.animIDOffsetY[1][3] = 0;
    env->character.animation.animIDOffsetX[1][3] = 0;
    env->character.animation.sizeSpriteX = 1.0f / 12.0f;
    env->character.animation.sizeSpriteY= 1.0f / 8.0f;
	env->character.animation.state = 0;
}
