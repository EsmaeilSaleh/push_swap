/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_rotate.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 14:19:09 by esaleh            #+#    #+#             */
/*   Updated: 2026/02/15 14:19:10 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	rotate_stack(t_node **s)
{
	t_node	*first;
	t_node	*second;
	t_node	*last;

	if (!s || !*s || !(*s)->next)
		return ;
	first = *s;
	second = first->next;
	if (!second->next)
	{
		second->next = first;
		first->next = NULL;
		*s = second;
		return ;
	}
	*s = second;
	first->next = NULL;
	last = second;
	while (last->next)
		last = last->next;
	last->next = first;
}

void	ra(t_node **a, int print)
{
	rotate_stack(a);
	if (print)
	{
		write(1, "ra\n", 3);
		debug_log_op("ra", *a, NULL);
	}
}

void	rb(t_node **b, int print)
{
	rotate_stack(b);
	if (print)
	{
		write(1, "rb\n", 3);
		debug_log_op("rb", NULL, *b);
	}
}

void	rr(t_node **a, t_node **b, int print)
{
	rotate_stack(a);
	rotate_stack(b);
	if (print)
	{
		write(1, "rr\n", 3);
		debug_log_op("rr", *a, *b);
	}
}
