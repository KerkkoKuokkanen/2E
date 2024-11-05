
#ifndef SPRITE_H
# define SPRITE_H

# include "shape.h"

class GLSprite
{
	private:
		GLShape *shape = NULL;
	public:
		GLSprite(glm::vec2 pos, glm::vec2 dim, GLuint sprite, Shader *shader, int useType);
		~GLSprite() {Delete();};
		void SetPosition(float x, float y) {shape->SetPosition(x, y);};
		void SetRotation(float angle) {shape->SetRotation(angle);};
		void SetWidth(float w) {shape->SetWidth(w);};
		void SetHeight(float h) {shape->SetHeight(h);};
		void Draw();
		void Delete();
};

#endif
