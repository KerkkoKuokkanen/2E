
#include "shape.h"
#include "tesselator.h"
#include <glm/gtc/type_ptr.hpp>

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
	for (int i = 0; i < numVertices; i++)
	{
		Vertex vert = {glm::vec3(tessVertices[count], tessVertices[count + 1], 0.0f),
							glm::vec2(tessVertices[count], tessVertices[count + 1])};
		verts.push_back(vert);
		count += 2;
	}
	free(vertecies);
	GLShape *shape = new GLShape(verts, indexVector, texture, defaultShader);
	return (shape);
}

GLShape::GLShape(std::vector<Vertex> &verts, std::vector<GLuint> &inds, GLuint texture, Shader *shader)
{
	GLShape::shader = shader;
	mesh.CreateMesh(verts, inds, texture);
}

void GLShape::Draw()
{
	shader->Activate();
	mesh.VAO.Bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.texture);
	glm::mat4 transform = glm::mat4(1.0f);
	float scaleFactor = 1.5f;
	transform = glm::scale(transform, glm::vec3(scaleFactor, scaleFactor, scaleFactor));

	int alphaLocation = glGetUniformLocation(shader->ID, "uniformAlpha");
	glUniform1f(alphaLocation, 1.0f);
	int transformLocation = glGetUniformLocation(shader->ID, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

	glDrawElements(GL_TRIANGLES, mesh.indecies.size(), GL_UNSIGNED_INT, 0);
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
