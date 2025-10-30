CC = cc
CFLAGS =  -fsanitize=address -g3 
# CFLAGS =   -g3 
RM = rm -f
NAME = miniRT
SRCF = main.c ../get_next_line/get_next_line.c ../get_next_line/get_next_line_utils.c linked_list.c ft_split.c split_white_apces.c \
			init_objects.c ft_atoi_double.c ft_atoi_color.c printer.c initialize_scenes.c mlx_start.c \
			camera.c vec3_math.c debug_prints.c world.c free_alloc.c print_debug.c textures.c ft_isdigit.c
OBJS = $(SRCF:.c=.o)
LIBFT = libft/libft.a

LIBMLX = ../MLX42/build/libmlx42.a
LIBS = -L$(HOME)/goinfre/homebrew/lib -lglfw -lm 

all : $(NAME) 

$(NAME) : $(OBJS) $(LIBMLX)
	$(CC) $(CFLAGS) $(OBJS) $(LIBMLX) $(LIBS) -o $(NAME)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean :
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all