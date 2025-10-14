CC= cc
CFLAGS= -g
LIBS= ./libs/libft/libft.a -lreadline
SRCS= test.c \
	./parse/parse_utils.c \
	./parse/parse.c \
	./lexer/redirection.c \
	./lexer/builtins.c \
	./lexer/quotes.c
NAME= minishell

$(NAME): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LIBS) -o $(NAME)
clean:

fclean: clean
	rm -f $(NAME)
re: fclean $(NAME)
