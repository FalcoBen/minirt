CC = cc
CFLAGS = -fsanitize=address -g3 
RM = rm -f
NAME = miniRT
SRCF = main.c get_next_line/get_next_line.c get_next_line/get_next_line_utils.c linked_list.c ft_split.c split_white_apces.c \
			init_objects.c ft_atoi_double.c ft_atoi_color.c printer.c initialize_scenes.c
OBJS = $(SRCF:.c=.o)
LIBFT = libft/libft.a

all : $(NAME) 

# $(LIBFT) :
# 	make -C libft

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean :
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all