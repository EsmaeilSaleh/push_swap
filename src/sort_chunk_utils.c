/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_chunk_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esmaeil <esmaeil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 00:00:00 by esmaeil           #+#    #+#             */
/*   Updated: 2026/01/18 00:00:00 by esmaeil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	find_min_pos(t_node *a)
{
	int	pos;
	int	min_pos;
	int	min_idx;

	pos = 0;
	min_pos = 0;
	min_idx = a->index;
	while (a)
	{
		if (a->index < min_idx)
		{
			min_idx = a->index;
			min_pos = pos;
		}
		pos++;
		a = a->next;
	}
	return (min_pos);
}

void	rotate_to_min(t_node **a)
{
	int	pos;
	int	size;

	pos = find_min_pos(*a);
	size = stack_size(*a);
	if (pos <= size / 2)
		while ((*a)->index != 0)
			ra(a, 1);
	else
		while ((*a)->index != 0)
			rra(a, 1);
}

int	find_max_pos(t_node *b)
{
	int		pos;
	int		max_pos;
	int		max_index;
	t_node	*tmp;

	tmp = b;
	pos = 0;
	max_pos = 0;
	max_index = tmp->index;
	while (tmp)
	{
		if (tmp->index > max_index)
		{
			max_index = tmp->index;
			max_pos = pos;
		}
		pos++;
		tmp = tmp->next;
	}
	return (max_pos);
}
