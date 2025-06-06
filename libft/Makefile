# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/21 12:19:10 by nlouis            #+#    #+#              #
#    Updated: 2025/02/04 13:15:46 by nlouis           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -fPIE -I include

NAME	= libft.a

SRCS	=	srcs/array/ft_copy_strarray.c \
			srcs/array/ft_free_array.c \
			srcs/array/ft_putintarray.c \
			srcs/array/ft_split.c \
			srcs/array/ft_qsort.c \
			srcs/list/ft_lstnew.c \
			srcs/list/ft_lstadd_front.c \
			srcs/list/ft_lstsize.c \
			srcs/list/ft_lstlast.c \
			srcs/list/ft_lstadd_back.c \
			srcs/list/ft_lstdelone.c \
			srcs/list/ft_lstclear.c \
			srcs/list/ft_lstiter.c \
			srcs/list/ft_lstmap.c \
			srcs/list/ft_del.c \
			srcs/check/ft_isalnum.c \
			srcs/check/ft_isalpha.c \
			srcs/check/ft_isascii.c \
			srcs/check/ft_isdigit.c \
			srcs/check/ft_is_valid_num.c \
			srcs/check/ft_isprint.c \
			srcs/check/ft_ispunct.c \
			srcs/check/ft_isspace.c \
			srcs/check/ft_islower.c \
			srcs/check/ft_isupper.c \
			srcs/check/ft_issign.c \
			srcs/check/ft_isstrpositive.c \
			srcs/mem/ft_calloc.c \
			srcs/mem/ft_realloc.c \
			srcs/mem/ft_bzero.c \
			srcs/mem/ft_memchr.c \
			srcs/mem/ft_memcmp.c \
			srcs/mem/ft_memcpy.c \
			srcs/mem/ft_memmove.c \
			srcs/mem/ft_memset.c \
			srcs/write/ft_putchar_fd.c \
			srcs/write/ft_putendl_fd.c \
			srcs/write/ft_putnbr_fd.c \
			srcs/write/ft_putstr_fd.c \
			srcs/StrNumConv/ft_atoi.c \
			srcs/StrNumConv/ft_atoll.c \
			srcs/StrNumConv/ft_itoa.c \
			srcs/StrNumConv/ft_utoa.c \
			srcs/StrNumConv/ft_itoa_base.c \
			srcs/str/ft_strchr.c \
			srcs/str/ft_strdup.c \
			srcs/str/ft_striteri.c \
			srcs/str/ft_strjoin.c \
			srcs/str/ft_strlcat.c \
			srcs/str/ft_strlcpy.c \
			srcs/str/ft_strcpy.c \
			srcs/str/ft_strlen.c \
			srcs/str/ft_strmapi.c \
			srcs/str/ft_strcmp.c \
			srcs/str/ft_strncmp.c \
			srcs/str/ft_strndup.c \
			srcs/str/ft_strnstr.c \
			srcs/str/ft_strrchr.c \
			srcs/str/ft_strtrim.c \
			srcs/str/ft_substr.c \
			srcs/str/ft_tolower.c \
			srcs/str/ft_toupper.c \
			srcs/gnl/get_next_line.c \
			srcs/ft_printf/ft_printf.c \
			srcs/ft_printf/ft_printf_utils.c \
			srcs/ft_printf/print_csper_utils.c \
			srcs/ft_printf/print_csper.c \
			srcs/ft_printf/print_idux_hashflag.c \
			srcs/ft_printf/print_idux_precision.c \
			srcs/ft_printf/print_idux_print.c \
			srcs/ft_printf/print_idux_zero_padding.c \
			srcs/ft_printf/print_idux.c \
			srcs/ft_printf/print_p_utils.c \
			srcs/ft_printf/process_format_string.c \
			srcs/ft_printf/setup_format.c \
			srcs/ft_printf/write_safely.c

OBJS    = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	mkdir -p lib
	ar rcs $@ $^

%.o: %.c libft.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
