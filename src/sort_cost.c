/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_cost.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esmaeil <esmaeil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 10:00:00 by esmaeil           #+#    #+#             */
/*   Updated: 2026/02/09 10:00:00 by esmaeil          ###   ########.fr       */
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
