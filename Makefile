CC= cc
CFLAGS= -g
LIBS= ./libs/libft/libft.a
SRCS= test.c \
	./parse/parse_utils.c \
	./parse/parse.c
NAME= minishell

$(NAME): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LIBS) -o $(NAME)
clean:

fclean: clean
	rm -f $(NAME)
re: fclean $(NAME)
