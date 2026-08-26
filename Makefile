NAME	= minishell

# Compiler and compilation flags
CC		= cc
CFLAGS	= -Werror -Wextra -Wall -g

# Build files and directories
INC_PATH = ./

# Libft files and directories
LIBFT_PATH	= ./libs/libft/
LIBFT		= ./libs/libft/libft.a

INC = -I $(INC_PATH) -I $(LIBFT_PATH)

SRCS= ./lexer/lexer.c \
	./lexer/clean.c \
	./lexer/lexer_init.c \
	./lexer/lexical_analysis.c \
	./lexer/lexical_analysis_utils.c \
	./parser/parse.c \
	./parser/parse_utils.c \
	./parser/parse_utils2.c \
	./parser/build_path.c \
	./parser/build_path_utils.c \
	./parser/prepare_heredocs.c \
	./parser/prepare_heredocs_utils.c \
	./builtins/builtins.c \
	./builtins/ft_pwd.c \
	./builtins/ft_exit.c \
	./builtins/ft_echo.c \
	./builtins/ft_env.c \
	./builtins/ft_cd.c \
	./exe/simple_exe.c \
	./exe/work.c \
	./exe/work_utils.c \
	./exe/pipe_utils.c \
	./exe/pipe_setup.c \
	./exe/fork_child.c \
	./exe/exe.c \
	./exe/exe_run.c \
	./exe/exe_redir.c \
	./signals/signals.c \
	./signals/signals_handlers.c \
	./env/env.c \
	./env/env_utils.c \
	./env/export.c \
	./env/unset.c \
	./env/env_to_array.c \
	./expander/expander.c \
	./expander/expander_utils.c \
	./expander/expander_utils2.c \
	./main_utils.c \
	./main.c 

OBJS = $(SRCS:.c=.o)

# Main rule
all: $(LIBFT) $(NAME)

# Objects rule
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

# Project file rule
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(INC) $(LIBFT) -lreadline

# Libft rule
$(LIBFT):
	make -C $(LIBFT_PATH)

# Clean up build files rule
clean:
	rm -f $(OBJS)

# Remove program executable
fclean: clean
	rm -f $(NAME)

# Clean + remove executable
re: fclean all

.PHONY: all re clean fclean