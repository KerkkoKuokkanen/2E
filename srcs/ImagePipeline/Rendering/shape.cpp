
#include "shape.h"
#include "tesselator.h"
#include "commonTools.h"
#include "convexOverlap.h"

#define BIG_VALUE 9999999.9f
#define SMALL_VALUE -9999999.9f

Shader *defaultShader;
static int alphaLocation = 0;

static void* libtessAlloc(void* userData, unsigned int size)
{
	void *ret = malloc(size);
	return (ret);
}

static void* libtessRealloc(void* userData, void* ptr, unsigned int size)
{
	void *ret = realloc(ptr, size);
	if (ptr != NULL)
		free(ptr);
	return (ret);
}

static void libtessFree(void* userData, void* ptr)
{
	free(ptr);
}

t_DataForShape CreateGLShapeData(std::vector<float> &points)
{
	t_DataForShape shapeData;

	TESSalloc allocator = {0};
	allocator.memalloc = libtessAlloc;
	allocator.memrealloc = libtessRealloc;
	allocator.memfree = libtessFree;
	TESStesselator* tess = tessNewTess(&allocator);

	const unsigned int size = (int)points.size();
	float *vertecies = (float*)malloc(sizeof(float) * size);
	for (int i = 0; i < size; i++)
		vertecies[i] = points[i];
	tessAddContour(tess, 2, vertecies, sizeof(float) * 2, size / 2);
	tessTesselate(tess, TESS_WINDING_NONZERO, TESS_POLYGONS, 3, 2, NULL);

	int numVertices = tessGetVertexCount(tess);
	int numIndecies = tessGetElementCount(tess);

	const float* tessVertices = tessGetVertices(tess);
	const int* tessIndices = tessGetElements(tess);

	shapeData.indexData = std::vector<GLuint>(tessIndices, tessIndices + (numIndecies * 3));
	int count = 0;
	float xMin = BIG_VALUE, yMin = BIG_VALUE;
	float xMax = SMALL_VALUE, yMax = SMALL_VALUE;
	for (int i = 0; i < numVertices; i++)
	{
		float x = tessVertices[count];
		float y = tessVertices[count + 1];
		Vertex vert = {glm::vec3(x, y, 0.0f), glm::vec2(x, y)};
		xMin = (x < xMin) ? x : xMin;
		yMin = (y < yMin) ? y : yMin;
		xMax = (x > xMax) ? x : xMax;
		yMax = (y > yMax) ? y : yMax;
		shapeData.vertexData.push_back(vert);
		count += 2;
	}
	free(vertecies);
	tessDeleteTess(tess);
	t_BoundingB box = {{xMin, yMax}, {xMax, yMax}, {xMax, yMin}, {xMin, yMin}};
	shapeData.bBox = box;
	return (shapeData);
}

GLShape *CreateGLShape(std::vector<float> &points, GLuint texture)
{
	t_DataForShape used = CreateGLShapeData(points);
	GLShape *shape = new GLShape(used.vertexData, used.indexData, texture, defaultShader, used.bBox);
	return (shape);
}

GLShape::GLShape(std::vector<Vertex> &verts, std::vector<GLuint> &inds, GLuint texture, Shader *shader, t_BoundingB boundingBox)
{
	vertexAmount = (int)verts.size();
	GLShape::shader = (shader == NULL) ? defaultShader : shader;
	mesh.CreateMesh(verts, inds, texture);
	boundBox = boundingBox;
}

static t_Point RotatePoint(t_Point xy, float centerX, float centerY, float cosTheta, float sinTheta)
{
	float xPos = xy.x - centerX;
	float yPos = xy.y - centerY;
	float xNew = xPos * cosTheta - yPos * sinTheta;
	float yNew = xPos * sinTheta + yPos * cosTheta;
	t_Point ret = {xNew + centerX, yNew + centerY};
	return (ret);
}

static t_Point GetCenter(t_BoundingB &boundBox)
{
	float xCenter = (boundBox.leftBottom.x + boundBox.leftTop.x + boundBox.rightBottom.x + boundBox.rightTop.x) / 4.0f;
	float yCenter = (boundBox.leftBottom.y + boundBox.leftTop.y + boundBox.rightBottom.y + boundBox.rightTop.y) / 4.0f;
	return ((t_Point){xCenter, yCenter});
}

void GLShape::RotateGLShape(float angle, Vertex *vertData)
{
	float cosTheta = std::cos(angle);
	float sinTheta = std::sin(angle);

	t_Point center = GetCenter(boundBox);
	float xCenter = center.x;
	float yCenter = center.y;

	for (int i = 0; i < vertexAmount; i++)
	{
		float xPos = vertData[i].position.x - xCenter;
		float yPos = vertData[i].position.y - yCenter;
		float xNew = xPos * cosTheta - yPos * sinTheta;
		float yNew = xPos * sinTheta + yPos * cosTheta;
		vertData[i].position.x = xNew + xCenter;
		vertData[i].position.y = yNew + yCenter;
	}
	boundBox.leftBottom = RotatePoint(boundBox.leftBottom, xCenter, yCenter, cosTheta, sinTheta);
	boundBox.leftTop = RotatePoint(boundBox.leftTop, xCenter, yCenter, cosTheta, sinTheta);
	boundBox.rightBottom = RotatePoint(boundBox.rightBottom, xCenter, yCenter, cosTheta, sinTheta);
	boundBox.rightTop = RotatePoint(boundBox.rightTop, xCenter, yCenter, cosTheta, sinTheta);
}

void GLShape::Draw()
{
	if (!ReactangleScreenOverlap(boundBox))
		return ;
	shader->Activate();
	mesh.VAO.Bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.texture);

	glUniform1f(alphaLocation, 1.0f);

	glDrawElements(GL_TRIANGLES, mesh.indecies.size(), GL_UNSIGNED_INT, 0);
}

void GLShape::SetPosition(float x, float y)
{
	t_Point center = GetCenter(boundBox);
	float addX = x - center.x;
	float addY = y - center.y;
	mesh.VAO.Bind();
	mesh.VBO.Bind();
	Vertex* vertData = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	for (int i = 0; i < vertexAmount; i++)
	{
		vertData[i].position.x += addX;
		vertData[i].position.y += addY;
	}
	boundBox.leftBottom = {boundBox.leftBottom.x + addX, boundBox.leftBottom.y + addY};
	boundBox.leftTop = {boundBox.leftTop.x + addX, boundBox.leftTop.y + addY};
	boundBox.rightBottom = {boundBox.rightBottom.x + addX, boundBox.rightBottom.y + addY};
	boundBox.rightTop = {boundBox.rightTop.x + addX, boundBox.rightTop.y + addY};
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void GLShape::SetRotation(float angle)
{
	if (FAlmostEqual(angle, GLShape::angle))
		return ;
	float add = angle - GLShape::angle;
	mesh.VAO.Bind();
	mesh.VBO.Bind();
	Vertex* vertData = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	RotateGLShape(add, vertData);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	GLShape::angle = angle;
}

void GLShape::Delete()
{
	mesh.Delete();
}

void InitShapes(Shader *shaderProgram)
{
	defaultShader = shaderProgram;
	alphaLocation = glGetUniformLocation(shaderProgram->ID, "uniformAlpha");
}
