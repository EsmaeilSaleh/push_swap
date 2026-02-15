/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 14:21:21 by esaleh            #+#    #+#             */
/*   Updated: 2026/02/15 14:21:23 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

#ifdef DEBUG

static int	has_cycle(t_node *s)
{
	t_node	*slow;
	t_node	*fast;

	slow = s;
	fast = s;
	while (fast && fast->next)
	{
		slow = slow->next;
		fast = fast->next->next;
		if (slow == fast)
			return (1);
	}
	return (0);
}

static void	print_stack(const char *name, t_node *s)
{
	int	count;

	count = 0;
	ft_putstr_fd((char *)name, 2);
	ft_putstr_fd(":", 2);
	while (s && count < 150)
	{
		ft_putstr_fd(" (", 2);
		ft_putnbr_fd(s->value, 2);
		ft_putstr_fd("|", 2);
		ft_putnbr_fd(s->index, 2);
		ft_putstr_fd(")", 2);
		s = s->next;
		count++;
	}
	if (s)
		ft_putstr_fd(" ...", 2);
	ft_putstr_fd("\n", 2);
}

void	debug_log_op(const char *op, t_node *a, t_node *b)
{
	if (has_cycle(a) || has_cycle(b))
	{
		ft_putstr_fd("[DEBUG] cycle detected\n", 2);
		exit(1);
	}
	ft_putstr_fd("[DEBUG] op=", 2);
	ft_putstr_fd((char *)op, 2);
	ft_putstr_fd("\n", 2);
	print_stack("A", a);
	print_stack("B", b);
}

#else

void	debug_log_op(const char *op, t_node *a, t_node *b)
{
	(void)op;
	(void)a;
	(void)b;
}

#endif
