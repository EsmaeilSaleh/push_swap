/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_swap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esmaeil <esmaeil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 00:00:00 by esmaeil           #+#    #+#             */
/*   Updated: 2026/01/18 00:00:00 by esmaeil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	swap_stack(t_node **s)
{
	t_node	*first;
	t_node	*second;

	if (!s || !*s || !(*s)->next)
		return ;
	first = *s;
	second = (*s)->next;
	first->next = second->next;
	second->next = first;
	*s = second;
}

void	sa(t_node **a, int print)
{
	swap_stack(a);
	if (print)
	{
		write(1, "sa\n", 3);
		debug_log_op("sa", *a, NULL);
	}
}

void	sb(t_node **b, int print)
{
	swap_stack(b);
	if (print)
	{
		write(1, "sb\n", 3);
		debug_log_op("sb", NULL, *b);
	}
}

void	ss(t_node **a, t_node **b, int print)
{
	swap_stack(a);
	swap_stack(b);
	if (print)
	{
		write(1, "ss\n", 3);
		debug_log_op("ss", *a, *b);
	}
}
