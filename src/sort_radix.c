/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_radix.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esmaeil <esmaeil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 00:00:00 by esmaeil           #+#    #+#             */
/*   Updated: 2026/01/18 00:00:00 by esmaeil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	max_bits(t_node *a)
{
	int	max;
	int	bits;

	max = 0;
	bits = 0;
	while (a)
	{
		if (a->index > max)
			max = a->index;
		a = a->next;
	}
	while ((max >> bits) != 0)
		bits++;
	return (bits);
}

void	radix_sort(t_node **a, t_node **b)
{
	int	i;
	int	j;
	int	size;
	int	bits;

	size = stack_size(*a);
	bits = max_bits(*a);
	i = 0;
	while (i < bits)
	{
		j = 0;
		while (j < size)
		{
			if (((*a)->index >> i) & 1)
				ra(a, 1);
			else
				pb(a, b, 1);
			j++;
		}
		while (*b)
			pa(a, b, 1);
		i++;
	}
}
