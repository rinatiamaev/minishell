/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 01:23:16 by nlouis            #+#    #+#             */
/*   Updated: 2025/01/31 08:05:39 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <stdarg.h>
# include <stdbool.h>
# include <errno.h>

# define FLAG_HASH    1
# define FLAG_ZERO    2
# define FLAG_MINUS   4
# define FLAG_PLUS    8
# define FLAG_SPACE   16

typedef struct s_fmt
{
	int		flags;
	int		width;
	int		precision;
	char	specifier;
}	t_fmt;

typedef struct s_pf
{
	int		total;
	int		write_error;
	int		malloc_error;
	t_fmt	*fmt;
}	t_pf;

# define BUFFER_SIZE 1024
# define MAX_FD 1024

# define ATOI_ERROR 9223372036854775807LL

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}				t_list;

char		**ft_copy_array(char **array);
void		ft_free_array(void **array);
int			ft_putintarray(int *array, int size);
char		**ft_split(const char *s, char c);
void		ft_qsort(int *array, int size);

t_list		*ft_lstnew(void *content);
void		ft_lstadd_front(t_list **lst, t_list *new);
int			ft_lstsize(t_list *lst);
t_list		*ft_lstlast(t_list *lst);
void		ft_lstadd_back(t_list **lst, t_list *new);
void		ft_lstdelone(t_list *lst, void (*del)(void*));
void		ft_lstclear(t_list **lst, void (*del)(void*));
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
void		ft_del(void *content);

int			ft_isalnum(int c);
int			ft_isalpha(int c);
int			ft_isascii(int c);
int			ft_isdigit(int c);
bool		ft_is_valid_number(const char *str);
int			ft_isprint(int c);
int			ft_ispunct(int c);
int			ft_isspace(int c);
void		skip_whitespace_index(const char *input, int *i);
int			ft_islower(int c);
int			ft_isupper(int c);
int			ft_issign(int c);
int			ft_isstrpositive(char *str);

void		*ft_calloc(size_t count, size_t size);
void		*ft_realloc(void *ptr, size_t old_size, size_t new_size);
void		ft_bzero(void *s, size_t n);
void		*ft_memchr(const void *buffer, int c, size_t count);
int			ft_memcmp(const void *buffer1, const void *buffer2, size_t count);
void		*ft_memcpy(void *dest, const void *src, size_t count);
void		*ft_memmove(void *dest, const void *src, size_t count);
void		*ft_memset(void *dest, int c, size_t count);

void		ft_putchar_fd(char c, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);
int			ft_putstr_fd(char *s, int fd);

long long	ft_atoi(const char *str);
long long	ft_atoll(const char *str);
char		*ft_itoa(long long n);
char		*ft_utoa(unsigned long long n);
char		*ft_itoa_base(unsigned long long n, int base, int uppercase);

char		*ft_strchr(const char *str, int c);
char		*ft_strcpy(char *dst, const char *src);
char		*ft_strdup(const char *src);
void		ft_striteri(char *s, void (*f)(unsigned int, char*));
char		*ft_strjoin(const char *s1, const char *s2);
size_t		ft_strlcat(char *dest, const char *src, size_t size);
size_t		ft_strlcpy(char *dest, const char *src, size_t size);
size_t		ft_strlen(const char *str);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int			ft_strcmp(const char *s1, const char *s2);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strndup(const char *src, size_t n);
char		*ft_strnstr(const char *big, const char *little, size_t len);
char		*ft_strrchr(const char *str, int c);
char		*ft_strtrim(const char *s1, const char *set);
char		*ft_substr(const char *s, unsigned int start, size_t len);
int			ft_tolower(int c);
int			ft_toupper(int c);

char		*get_next_line(int fd);

int			ft_printf(const char *format, ...);
int			initialize_printf_structs(t_pf *pf);
void		process_format_string(const char *format, va_list *args, t_pf *pf);
void		write_safely(int fd, char *str, t_pf *pf);
void		write_safely_len(int fd, char *str, size_t len, t_pf *pf);
void		write_char_safely(int fd, char c, t_pf *pf);
int			is_valid_flag(char c);
int			setup_format(const char **format, t_fmt *fmt);
void		print_padding(int padding, char pad_char, t_pf *pf);
int			handle_precision_s(t_fmt *fmt, int len);
int			handle_width(t_fmt *fmt, size_t len);
void		format_and_print_c(t_fmt *fmt, va_list *args, t_pf *pf);
void		format_and_print_s(t_fmt *fmt, va_list *args, t_pf *pf);
void		format_and_print_percent(t_fmt *fmt, t_pf *pf);
void		format_and_print_p(t_fmt *fmt, va_list *args, t_pf *pf);
char		*handle_precision_p(t_fmt *fmt, char *str, int *len, t_pf *pf);
void		handle_space_flag_p(t_pf *pf, int *padding);
void		handle_minus_flag_p(char *str, int padding, t_pf *pf);
void		print_right_aligned_p(t_fmt *fmt, char *str, int padding, t_pf *pf);
void		format_and_print_id(t_fmt *fmt, va_list *args, t_pf *pf);
void		format_and_print_u(t_fmt *fmt, va_list *args, t_pf *pf);
void		format_and_print_x(t_fmt *fmt, va_list *args, int uppercase, t_pf *pf);
char		*handle_precision_int(t_fmt *fmt, char *str, int *len, t_pf *pf);
void		print_formatted_number(t_fmt *fmt, char *str, int len, t_pf *pf);
int			requires_alternative_form(char specifier, char *str);
char		*prepend_alternative_form(char *str, char specifier, t_pf *pf);
int			get_alternative_form_length(char specifier);

#endif