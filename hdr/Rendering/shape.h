
#ifndef SHAPE_H
# define SHAPE_H

# include "mesh.h"
# include "ShaderClass.h"
# include <glm/gtc/matrix_transform.hpp>
# include "box.h"

typedef struct	s_DataForShape
{
	std::vector<Vertex>	vertexData;
	std::vector<GLuint>	indexData;
	t_Box				boundingBox;
}				t_DataForShape;

class GLShape
{
	private:
		Shader *shader = NULL;
		float alpha = 1.0f;
		float scaleFactor = 1.0f;
		t_Box bBox = {0.0f, 0.0f, 0.0f, 0.0f};
		int vertexAmount = 0;
		float angle = 0.0f;
		void SetBoundingBox(Vertex *vertData);
		void RotateGLShape(float angle, Vertex *vertData);
	public:
		Mesh mesh;
		GLShape(std::vector<Vertex> &verts, std::vector<GLuint> &inds, GLuint texture, Shader *shader, t_Box boundingBox);
		~GLShape() {Delete();};
		void SetPosition(float x, float y);
		void SetRotation(float angle);
		void SetScaleFactor(float scaleFactor) {GLShape::scaleFactor = scaleFactor;};
		void SetHeight(float h);
		void SetWidth(float w);
		t_Box GetBoundingBox() {return(bBox);};
		float GetAngle() {return(angle);};
		float GetAlpha() {return(alpha);};
		void Draw();
		void Delete();
};

t_DataForShape CreateShapeData(std::vector<float> &points);
GLShape *CreateGLShape(std::vector<float> &points, GLuint texture);
void InitShapes(Shader *shaderProgram);

#endif
