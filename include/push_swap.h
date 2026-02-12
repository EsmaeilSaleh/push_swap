/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esmaeil <esmaeil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 00:00:00 by esmaeil           #+#    #+#             */
/*   Updated: 2026/01/18 00:00:00 by esmaeil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <stdlib.h>
# include <unistd.h>
# include "../libft/libft.h"

typedef struct s_node
{
	int				value;
	int				index;
	struct s_node	*next;
}	t_node;

/* parse */
void	parse_args(t_node **a, int argc, char **argv);
void	assign_indexes(t_node *a);
int		is_overflow(long res, int sign);
int		parse_sign(char *s, int *i);
int		has_duplicate(t_node *a, int value);
void	free_split_local(char **split);
int		parse_int(char *s, int *out);
void	sort_array(int *arr, int size);

/* stack */
t_node	*node_new(int value);
void	stack_add_back(t_node **stack, t_node *new_node);
void	stack_add_back_fast(t_node **stack, t_node **tail, t_node *new_node);
int		stack_size(t_node *stack);
int		is_sorted(t_node *stack);
void	free_stack(t_node **stack);

/* ops */
void	sa(t_node **a, int print);
void	sb(t_node **b, int print);
void	ss(t_node **a, t_node **b, int print);
void	pa(t_node **a, t_node **b, int print);
void	pb(t_node **a, t_node **b, int print);
void	ra(t_node **a, int print);
void	rb(t_node **b, int print);
void	rr(t_node **a, t_node **b, int print);
void	rra(t_node **a, int print);
void	rrb(t_node **b, int print);
void	rrr(t_node **a, t_node **b, int print);
void	rotate_stack(t_node **s);
void	rev_rotate_stack(t_node **s);

/* sort */
void	sort_small(t_node **a, t_node **b);
void	chunk_sort(t_node **a, t_node **b, int size);
void	sort_three_cases(t_node **a, int first, int second, int third);
int		min_pos(t_node *a);
int		find_min_pos(t_node *a);
void	rotate_to_min(t_node **a);
int		find_max_pos(t_node *b);
int		get_rot_value(int pos, int size);
int		calc_move_cost(int rot_a, int rot_b);
int		find_insert_pos_a(t_node *a, int index);
void	exec_combined_rotations(t_node **a, t_node **b, int *rot_a, int *rot_b);
void	push_back_all(t_node **a, t_node **b);

/* utils */
void	error_exit(t_node **a, t_node **b, char **split);

/* debug */
void	debug_log_op(const char *op, t_node *a, t_node *b);

#endif
