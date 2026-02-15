/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_cost.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 14:17:33 by esaleh            #+#    #+#             */
/*   Updated: 2026/02/15 14:17:35 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	abs_value(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

int	get_rot_value(int pos, int size)
{
	if (pos <= size / 2)
		return (pos);
	return (pos - size);
}

int	calc_move_cost(int rot_a, int rot_b)
{
	if ((rot_a >= 0 && rot_b >= 0) || (rot_a < 0 && rot_b < 0))
	{
		if (abs_value(rot_a) > abs_value(rot_b))
			return (abs_value(rot_a));
		return (abs_value(rot_b));
	}
	return (abs_value(rot_a) + abs_value(rot_b));
}
