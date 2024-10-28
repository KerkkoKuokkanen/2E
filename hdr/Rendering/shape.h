
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
	t_BoundingB			bBox;
}				t_DataForShape;

class GLShape
{
	private:
		Shader *shader = NULL;
		float alpha = 1.0f;
		t_BoundingB boundBox;
		int vertexAmount = 0;
		float angle = 0.0f;
		void RotateGLShape(float angle, Vertex *vertData);
		Vertex *BindOwn();
		void UnBindOwn();
		void SetHeightOwn(float h, Vertex *vertData);
		void SetWidthOwn(float w, Vertex *vertData);
	public:
		Mesh mesh;
		GLShape(std::vector<Vertex> &verts, std::vector<GLuint> &inds, GLuint texture, Shader *shader, t_BoundingB boundingBox);
		~GLShape() {Delete();};
		void SetPosition(float x, float y);
		void SetRotation(float angle);
		void SetHeight(float h);
		void SetWidth(float w);
		void SetScale(float scale);
		t_BoundingB GetBoundingBox() {return(boundBox);};
		float GetAngle() {return(angle);};
		float GetAlpha() {return(alpha);};
		void Draw();
		void Delete();
};

t_DataForShape CreateGLShapeData(std::vector<float> &points);
GLShape *CreateGLShape(std::vector<float> &points, GLuint texture);
void InitShapes(Shader *shaderProgram);

#endif
