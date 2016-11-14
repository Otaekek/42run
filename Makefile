# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/12/10 14:41:44 by tvallee           #+#    #+#              #
#    Updated: 2015/10/10 11:16:13 by rcargou          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC		= main.c procedural_generation.c set_opengl.c init_opengl.c shader.c render.c load_bmp.c update.c \
			create_entity.c
NAME	= 42run
CC		= gcc
CFLAGS 	= -Wall -Wextra -Werror
SRC_DIR	= src
VPATH	= $(SRC_DIR)
INC		= -I./include -I./rc_lib/libft -I./minilibx_macos -I./rc_lib/rc_math
OBJ_DIR	= obj
OBJ		= $(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))
LIB		= -L./rc_lib/libft -lft -lm -L./minilibx_macos -lmlx -L./rc_lib/rc_math -lrcmath
FW      = -framework Appkit -framework OpenGL
LIBFT   = rc_lib/libft/libft.a 
MLX 	= minilibx_macos/libmlx.a
RC_MATH = rc_lib/rc_math/librcmath.a

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJ) $(LIBFT) $(RC_MATH) $(MLX)
	@echo "Linkin'"
	@$(CC) -o $(NAME) $(CFLAGS) $(OBJ) $(INC) $(FW) $(LIB)
	@echo "Done ! \o/"

$(OBJ_DIR)/%.o: %.c
	@echo "Compiling $<."
	@$(CC) $(LIN_INC) $(CFLAGS) -c $< $(INC) -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(RC_MATH):
	@make -C rc_lib/rc_math

$(LIBFT):
	@make -C rc_lib/libft

$(MLX): 
	@make -C minilibx_macos

clean:
	@echo "Deleting obj files."
	rm -Rf $(OBJ_DIR)

fclean: clean
	@echo "Deleting binary."
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
