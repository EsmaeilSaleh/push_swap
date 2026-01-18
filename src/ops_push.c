/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_push.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esmaeil <esmaeil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 00:00:00 by esmaeil           #+#    #+#             */
/*   Updated: 2026/01/18 00:00:00 by esmaeil          ###   ########.fr       */
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
		write(1, "pa\n", 3);
}

void	pb(t_node **a, t_node **b, int print)
{
	push_stack(a, b);
	if (print)
		write(1, "pb\n", 3);
}
