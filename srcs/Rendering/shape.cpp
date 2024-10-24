
#include "shape.h"
#include "tesselator.h"
#include <glm/gtc/type_ptr.hpp>
#include "commonTools.h"

#define BIG_VALUE 9999999.9f
#define SMALL_VALUE -9999999.9f

Shader *defaultShader;
static int alphaLocation = 0;
static int transformLocation = 0;

static bool DrawCheck(t_Box box)
{
	if (box.x > 1.0f || (box.x + box.w) < (-1.0f))
		return (false);
	if (box.y > 1.0f || (box.y + box.h) < (-1.0f))
		return (false);
	return (true);
}

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

t_DataForShape CreateShapeData(std::vector<float> &points)
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
	shapeData.boundingBox = {xMin, yMin, xMax - xMin, yMax - yMin};
	return (shapeData);
}

GLShape *CreateGLShape(std::vector<float> &points, GLuint texture)
{
	t_DataForShape used = CreateShapeData(points);
	GLShape *shape = new GLShape(used.vertexData, used.indexData, texture, defaultShader, used.boundingBox);
	return (shape);
}

GLShape::GLShape(std::vector<Vertex> &verts, std::vector<GLuint> &inds, GLuint texture, Shader *shader, t_Box boundingBox)
{
	vertexAmount = (int)verts.size();
	GLShape::shader = (shader == NULL) ? defaultShader : shader;
	mesh.CreateMesh(verts, inds, texture);
	bBox = {boundingBox.x * scaleFactor, boundingBox.y * scaleFactor,
			boundingBox.w * scaleFactor, boundingBox.h * scaleFactor};
}

void GLShape::RotateGLShape(float angle, Vertex *vertData)
{
	float cosTheta = std::cos(angle);
	float sinTheta = std::sin(angle);

	float xCenter = bBox.x + (bBox.w / 2.0f);
	float yCenter = bBox.y + (bBox.h / 2.0f);

	for (int i = 0; i < vertexAmount; i++)
	{
		float xPos = vertData[i].position.x - xCenter;
		float yPos = vertData[i].position.y - yCenter;
		float xNew = xPos * cosTheta - yPos * sinTheta;
		float yNew = xPos * sinTheta + yPos * cosTheta;
		vertData[i].position.x = xNew + xCenter;
		vertData[i].position.y = yNew + yCenter;
	}
}

void GLShape::Draw()
{
	if (!DrawCheck(bBox))
		return ;
	shader->Activate();
	mesh.VAO.Bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.texture);
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::scale(transform, glm::vec3(scaleFactor, scaleFactor, scaleFactor));

	glUniform1f(alphaLocation, 1.0f);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

	glDrawElements(GL_TRIANGLES, mesh.indecies.size(), GL_UNSIGNED_INT, 0);
}

void GLShape::SetPosition(float x, float y)
{
	float addX = x - (bBox.x + bBox.w / 2.0f);
	float addY = y - (bBox.y + bBox.h / 2.0f);
	mesh.VAO.Bind();
	mesh.VBO.Bind();
	Vertex* vertData = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	for (int i = 0; i < vertexAmount; i++)
	{
		vertData[i].position.x += addX;
		vertData[i].position.y += addY;
	}
	SetBoundingBox(vertData);
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void GLShape::SetBoundingBox(Vertex *vertData)
{
	float xMin = BIG_VALUE, yMin = BIG_VALUE;
	float xMax = SMALL_VALUE, yMax = SMALL_VALUE;
	for (int i = 0; i < vertexAmount; i++)
	{
		float x = vertData[i].position.x;
		float y = vertData[i].position.y;
		xMin = (x < xMin) ? x : xMin;
		xMax = (x > xMax) ? x : xMax;
		yMin = (y < yMin) ? y : yMin;
		yMax = (y > yMax) ? y : yMax;
	}
	bBox = {xMin * scaleFactor, yMin * scaleFactor,
			(xMax - xMin) * scaleFactor,
			(yMax - yMin) * scaleFactor};
}

void GLShape::SetRotation(float angle)
{
	if (FAlmostEqual(angle, GLShape::angle))
		return ;
	float add = angle - GLShape::angle;
	mesh.VAO.Bind();
	mesh.VBO.Bind();
	Vertex* vertData = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	RotateGLShape(add, vertData);
	SetBoundingBox(vertData);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	GLShape::angle = angle;
}

void GLShape::SetHeight(float h)
{
	if (FAlmostEqual(h, bBox.h))
		return ;
	float add = h - bBox.h;
	float addToSide = add * 0.5f;
	float centerY = bBox.y + bBox.h * 0.5f;
	float scale = addToSide / (bBox.h * 0.5f);
	mesh.VAO.Bind();
	mesh.VBO.Bind();
	Vertex* vertData = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	for (int i = 0; i < vertexAmount; i++)
	{
		float vertY = vertData[i].position.y;
		float distFrombBoxC = vertY - centerY;
		float indAdd = scale * distFrombBoxC;
		vertY += indAdd;
		vertData[i].position.y = vertY;

	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	bBox.y -= addToSide;
	bBox.h += add;
}

void GLShape::SetWidth(float w)
{
	if (FAlmostEqual(w, bBox.w))
		return ;
	float add = w - bBox.w;
	float addToSide = add * 0.5f;
	float centerX = bBox.x + bBox.w * 0.5f;
	float scale = addToSide / (bBox.w * 0.5f);
	mesh.VAO.Bind();
	mesh.VBO.Bind();
	Vertex* vertData = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	for (int i = 0; i < vertexAmount; i++)
	{
		float vertX = vertData[i].position.x;
		float distFrombBoxC = vertX - centerX;
		float indAdd = scale * distFrombBoxC;
		vertX += indAdd;
		vertData[i].position.x = vertX;

	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	bBox.x -= addToSide;
	bBox.w += add;
}

void GLShape::Delete()
{
	mesh.Delete();
}

void InitShapes(Shader *shaderProgram)
{
	defaultShader = shaderProgram;
	alphaLocation = glGetUniformLocation(shaderProgram->ID, "uniformAlpha");
	transformLocation = glGetUniformLocation(shaderProgram->ID, "transform");
}
