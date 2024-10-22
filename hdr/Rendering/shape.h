
#ifndef SHAPE_H
# define SHAPE_H

# include "mesh.h"
# include "ShaderClass.h"
# include <glm/gtc/matrix_transform.hpp>
# include "box.h"

class GLShape
{
	private:
		Shader *shader = NULL;
		float alpha = 1.0f;
		float scaleFactor = 1.0f;
		t_Box bBox = {0.0f, 0.0f, 0.0f, 0.0f};
		int vertexAmount = 0;
	public:
		Mesh mesh;
		GLShape(std::vector<Vertex> &verts, std::vector<GLuint> &inds, GLuint texture, Shader *shader, t_Box boundingBox);
		~GLShape() {Delete();};
		void SetPosition(float x, float y);
		void SetScaleFactor(float scaleFactor) {GLShape::scaleFactor = scaleFactor;};
		void Draw();
		void Delete();
};

GLShape *CreateGLShape(std::vector<float> &points, GLuint texture);
void InitShapes(Shader *shaderProgram);

#endif
