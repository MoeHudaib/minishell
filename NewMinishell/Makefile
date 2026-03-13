CC= cc

CFLAGS= -g

NAME =minishell

LIBS= ./libs/libft/libft.a -lreadline

SRCS= ./lexer/lexer.c \
	./lexer/clean.c \
	./lexer/lexer_init.c \
	./lexer/lexical_analysis.c \
	./parser/parse.c \
	./parser/parse_utils.c \
	./builtins/builtins.c \
	./builtins/decider.c \
	./builtins/ft_pwd.c \
	./builtins/ft_exit.c \
	./builtins/ft_echo.c \
	./builtins/ft_env.c \
	./builtins/ft_cd.c \
	./exe/simple_exe.c \
	./lexer/organize.c \
	./env/env.c \
	./env/export.c \
	./env/unset.c \
	./env/env_to_array.c \
	./expander/expander.c \
	./expander/expander_utils.c \
	./lexer/main.c 

$(NAME):
	$(CC) $(CFLAGS) $(SRCS) $(LIBS) -o $(NAME)

clean:

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)
