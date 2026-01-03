CC= cc

CFLAGS= -g

LIBS= ./libs/libft/libft.a -lreadline

SRCS= ./lexer/lexer.c \
	./lexer/clean.c \
	./lexer/lexer_init.c \
	./lexer/lexical_analysis.c

$(NAME): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LIBS) -o $(NAME)

clean:

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)