# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fbenalla <fbenalla@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/05 12:31:20 by fbenalla          #+#    #+#              #
#    Updated: 2024/11/10 01:02:36 by fbenalla         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCF = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c \
		ft_isalpha.c ft_isascii.c  ft_isdigit.c ft_isprint.c \
		ft_itoa.c ft_memchr.c ft_memcmp.c ft_memcpy.c \
		ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c \
		ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strchr.c \
		ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c \
		ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c \
		ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c \
		ft_tolower.c ft_toupper.c

BONUSF = ft_lstadd_back_bonus.c ft_lstadd_front_bonus.c \
		ft_lstclear_bonus.c ft_lstdelone_bonus.c \
		ft_lstiter_bonus.c ft_lstlast_bonus.c ft_lstmap_bonus.c \
		ft_lstnew_bonus.c ft_lstsize_bonus.c 
 
CFLAGS = -Wall -Wextra -Werror

CC = cc

AR = ar rc

LIBFTH = libft.h

OBJC = $(SRCF:.c=.o)

OBJB = $(BONUSF:.c=.o)

RM = rm -f 

NAME = libft.a 

$(NAME) : $(OBJC)
	
all : $(NAME)

bonus : $(OBJB)

%.o : %.c $(LIBFTH)
	$(CC) $(CFLAGS) -c $<
	$(AR) $(NAME) $@

clean :
	$(RM) $(OBJC) $(OBJB)

fclean : clean
	$(RM) $(NAME)

re : fclean bonus all
