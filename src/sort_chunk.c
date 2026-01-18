/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_chunk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esmaeil <esmaeil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 00:00:00 by esmaeil           #+#    #+#             */
/*   Updated: 2026/01/18 00:00:00 by esmaeil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	get_chunk_size(int size)
{
	if (size <= 16)
		return (8);
	else if (size <= 100)
		return (size / 5);
	else if (size <= 500)
		return (size / 12);
	else
		return (size / 15);
}

static void	push_all_but_three(t_node **a, t_node **b, int size)
{
	int	chunk_size;
	int	total_pushed;
	int	chunk_limit;

	chunk_size = get_chunk_size(size);
	total_pushed = 0;
	chunk_limit = chunk_size;
	while (stack_size(*a) > 3)
	{
		if ((*a)->index < chunk_limit)
		{
			pb(a, b, 1);
			total_pushed++;
			if (total_pushed >= chunk_limit)
				chunk_limit += chunk_size;
			if (*b && (*b)->next && (*b)->index < chunk_limit - chunk_size / 2)
				rb(b, 1);
		}
		else
			ra(a, 1);
	}
}

static void	rotate_to_max(t_node **b)
{
	int	max_pos;
	int	size;

	size = stack_size(*b);
	max_pos = find_max_pos(*b);
	if (max_pos <= size / 2)
	{
		while (max_pos-- > 0)
			rb(b, 1);
	}
	else
	{
		while (max_pos++ < size)
			rrb(b, 1);
	}
}

static void	push_back_all(t_node **a, t_node **b)
{
	while (*b)
	{
		rotate_to_max(b);
		pa(a, b, 1);
	}
}

void	chunk_sort(t_node **a, t_node **b, int size)
{
	if (size <= 3)
	{
		sort_small(a, b);
		return ;
	}
	push_all_but_three(a, b, size);
	sort_small(a, b);
	push_back_all(a, b);
	rotate_to_min(a);
}
