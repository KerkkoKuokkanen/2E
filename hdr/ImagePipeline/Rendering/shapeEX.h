
#ifndef SHAPE_EX_H
# define SHAPE_EX_H

# include "shape.h"

class GLShapeEX : public GLShape
{
	private:
		Vertex *BindOwn();
		void UnBindOwn();
		t_Point center;
		int vertexAmount;
		t_BoundingB defaultEdges;
		std::vector<Vertex> *ogVertexData;
		std::vector<GLuint> *ogIndexData;
		void SetTextRotOwn(Vertex *vertData, float angle);
		void SetTextureDistance(Vertex *vertData, float distance);
		void SetTexturePosition(Vertex *vertData, float x, float y);
		void SetTextureRotation(Vertex *vertData, float angle);
		float angle = 0.0f;
	public:
		GLShapeEX(std::vector<Vertex> &verts, std::vector<GLuint> &inds, GLuint texture, Shader *shader, t_BoundingB boundingBox, int useType);
		void SetAll(float x, float y, float distance, float angle);
		t_DataForShape GetCurrentShapeDataSet();
		~GLShapeEX();
};

#endif
