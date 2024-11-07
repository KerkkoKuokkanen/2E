
#ifndef SHAPE_H
# define SHAPE_H

# include "mesh.h"
# include "ShaderClass.h"
# include <glm/gtc/matrix_transform.hpp>
# include "box.h"

namespace n_DrawUseTypes
{
	enum {
		DATA_STAYS_MOSTLY_THE_SAME,
		DATA_CHANGES_LOT,
		ONE_USE_ONLY
	};
};

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
		glm::vec4 imageColor = {1.0f, 1.0f, 1.0f, 1.0f};
		float angle = 0.0f;
		float width = 0.0f;
		float height = 0.0f;
		t_Point pivotPoint = {0.0f, 0.0f};
		t_Point position = {0.0f, 0.0f};
		void SetRotatedBoundBox();
		bool detectOverScreenOff = false;
	protected:
		t_Point GetPivotPoint() {return (pivotPoint);};
	public:
		Mesh mesh;
		GLShape(std::vector<Vertex> &verts, std::vector<GLuint> &inds, GLuint texture, Shader *shader, t_BoundingB boundingBox, int useType);
		~GLShape() {Delete();};
		void RemoveOverScreenDetection() {detectOverScreenOff = true;};
		void SetPosition(float x, float y);
		void SetRotation(float angle);
		void SetHeight(float h);
		void SetWidth(float w);
		void SetColor(float r, float g, float b, float a) {imageColor = {r, g, b, a};};
		t_BoundingB GetBoundingBox() {return(boundBox);};
		t_BoundingB GetRotatedBoundingBox() {return(rotatedBoundBox);};
		float GetAngle() {return(angle);};
		glm::vec4 GetColor() {return(imageColor);};
		float GetWidth() {return(width);};
		float GetHeight() {return(height);};
		void Draw();
		void Delete();
};

t_DataForShape CreateGLShapeData(std::vector<float> &points);
GLShape *CreateGLShape(std::vector<float> &points, GLuint texture, int useType);
void InitShapes(Shader *shaderProgram);

#endif
