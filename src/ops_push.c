/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_push.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 14:19:27 by esaleh            #+#    #+#             */
/*   Updated: 2026/02/15 14:19:29 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	push_stack(t_node **from, t_node **to)
{
	t_node	*tmp;

	if (!from || !*from)
		return ;
	tmp = *from;
	*from = (*from)->next;
	tmp->next = *to;
	*to = tmp;
}

void	pa(t_node **a, t_node **b, int print)
{
	push_stack(b, a);
	if (print)
	{
		write(1, "pa\n", 3);
		debug_log_op("pa", *a, *b);
	}
}

void	pb(t_node **a, t_node **b, int print)
{
	push_stack(a, b);
	if (print)
	{
		write(1, "pb\n", 3);
		debug_log_op("pb", *a, *b);
	}
}
