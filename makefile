
NAME = 2E

SRCS =	srcs/main.cpp srcs/init.cpp srcs/GL_Stuff/EBO.cpp srcs/GL_Stuff/ShaderClass.cpp srcs/GL_Stuff/mesh.cpp \
		srcs/Rendering/sprite.cpp srcs/GL_Stuff/VAO.cpp srcs/GL_Stuff/VBO.cpp

OBJ = $(SRCS:.cpp=.o)
DEP = $(OBJ:.o=.d)

HDR =	-I hdr/GL_Stuff -I hdr/ -I hdr/Rendering

FLAGS = -std=c++11 -I/opt/homebrew/Cellar/glm/1.0.1/include -O2 -g -DGL_SILENCE_DEPRECATION
CGFLAGS = 
INCLUDES	=	-I./frameworks/SDL2.framework/Versions/A/Headers \
				-I./frameworks/SDL2_image.framework/Versions/A/Headers \
				-I./frameworks/SDL2_mixer.framework/Versions/A/Headers \
				-I./frameworks/SDL2_ttf.framework/Versions/A/Headers \
				-F./frameworks/
FRAMEWORKS	=	-F./frameworks \
				-rpath ./frameworks \
				 \
				-framework SDL2 -framework SDL2_image \
				-framework SDL2_mixer \
				-framework SDL2_ttf \
				-framework OpenGL 

all: $(NAME)

-include $(DEP)

$(NAME): $(OBJ)
	@g++ $(FLAGS) -fsanitize=address $(CGFLAGS) $(FRAMEWORKS) $(OBJ) -o $(NAME)

.cpp.o:
	@g++ $(FLAGS) -fsanitize=address $(INCLUDES) $(HDR) -MMD -MP -MT $@ -c $< -o $@

clean:
	@rm -rf $(OBJ) $(DEP)

fclean: clean
	@rm -rf $(NAME)

re: fclean all