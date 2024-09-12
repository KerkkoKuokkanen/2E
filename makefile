
NAME = 2E

SRCS =	srcs/main.cpp srcs/init.cpp

OBJ = $(SRCS:.cpp=.o)
DEP = $(OBJ:.o=.d)

HDR =	

FLAGS = -std=c++11 -O2 -g -DGL_SILENCE_DEPRECATION
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