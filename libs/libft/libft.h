/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad <mohammad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 13:14:48 by mhdeeb            #+#    #+#             */
/*   Updated: 2025/10/03 00:26:57 by mohammad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

/* Includes */
# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>

/* Linked List Struct */
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

/* Linked List Functions */
t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new);
int		ft_lstsize(t_list *head);
t_list	*ft_lstlast(t_list *head);
void	ft_lstadd_back(t_list **head, t_list *new);
void	ft_lstdelone(t_list *node, void (*del)(void *));
void	ft_lstclear(t_list **head, void (*del)(void *));
void	ft_lstiter(t_list *head, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

/* Character Checks */
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);

/* Character Case */
int		ft_toupper(int c);
int		ft_tolower(int c);

/*	Math	*/
double	ft_sqrt(double x);

/* String and Memory Functions */
size_t	ft_strlen(const char *str);
void	*ft_memset(void *b, int c, size_t n);
void	ft_bzero(void *buffer, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		ft_strcmp(char *s1, char *s2);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);

/* String Functions */
size_t	ft_strlcpy(char *dst, const char *src, size_t n);
size_t	ft_strlcat(char *dst, const char *src, size_t n);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strnstr(const char *str, const char *substr, size_t n);
char	*ft_strdup(const char *s1);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char *));

/* Conversion */
int		ft_atoi(const char *str);
char	*ft_itoa(int n);

/* Memory Allocation */
void	*ft_calloc(size_t count, size_t size);

/* Write Functions */
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);

#endif
