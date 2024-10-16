
NAME = 2E

SRCS =	srcs/main.cpp srcs/Tools/init.cpp srcs/GL_Stuff/EBO.cpp srcs/GL_Stuff/ShaderClass.cpp srcs/GL_Stuff/mesh.cpp \
		srcs/Rendering/sprite.cpp srcs/GL_Stuff/VAO.cpp srcs/GL_Stuff/VBO.cpp srcs/GL_Stuff/TextureLoader.cpp \
		srcs/Rendering/loadTextures.cpp srcs/Tools/commonTools.cpp srcs/Tools/poller.cpp \
		srcs/tessTest.cpp

OBJ = $(SRCS:.cpp=.o)

DEP = $(CPP_OBJ:.o=.d)

HDR = -I hdr/GL_Stuff -I hdr/ -I hdr/Rendering -I hdr/Tools -I frameworks/libtess2/Include

FLAGS = -std=c++11 -I/opt/homebrew/Cellar/glm/1.0.1/include -g -DGL_SILENCE_DEPRECATION
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

LIBTESS_PATH = frameworks/libtess2/Source/libtess2.a

all: $(NAME)

-include $(DEP)

$(NAME): $(OBJ)
	@g++ $(FLAGS) $(CGFLAGS) $(FRAMEWORKS) $(OBJ) $(LIBTESS_PATH) -lm -o $(NAME)

.cpp.o:
	@g++ $(FLAGS) $(INCLUDES) $(HDR) -MMD -MP -MT $@ -c $< -o $@

.c.o:
	@gcc $(FLAGS) $(HDR) -MMD -MP -MT $@ -c $< -o $@

clean:
	@rm -rf $(OBJ) $(DEP)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

fsan: fclean
fsan: FLAGS += -fsanitize=address
fsan: $(NAME)
