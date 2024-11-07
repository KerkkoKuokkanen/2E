
#ifndef SPRITE_H
# define SPRITE_H

# include "shape.h"

class GLSprite
{
	private:
		GLShape *shape = NULL;
		t_Point usedPosition = {0.0f, 0.0f};
	public:
		GLSprite(glm::vec2 pos, glm::vec2 dim, GLuint sprite, Shader *shader, int useType);
		~GLSprite() {Delete();};
		t_BoundingB GetBoundingB() {return (shape->GetRotatedBoundingBox());};
		void SetPosition(float x, float y) {shape->SetPosition(x, y);};
		void SetRotation(float angle) {shape->SetRotation(angle);};
		void SetWidth(float w) {shape->SetWidth(w);};
		void SetHeight(float h) {shape->SetHeight(h);};
		void SetColor(float r, float g, float b, float a) {shape->SetColor(r, g, b, a);};
		void RemoveOverScreenDetection() {shape->RemoveOverScreenDetection();};
		void SetTexture(GLuint texture) {shape->SetTexture(texture);};
		void Draw();
		void Delete();
};

#endif
