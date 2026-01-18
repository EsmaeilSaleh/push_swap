/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esmaeil <esmaeil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 00:00:00 by esmaeil           #+#    #+#             */
/*   Updated: 2026/01/18 00:00:00 by esmaeil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	main(int argc, char **argv)
{
	t_node	*a;
	t_node	*b;
	int		size;

	a = NULL;
	b = NULL;
	parse_args(&a, argc, argv);
	if (!a || is_sorted(a))
	{
		free_stack(&a);
		return (0);
	}
	assign_indexes(a);
	size = stack_size(a);
	if (size <= 5)
		sort_small(&a, &b);
	else
		chunk_sort(&a, &b, size);
	free_stack(&a);
	free_stack(&b);
	return (0);
}
