/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_pushback.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esmaeil <esmaeil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 10:00:00 by esmaeil           #+#    #+#             */
/*   Updated: 2026/02/09 10:00:00 by esmaeil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	pick_best_move(t_node *a, t_node *b, int *best_ra, int *best_rb)
{
	int	vals[5];
	int	rot[2];

	vals[0] = 0;
	vals[2] = 2147483647;
	vals[3] = stack_size(a);
	vals[4] = stack_size(b);
	while (b)
	{
		rot[0] = get_rot_value(find_insert_pos_a(a, b->index), vals[3]);
		rot[1] = get_rot_value(vals[0], vals[4]);
		vals[1] = calc_move_cost(rot[0], rot[1]);
		if (vals[1] < vals[2])
		{
			vals[2] = vals[1];
			*best_ra = rot[0];
			*best_rb = rot[1];
		}
		vals[0]++;
		b = b->next;
	}
}

void	push_back_all(t_node **a, t_node **b)
{
	int	rot_a;
	int	rot_b;

	while (*b)
	{
		pick_best_move(*a, *b, &rot_a, &rot_b);
		exec_combined_rotations(a, b, &rot_a, &rot_b);
		pa(a, b, 1);
	}
}
