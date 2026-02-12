NAME		= push_swap
CC		= cc
CFLAGS		= -Wall -Wextra -Werror -I include -I libft

SRCS		= src/main.c \
		src/stack.c \
		src/stack_utils.c \
		src/ops_swap.c \
		src/ops_push.c \
		src/ops_rotate.c \
		src/ops_reverse.c \
		src/parse.c \
		src/parse_utils.c \
		src/indexing.c \
		src/assign_indexes.c \
		src/debug.c \
		src/utils.c \
		src/sort_small.c \
		src/sort_small_utils.c \
		src/sort_chunk.c \
		src/sort_cost.c \
		src/sort_insert.c \
		src/sort_rotate_plan.c \
		src/sort_pushback.c \
		src/sort_chunk_utils.c

OBJS		= $(SRCS:.c=.o)
LIBFT		= libft/libft.a

all: $(NAME)

$(LIBFT):
	$(MAKE) -C libft

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

clean:
	$(MAKE) -C libft clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C libft fclean
	rm -f $(NAME)

re: fclean all

bonus: all

.PHONY: all clean fclean re bonus
