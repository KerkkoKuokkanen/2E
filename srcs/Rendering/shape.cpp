
#include "shape.h"
#include "tesselator.h"
#include <glm/gtc/type_ptr.hpp>

#define BIG_VALUE 9999999.9f
#define SMALL_VALUE -9999999.9f

Shader *defaultShader;
static int alphaLocation = 0;
static int transformLocation = 0;

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

GLShape *CreateGLShape(std::vector<float> &points, GLuint texture)
{
	TESSalloc allocator = {0};
	allocator.memalloc = libtessAlloc;
	allocator.memrealloc = libtessRealloc;
	allocator.memfree = libtessFree;
	TESStesselator* tess = tessNewTess(&allocator);

	const unsigned int size = (int)points.size();
	float *vertecies = (float*)malloc(sizeof(float) * size);
	for (int i = 0; i < size; i++)
		vertecies[i] = points[i];
	tessAddContour(tess, 2, vertecies, sizeof(float) * 2, 8);
	tessTesselate(tess, TESS_WINDING_NONZERO, TESS_POLYGONS, 3, 2, NULL);

	int numVertices = tessGetVertexCount(tess);
	int numIndecies = tessGetElementCount(tess);

	const float* tessVertices = tessGetVertices(tess);
	const int* tessIndices = tessGetElements(tess);

	std::vector<GLuint> indexVector(tessIndices, tessIndices + (numIndecies * 3));
	std::vector<Vertex> verts;
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
		verts.push_back(vert);
		count += 2;
	}
	free(vertecies);
	GLShape *shape = new GLShape(verts, indexVector, texture, defaultShader, {xMin, yMax, xMax - xMin, yMax - yMin});

	//GLShape *shape = new GLShape(verts, indexVector, texture, defaultShader, glm::vec4(xMin, yMax, xMax, yMin));
	return (shape);
}

GLShape::GLShape(std::vector<Vertex> &verts, std::vector<GLuint> &inds, GLuint texture, Shader *shader, t_Box boundingBox)
{
	vertexAmount = (int)verts.size();
	GLShape::shader = shader;
	mesh.CreateMesh(verts, inds, texture);
	bBox = boundingBox;
}

void GLShape::Draw()
{
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
	float addX = x - bBox.x;
	float addY = y - bBox.y;
	bBox.x = x;
	bBox.y = y;
	mesh.VAO.Bind();
	mesh.VBO.Bind();
	Vertex* vertData = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	for (int i = 0; i < vertexAmount; i++)
	{
		vertData[i].position.x += addX;
		vertData[i].position.y += addY;
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
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
