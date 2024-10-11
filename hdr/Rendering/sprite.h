
#ifndef SPRITE_H
# define SPRITE_H

# include "mesh.h"
# include "ShaderClass.h"

class GLSprite
{
	private:
		Shader *shader = NULL;
		float alpha = 1.0f;
	public:
		Mesh mesh;
		GLSprite(glm::vec2 pos, glm::vec2 dim, GLuint sprite, Shader *shader);
		void Draw();
		void DrawRect(glm::vec4 dest, glm::vec4 rect);
		void SetAlpha(float alpha) {GLSprite::alpha = alpha;};
		void SetDest(glm::vec4 dest);
		void SetRect(glm::vec4 rect);
};

void InitGLSprite(Shader &shader);

#endif
