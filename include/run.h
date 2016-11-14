/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   42run.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcargou <rcargou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/29 11:18:25 by rcargou           #+#    #+#             */
/*   Updated: 2015/10/09 17:46:09 by rcargou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUN_H
# define RUN_H

# include "rc_math.h"
# include "libft.h"
# include "mlx.h"
# include "mlx_opengl.h"

# include <OpenGL/gl3.h>
# include <fcntl.h>
# include <stdio.h>
# include <math.h>
# include <time.h>

# define COR_SIZE 2000
# define COR_SIZEX COR_SIZE
# define WALL_SIZE COR_SIZE
# define VISION WALL_SIZE * 20

# define BOXBOX riven

# define CHARACTER_ID -1

# define RIGHT 124
# define LEFT 123
# define DOWN 125
# define UP 126


typedef enum {
	FLOOR = 0,
	CEIL,
	BOTTOM,
	TURN,
	HOLE,
	WALL1,
	WALL2,
	WALL3,
	MAC,
	LAVEUSE,
}	walltype;

typedef struct		s_obstacle
{
	int				type;
	int				boxid;
	int				text;
	t_point			pos;
	t_point			dir;
	size_t			sizex;
	size_t			sizey;
	size_t			sizez;
}					t_obstacle;

typedef struct		s_ctrl
{
	int				up;
	int				down;
	int				left;
	int				right;
	int				space;
	int c;
}					t_ctrl;

typedef struct		s_gl_object
{
	GLuint			*elementBuffer;
	int				elementBufferSize;
	GLfloat			*vertex_array;
	GLfloat			*textBuffer;
	GLuint			vertex_size;
	GLuint			textIndex;
	GLuint			vertexArrayID;
	GLuint			progid;
	float			*modelMat;
	GLuint			modelMatID;
	GLuint			vertexBufferID;
	GLuint			elementBufferID;
	GLuint			textArrayID;
	GLuint			textBufferID;
	int				boxid;
}					t_gl_object;

typedef struct		s_wall
{
	int				size_l;
	int				type;
	char			*textpath;
	t_point			pos;
	t_point			dir;
	t_gl_object		*object;
	int				boxid;
}					t_wall;

typedef struct		s_camera
{
	t_point			pos;
	t_point			dir;
	float			zoom;
}					t_camera;

typedef struct		s_animation
{
	int				animID;
	int				animIDOffsetX[10][10];
	int				animIDOffsetY[10][10];
	int				animIDLenght[10];
	int				animIDSpeed[10];
	int				state;
	float			sizeSpriteX;
	float			sizeSpriteY;
	int				character;
}					t_animation;

typedef struct		s_character
{

	t_point			pos;
	t_point			dir;
	t_gl_object		*object;
	t_animation		animation;
	int				sizex;
	int				sizey;
}					t_character;

typedef struct		s_env
{
	float			speed_to_add;
	int				dont_turn;
	t_list			*obstacle;
	int				jump;
	int				trans;
	float			jumpspeed;
	t_ctrl			ctrl;
	t_list			*walls;
	t_list			*to_render;
	int				turn;
	int				vision;
	t_gl_object		*model;
	t_wall			wall[10];
	t_camera		camera;
	t_camera		cameraview;
	int				pause;
	GLuint			Mid;
	GLuint			Pid;
	GLuint			Vid;
	GLuint			textID;
	GLuint			progid;
	GLuint			alphaID;
	size_t			dist;
	size_t			percuted;
	void			*mlx;
	void			*win;
	float			speed;
	GLuint			textIDs[10];
	t_character		character;
	int				trebuch;
	int				text;
}					t_env;

void				create_laveuse(t_env *env, t_point pos, t_point dir, int boxid, int side);
void				del_obstacle(t_env *env, int id);
void				del(t_env *env, int id);
int					key_release_hook(int keycode, t_env *env);
int					key_hook(int keycode, t_env *env);
void				update(t_env *env);
void				procedural_gen(t_list **wall, t_env *env, t_point pos, t_point dir);
void				create_object(float *model_matrix, GLuint textid,
								  GLfloat *vertices, t_env *env,
					GLuint vertex_size, GLuint *e, int id, GLfloat *t);
void				gen_vbo(t_env *env, t_gl_object object);
int					render(t_env *env);
void				mlx(int n);
GLuint				loadshaders(char *filea, char *fileb);
float				*transform_to_matrix(t_point pos, t_point dir);
float				*transform_to_matrix_cam(t_point pos, t_point dir, float zoom, t_env *y, int a);
void				init_walls(t_env *env);
void				load_bmp(char *imagepath, int fd, t_env *env, int index);
t_point				converge_cam(t_point a, t_point b, t_env *u);
void				create_box(t_env *env, float size, float sizey, float sizez, int bitmask,
						int *text, int boxid, t_point pos, t_point dir);
void				create_wall(t_env *env, t_wall wall);
void				create_hole(t_env *env, t_point pos, t_point dir, int boxid, int side);
void				game_over(t_env *env);
t_wall				*in_wich_box_am_i(t_point p, t_list *walls);
void				create_character(t_env *env);
void				update_character(t_env *env);
t_point				box_position(t_point pos);
void				create_mac(t_env *env, t_point pos, t_point dir, int boxid, int side);

#endif
