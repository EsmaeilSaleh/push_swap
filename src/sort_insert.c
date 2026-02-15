/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_insert.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esaleh <esaleh@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 14:17:17 by esaleh            #+#    #+#             */
/*   Updated: 2026/02/15 14:17:19 by esaleh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	update_insert_data(int *data, int idx, int pos, int target)
{
	if (idx < data[2])
	{
		data[2] = idx;
		data[0] = pos;
	}
	if (idx > target && idx < data[3])
	{
		data[3] = idx;
		data[1] = pos;
	}
}

int	find_insert_pos_a(t_node *a, int index)
{
	int	pos;
	int	data[4];

	pos = 0;
	data[0] = 0;
	data[1] = -1;
	data[2] = 2147483647;
	data[3] = 2147483647;
	while (a)
	{
		update_insert_data(data, a->index, pos, index);
		pos++;
		a = a->next;
	}
	if (data[1] != -1)
		return (data[1]);
	return (data[0]);
}
