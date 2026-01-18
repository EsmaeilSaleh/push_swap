/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_indexes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esmaeil <esmaeil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 00:00:00 by esmaeil           #+#    #+#             */
/*   Updated: 2026/01/18 00:00:00 by esmaeil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	*stack_to_array(t_node *a, int size)
{
	int		*arr;
	int		i;

	arr = (int *)malloc(sizeof(int) * size);
	if (!arr)
		return (NULL);
	i = 0;
	while (a)
	{
		arr[i++] = a->value;
		a = a->next;
	}
	return (arr);
}

static void	set_indexes(t_node *a, int *arr, int size)
{
	int	i;

	while (a)
	{
		i = 0;
		while (i < size)
		{
			if (arr[i] == a->value)
			{
				a->index = i;
				break ;
			}
			i++;
		}
		a = a->next;
	}
}

void	assign_indexes(t_node *a)
{
	int	*arr;
	int	size;

	size = stack_size(a);
	arr = stack_to_array(a, size);
	if (!arr)
		error_exit(&a, NULL, NULL);
	sort_array(arr, size);
	set_indexes(a, arr, size);
	free(arr);
}
