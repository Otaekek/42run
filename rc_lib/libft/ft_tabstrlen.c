/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tabstrlen.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcargou <rcargou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/11 19:40:47 by rcargou           #+#    #+#             */
/*   Updated: 2015/09/04 14:25:36 by rcargou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_tabstrlen(char **tab)
{
	int i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}