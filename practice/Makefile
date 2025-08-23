CC      = cc
CFLAGS  =  -I MLX42/include -fsanitize=address -g3
NAME    = game
RM      = rm -f
SRCF    = chess_board.c #coordonne.c 
OBJS    = $(SRCF:.c=.o) 

LIBMLX  = MLX42/build/libmlx42.a
LIBS = -L$(HOME)/goinfre/homebrew/lib -lglfw -ldl -lm



all: $(NAME)

$(NAME): $(OBJS) $(LIBMLX)
	$(CC) $(CFLAGS) $(OBJS) $(LIBMLX) $(LIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all


