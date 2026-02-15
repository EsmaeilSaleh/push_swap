/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_small.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 14:16:37 by esaleh            #+#    #+#             */
/*   Updated: 2026/02/15 14:16:39 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	sort_three(t_node **a)
{
	int	first;
	int	second;
	int	third;

	if (is_sorted(*a))
		return ;
	first = (*a)->index;
	second = (*a)->next->index;
	third = (*a)->next->next->index;
	sort_three_cases(a, first, second, third);
}

static void	rotate_min_to_top(t_node **a)
{
	int	pos;
	int	size;

	pos = min_pos(*a);
	size = stack_size(*a);
	if (pos <= size / 2)
		while (pos-- > 0)
			ra(a, 1);
	else
		while (pos++ < size)
			rra(a, 1);
}

static void	sort_four(t_node **a, t_node **b)
{
	rotate_min_to_top(a);
	pb(a, b, 1);
	sort_three(a);
	pa(a, b, 1);
}

static void	sort_five(t_node **a, t_node **b)
{
	rotate_min_to_top(a);
	pb(a, b, 1);
	rotate_min_to_top(a);
	pb(a, b, 1);
	sort_three(a);
	pa(a, b, 1);
	pa(a, b, 1);
}

void	sort_small(t_node **a, t_node **b)
{
	int	size;

	size = stack_size(*a);
	if (size == 2)
	{
		if (!is_sorted(*a))
			sa(a, 1);
	}
	else if (size == 3)
		sort_three(a);
	else if (size == 4)
		sort_four(a, b);
	else if (size == 5)
		sort_five(a, b);
}
