
#ifndef SHAPE_H
# define SHAPE_H

# include "mesh.h"
# include "ShaderClass.h"
#include <glm/gtc/matrix_transform.hpp>

class GLShape
{
	private:
		Shader *shader = NULL;
		float alpha = 1.0f;
		glm::mat4 transform = glm::mat4(1.0f);
	public:
		Mesh mesh;
		GLShape(std::vector<Vertex> &verts, std::vector<GLuint> &inds, GLuint texture, Shader *shader);
		~GLShape() {Delete();};
		void Draw();
		void Delete();
};

GLShape *CreateGLShape(std::vector<float> &points, GLuint texture);
void InitShapes(Shader *shaderProgram);

#endif
