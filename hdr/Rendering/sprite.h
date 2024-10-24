
#ifndef SPRITE_H
# define SPRITE_H

# include "mesh.h"
# include "ShaderClass.h"
# include <glm/gtc/matrix_transform.hpp>
# include "box.h"

class GLSprite
{
	private:
		Shader *shader = NULL;
		float alpha = 1.0f;
		float scaleFactor = 1.0f;
		float angle = 0.0f;
		float w = 0.0f, h = 0.0f;
		t_Box bBox = {0.0f, 0.0f, 0.0f, 0.0f};
		void SetBoundingBox(Vertex *vertData);
		void RotateGLSprite(float angle, Vertex *vertData);
	public:
		Mesh mesh;
		GLSprite(glm::vec2 pos, glm::vec2 dim, GLuint sprite, Shader *shader);
		~GLSprite() {Delete();};
		void Draw();
		void SetAlpha(float alpha) {GLSprite::alpha = alpha;};
		void PositionSprite(float x, float y);
		void SetDest(glm::vec4 dest);
		void SetRect(glm::vec4 rect);
		void SetScaleFactor(float scaleFactor) {GLSprite::scaleFactor = scaleFactor;};
		void SetAngle(float angle);
		float GetAngle() {return (angle);};
		float GetAlpha() {return (alpha);};
		void Delete();
};

void InitGLSprite(Shader *shader);

#endif
