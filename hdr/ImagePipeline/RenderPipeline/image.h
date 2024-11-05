
#ifndef IMAGE_H
# define IMAGE_H

# include "sprite.h"
# include "renderSystem.h"

class Image : public RenderObj
{
	private:
		GLSprite *sprite = NULL;
	public:
		Image(GLuint texture, t_Box rect, float angle, int layer);
		~Image();
		GLSprite *GetAccessToGLSprite() {return (sprite);};
		void Draw() override;

};

void InitImage(Shader *usedShader);

#endif
