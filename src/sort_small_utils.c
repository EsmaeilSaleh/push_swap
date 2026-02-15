/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_small_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 14:16:26 by esaleh            #+#    #+#             */
/*   Updated: 2026/02/15 14:16:27 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sort_three_cases(t_node **a, int first, int second, int third)
{
	if (first > second && second < third && first < third)
		sa(a, 1);
	else if (first > second && second > third)
	{
		sa(a, 1);
		rra(a, 1);
	}
	else if (first > second && second < third && first > third)
		ra(a, 1);
	else if (first < second && second > third && first < third)
	{
		sa(a, 1);
		ra(a, 1);
	}
	else if (first < second && second > third && first > third)
		rra(a, 1);
}

int	min_pos(t_node *a)
{
	int	pos;
	int	min;
	int	min_pos;

	pos = 0;
	min_pos = 0;
	min = a->index;
	while (a)
	{
		if (a->index < min)
		{
			min = a->index;
			min_pos = pos;
		}
		pos++;
		a = a->next;
	}
	return (min_pos);
}
