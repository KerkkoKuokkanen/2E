
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
		t_BoundingB boundBox;
		t_BoundingB rotatedBoundBox;
		float alpha = 1.0f;
		float angle = 0.0f;
		float width = 0.0f;
		float height = 0.0f;
		t_Point pivotPoint = {0.0f, 0.0f};
		t_Point position = {0.0f, 0.0f};
		void SetRotatedBoundBox();
	public:
		Mesh mesh;
		GLShape(std::vector<Vertex> &verts, std::vector<GLuint> &inds, GLuint texture, Shader *shader, t_BoundingB boundingBox, int useType);
		~GLShape() {Delete();};
		void SetPosition(float x, float y);
		void SetRotation(float angle);
		void SetHeight(float h);
		void SetWidth(float w);
		t_BoundingB &GetBoundingBox() {return(boundBox);};
		float GetAngle() {return(angle);};
		float GetAlpha() {return(alpha);};
		void Draw();
		void Delete();
};

t_DataForShape CreateGLShapeData(std::vector<float> &points);
GLShape *CreateGLShape(std::vector<float> &points, GLuint texture, int useType);
void InitShapes(Shader *shaderProgram);

#endif
