CC = cc
CFLAGS = -fsanitize=address -g3 
RM = rm -f
NAME = test
SRCF = main.c rays.c
OBJS = $(SRCF:.c=.o)

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