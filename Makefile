SRCS_FILES = main.c exit_utils.c free.c signals.c path_init.c \
			pwd_handle.c export_handle.c \
			cmd_exec.c cmd_utils.c cmd_init.c built_in.c cd.c echo.c exit.c \
			lexer.c lexer2.c parse.c expander.c \
			heredoc.c unset.c  setup_io.c executer.c print_err.c\


SRCS = $(addprefix srcs/, $(SRCS_FILES))

OBJS_DIR = objs
OBJS_FILES = $(SRCS_FILES:.c=.o)
OBJS = $(addprefix $(OBJS_DIR)/, $(OBJS_FILES))

CC = cc
CFLAGS = -Wall -Wextra  -g -I inc -I libft


NAME = minishell

LIBS = -L libft -lft -lreadline
LIBFT = libft/libft.a

all: $(NAME) $(LIBFT)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(LIBFT):
	make -C libft

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: srcs/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re
