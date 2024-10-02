
#ifndef SPRITE_H
# define SPRITE_H

# include "mesh.h"
# include "ShaderClass.h"

class Sprite
{
private:
	Shader *shader = NULL;
public:
	Mesh mesh;
	Sprite(glm::vec2 pos, glm::vec2 dim, GLuint sprite, Shader &shader);
	void Draw();
	void DrawRect(glm::vec4 dest, glm::vec4 rect);
};

#endif
