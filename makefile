NAME = 2E

# List all source directories
SRC_DIRS = srcs srcs/ImagePipeline/GL_Stuff srcs/ImagePipeline/Rendering srcs/Tools srcs/Tools/input

# Find all .cpp and .c files in the listed source directories
SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp $(dir)/*.c))

# Create a list of object files by replacing .cpp and .c extensions with .o
OBJ = $(SRCS:.cpp=.o)
OBJ := $(OBJ:.c=.o)

# Create a list of dependency files
DEP = $(OBJ:.o=.d)

# Header directories
HDR =	-I hdr/GL_Stuff -I hdr/ -I hdr/Rendering -I hdr/Tools -I frameworks/libtess2/Include \
		-I hdr/Tools/input

# Compilation flags
FLAGS = -std=c++11 -I/opt/homebrew/Cellar/glm/1.0.1/include -g -DGL_SILENCE_DEPRECATION
CGFLAGS = 

# Framework and library includes
INCLUDES = -I./frameworks/SDL2.framework/Versions/A/Headers \
		   -I./frameworks/SDL2_image.framework/Versions/A/Headers \
		   -I./frameworks/SDL2_mixer.framework/Versions/A/Headers \
		   -I./frameworks/SDL2_ttf.framework/Versions/A/Headers \
		   -F./frameworks/

FRAMEWORKS = -F./frameworks \
			 -rpath ./frameworks \
			 -framework SDL2 -framework SDL2_image \
			 -framework SDL2_mixer \
			 -framework SDL2_ttf \
			 -framework OpenGL 

# Path to external library
LIBTESS_PATH = frameworks/libtess2/Source/libtess2.a

# Main target
all: $(NAME)

# Include the dependency files
-include $(DEP)

# Link the program
$(NAME): $(OBJ)
	@g++ $(FLAGS) $(CGFLAGS) $(FRAMEWORKS) $(OBJ) $(LIBTESS_PATH) -lm -o $(NAME)

# Compile C++ files to object files
%.o: %.cpp
	@g++ $(FLAGS) $(INCLUDES) $(HDR) -MMD -MP -MT $@ -c $< -o $@

# Compile C files to object files
%.o: %.c
	@gcc $(FLAGS) $(HDR) -MMD -MP -MT $@ -c $< -o $@

# Clean up object and dependency files
clean:
	@rm -rf $(OBJ) $(DEP)

# Full clean
fclean: clean
	@rm -rf $(NAME)

# Rebuild everything
re: fclean all

# Sanitize build
fsan: fclean
fsan: FLAGS += -fsanitize=address
fsan: $(NAME)