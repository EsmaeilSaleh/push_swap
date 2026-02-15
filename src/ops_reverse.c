/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_reverse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 14:19:18 by esaleh            #+#    #+#             */
/*   Updated: 2026/02/15 14:19:19 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	rev_rotate_stack(t_node **s)
{
	t_node	*first;
	t_node	*prev;
	t_node	*last;

	if (!s || !*s || !(*s)->next)
		return ;
	first = *s;
	if (!first->next->next)
	{
		last = first->next;
		first->next = NULL;
		last->next = first;
		*s = last;
		return ;
	}
	prev = first;
	last = first->next;
	while (last->next)
	{
		prev = last;
		last = last->next;
	}
	prev->next = NULL;
	last->next = first;
	*s = last;
}

void	rra(t_node **a, int print)
{
	rev_rotate_stack(a);
	if (print)
	{
		write(1, "rra\n", 4);
		debug_log_op("rra", *a, NULL);
	}
}

void	rrb(t_node **b, int print)
{
	rev_rotate_stack(b);
	if (print)
	{
		write(1, "rrb\n", 4);
		debug_log_op("rrb", NULL, *b);
	}
}

void	rrr(t_node **a, t_node **b, int print)
{
	rev_rotate_stack(a);
	rev_rotate_stack(b);
	if (print)
	{
		write(1, "rrr\n", 4);
		debug_log_op("rrr", *a, *b);
	}
}
