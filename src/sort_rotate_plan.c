/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_rotate_plan.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esmaeil <esmaeil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 10:00:00 by esmaeil           #+#    #+#             */
/*   Updated: 2026/02/09 10:00:00 by esmaeil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	rotate_both_up(t_node **a, t_node **b, int *rot_a, int *rot_b)
{
	while (*rot_a > 0 && *rot_b > 0)
	{
		rr(a, b, 1);
		(*rot_a)--;
		(*rot_b)--;
	}
}

static void	rotate_both_down(t_node **a, t_node **b, int *rot_a, int *rot_b)
{
	while (*rot_a < 0 && *rot_b < 0)
	{
		rrr(a, b, 1);
		(*rot_a)++;
		(*rot_b)++;
	}
}

void	exec_combined_rotations(t_node **a, t_node **b, int *rot_a, int *rot_b)
{
	rotate_both_up(a, b, rot_a, rot_b);
	rotate_both_down(a, b, rot_a, rot_b);
	while (*rot_a > 0)
	{
		ra(a, 1);
		(*rot_a)--;
	}
	while (*rot_a < 0)
	{
		rra(a, 1);
		(*rot_a)++;
	}
	while (*rot_b > 0)
	{
		rb(b, 1);
		(*rot_b)--;
	}
	while (*rot_b < 0)
	{
		rrb(b, 1);
		(*rot_b)++;
	}
}
