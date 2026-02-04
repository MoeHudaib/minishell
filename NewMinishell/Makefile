CC= cc

CFLAGS= -g

NAME =minishell

LIBS= ./libs/libft/libft.a -lreadline

SRCS= ./lexer/lexer.c \
	./lexer/clean.c \
	./lexer/lexer_init.c \
	./lexer/lexical_analysis.c \
	./lexer/main.c 

$(NAME):
	$(CC) $(CFLAGS) $(SRCS) $(LIBS) -o $(NAME)

clean:

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)
