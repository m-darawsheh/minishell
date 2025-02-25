NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror

SRCS = main.c \

LIBS = -L libft -lft

LIBFT = libft/libft.a

OBJS = $(SRCS:.c=.o)


all: $(NAME) $(LIBFT)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(LIBFT):
	make -C libft

clean:
	rm -f $(OBJS)
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re
