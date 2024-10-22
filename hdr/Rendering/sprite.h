
#ifndef SPRITE_H
# define SPRITE_H

# include "mesh.h"
# include "ShaderClass.h"
#include <glm/gtc/matrix_transform.hpp>

class GLSprite
{
	private:
		Shader *shader = NULL;
		float alpha = 1.0f;
		float scaleFactor = 1.0f;
	public:
		Mesh mesh;
		GLSprite(glm::vec2 pos, glm::vec2 dim, GLuint sprite, Shader *shader);
		~GLSprite() {Delete();};
		void Draw();
		void SetAlpha(float alpha) {GLSprite::alpha = alpha;};
		void SetDest(glm::vec4 dest);
		void SetRect(glm::vec4 rect);
		void SetScaleFactor(float scaleFactor) {GLSprite::scaleFactor = scaleFactor;};
		void Delete();
};

void InitGLSprite(Shader *shader);

#endif
