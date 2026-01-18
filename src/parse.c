/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esmaeil <esmaeil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 00:00:00 by esmaeil           #+#    #+#             */
/*   Updated: 2026/01/18 00:00:00 by esmaeil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	parse_int(char *s, int *out)
{
	long	res;
	int		sign;
	int		i;

	i = 0;
	res = 0;
	sign = parse_sign(s, &i);
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		res = (res * 10) + (s[i] - '0');
		if (is_overflow(res, sign))
			return (0);
		i++;
	}
	*out = (int)(res * sign);
	return (1);
}

static void	add_token(t_node **a, t_node **tail, char *token, char **split)
{
	int		value;
	t_node	*node;

	if (!parse_int(token, &value))
		error_exit(a, NULL, split);
	if (has_duplicate(*a, value))
		error_exit(a, NULL, split);
	node = node_new(value);
	if (!node)
		error_exit(a, NULL, split);
	if (tail)
		stack_add_back_fast(a, tail, node);
	else
		stack_add_back(a, node);
}

static void	parse_split(t_node **a, char *arg)
{
	char	**split;
	t_node	*tail;
	int		i;

	split = ft_split(arg, ' ');
	if (!split || !split[0])
		error_exit(a, NULL, split);
	tail = NULL;
	i = 0;
	while (split[i])
		add_token(a, &tail, split[i++], split);
	free_split_local(split);
}

static void	parse_argv(t_node **a, int argc, char **argv)
{
	t_node	*tail;
	int		i;

	tail = NULL;
	i = 1;
	while (i < argc)
		add_token(a, &tail, argv[i++], NULL);
}

void	parse_args(t_node **a, int argc, char **argv)
{
	if (argc < 2)
		return ;
	if (argc == 2)
		parse_split(a, argv[1]);
	else
		parse_argv(a, argc, argv);
}
