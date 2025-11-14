NAME		= minirt

# Compiler and flags
CC			= gcc
CFLAGS		= -g3 -O3 -Ofast -ffast-math -mfpmath=sse -fno-math-errno
DEBUG_FLAGS	= -g3 -pg -fsanitize=address
INCLUDES	= -I. -I../../ -I../tupeles_test_rm

# MLX42 library
MLX42		= ../libmlx42.a
MLX_FLAGS	= -ldl -lglfw -pthread -lm

# Source files
SRCS		= making_scene/world.c \
			  making_scene/world_2.c \
			  making_scene/intersection.c \
			  making_scene/prepare_computations.c \
			  making_scene/view_transformation.c \
			  making_scene/iplementing_camera.c \
			  making_scene/shadow.c \
			  making_scene/cylinder.c \
			  making_scene/plane.c \
			  making_scene/sphere.c \
			  making_scene/cone.c \
			  making_scene/bump_map.c \
			  tupeles_test_rm/canva_1.c \
			  tupeles_test_rm/Creating_Rays.c \
			  tupeles_test_rm/light_and_shading.c \
			  tupeles_test_rm/operations_on_tuple.c \
			  tupeles_test_rm/tupels.c \
			  tupeles_test_rm/matric_1.c \
			  tupeles_test_rm/matric.c \
			  tupeles_test_rm/Transformations.c \
			  tupeles_test_rm/stack1.c \
			  tupeles_test_rm/stack2.c \
			  tupeles_test_rm/cmp_foalt.c \
			  ../garbage_collector/my_malloc.c \
				../fbenalla/miniRT/main.c     \
				../fbenalla/miniRT/linked_list.c \
				../fbenalla/miniRT/init_objects.c \
				../fbenalla/miniRT/ft_split.c \
				../fbenalla/miniRT/ft_isdigit.c \
				../fbenalla/miniRT/ft_atoi_double.c \
				../fbenalla/miniRT/ft_atoi_color.c \
				../fbenalla/miniRT/free_alloc.c\
				../fbenalla/miniRT/split_white_apces.c \
				../fbenalla/get_next_line/get_next_line_utils.c \
				../fbenalla/get_next_line/get_next_line.c \
				../linking.c \
				making_scene/passing_data.c\
				../fbenalla/miniRT/textures.c \
				../fbenalla/miniRT/plane_create.c \
				../fbenalla/miniRT/sphere_create.c \
				../fbenalla/miniRT/cylinder_create.c \
				../fbenalla/miniRT/cone_create.c \
				../fbenalla/miniRT/cone_bumpmap.c                \
				../fbenalla/miniRT/cone_create_extra.c \
				../fbenalla/miniRT/cylinder_bumpmap.c \
				../fbenalla/miniRT/cylinder_create_extra.c \
				../fbenalla/miniRT/plan_bumpmap.c \
				../fbenalla/miniRT/plane_create_extra.c \
				../fbenalla/miniRT/sphere_bumpmap.c \
				../fbenalla/miniRT/sphere_create_extra.c \
				../fbenalla/miniRT/free_alloc_extra.c \
				../fbenalla/miniRT/free_alloc_extra_2.c \
				../fbenalla/miniRT/parssing_helper.c \
				../fbenalla/miniRT/init_ambient.c \
				../fbenalla/miniRT/init_light.c \
				../fbenalla/miniRT/camera_init.c \
				../fbenalla/miniRT/verify_data.c \
				../fbenalla/miniRT/ft_atoi.c \
				making_scene/passing_data_cone.c \
				making_scene/passing_data_cy.c \
				making_scene/passing_data_pl.c \
				making_scene/passing_data_sp.c \
# Object files
OBJS		= $(SRCS:.c=.o)

# Colors for output
GREEN		= \033[0;32m
RED			= \033[0;31m
BLUE		= \033[0;34m
YELLOW		= \033[0;33m
RESET		= \033[0m

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(BLUE)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(OBJS) $(MLX42) $(MLX_FLAGS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) compiled successfully!$(RESET)"

%.o: %.c
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -f $(OBJS)
	@echo "$(GREEN)✓ Object files cleaned!$(RESET)"

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@echo "$(GREEN)✓ $(NAME) removed!$(RESET)"

re: fclean all

run: all
	@echo "$(BLUE)Running $(NAME)...$(RESET)"
	@./$(NAME)

# Specific build without sanitizer (for performance testing)
release: CFLAGS = -Wall -Wextra -Werror -O3 -Ofast -ffast-math -mfpmath=sse -fno-math-errno
release: DEBUG_FLAGS =
release: fclean $(NAME)
	@echo "$(GREEN)✓ Release build completed!$(RESET)"

# Specific build with debug symbols only (no sanitizer)
debug: DEBUG_FLAGS = -g3
debug: fclean $(NAME)
	@echo "$(GREEN)✓ Debug build completed!$(RESET)"

# Show help
help:
	@echo "$(BLUE)Available targets:$(RESET)"
	@echo "  $(GREEN)all$(RESET)      - Build minirt (default)"
	@echo "  $(GREEN)clean$(RESET)    - Remove object files"
	@echo "  $(GREEN)fclean$(RESET)   - Remove object files and executable"
	@echo "  $(GREEN)re$(RESET)       - Rebuild everything"
	@echo "  $(GREEN)run$(RESET)      - Build and run minirt"
	@echo "  $(GREEN)release$(RESET)  - Build optimized version without debug symbols"
	@echo "  $(GREEN)debug$(RESET)    - Build with debug symbols only (no sanitizer)"
	@echo "  $(GREEN)help$(RESET)     - Show this help message"

.PHONY: all clean fclean re run release debug help