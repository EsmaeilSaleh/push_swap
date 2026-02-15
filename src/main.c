/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 14:19:37 by esaleh            #+#    #+#             */
/*   Updated: 2026/02/15 14:19:39 by esaleh           ###   ########.fr       */
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
