CC= cc

CFLAGS= -g

NAME =minishell

LIBS= ./libs/libft/libft.a -lreadline

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

$(NAME):
	$(CC) $(CFLAGS) $(SRCS) $(LIBS) -o $(NAME)

clean:

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)
