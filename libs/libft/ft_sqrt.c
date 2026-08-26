/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 00:12:23 by mohammad          #+#    #+#             */
/*   Updated: 2025/10/03 00:27:24 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_set_bounds(double x, double *left, double *right)
{
	if (x < 1)
	{
		*left = x;
		*right = 1;
	}
	else
	{
		*left = 1;
		*right = x;
	}
}

double	ft_sqrt(double x)
{
	double	left;
	double	right;
	double	middle;
	double	precision;

	if (x < 0)
		return (-1);
	if (x == 0 || x == 1)
		return (x);
	ft_set_bounds(x, &left, &right);
	precision = 0.00001;
	while ((right - left) > precision)
	{
		middle = (left + right) / 2;
		if (middle * middle > x)
			right = middle;
		else
			left = middle;
	}
	return (middle);
}
