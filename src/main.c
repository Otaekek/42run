/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcargou <rcargou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/29 11:17:59 by rcargou           #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "run.h"

int main (int ac, char **av)
{
	int *options;

	options = malloc(sizeof(int));
	options = ft_getoptions("h", &ac, &av);
	srand(clock());
	mlx(options[0]);
	return (0);
}
