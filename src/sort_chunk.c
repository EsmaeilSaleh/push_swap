/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_chunk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 14:17:59 by esaleh            #+#    #+#             */
/*   Updated: 2026/02/15 14:18:00 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	get_chunk_size(int size)
{
	if (size <= 100)
		return (size / 6 + 8);
	if (size <= 500)
		return (size / 15 + 18);
	return (size / 20 + 24);
}

static int	find_chunk_pos(t_node *a, int chunk_limit, int max_index)
{
	int	pos;
	int	first;
	int	last;

	pos = 0;
	first = -1;
	last = -1;
	while (a)
	{
		if (a->index < chunk_limit && a->index < max_index)
		{
			if (first == -1)
				first = pos;
			last = pos;
		}
		pos++;
		a = a->next;
	}
	if (first == -1)
		return (-1);
	if (first <= pos - last)
		return (first);
	return (last);
}

static void	rotate_to_pos(t_node **a, int pos)
{
	int	size;

	size = stack_size(*a);
	if (pos <= size / 2)
		while (pos-- > 0)
			ra(a, 1);
	else
		while (pos++ < size)
			rra(a, 1);
}

static void	push_all_but_three(t_node **a, t_node **b, int size)
{
	int	chunk_size;
	int	chunk_limit;
	int	pos;

	chunk_size = get_chunk_size(size);
	chunk_limit = chunk_size;
	while (stack_size(*a) > 3)
	{
		pos = find_chunk_pos(*a, chunk_limit, size - 3);
		if (pos == -1)
		{
			chunk_limit += chunk_size;
			continue ;
		}
		rotate_to_pos(a, pos);
		pb(a, b, 1);
		if (*b && (*b)->next && (*b)->index < chunk_limit - (chunk_size / 2))
			rb(b, 1);
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
