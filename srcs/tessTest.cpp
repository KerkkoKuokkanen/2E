
#include "tesselator.h"
#include "mesh.h"
#include "Textures.h"
#include <stdio.h>
#include <stdlib.h>

void* myAlloc(void* userData, unsigned int size) {
	return malloc(size);
}

void* myRealloc(void* userData, void* ptr, unsigned int size) {
	return realloc(ptr, size);
}

void myFree(void* userData, void* ptr) {
	free(ptr);
}

Mesh *TessalationTest()
{
	TESSalloc allocator = {0};
	allocator.memalloc = myAlloc;
	allocator.memrealloc = myRealloc;
	allocator.memfree = myFree;
	TESStesselator* tess = tessNewTess(&allocator);

	const float vertices[] = {
		-0.5f, -0.5f, 
	    0.5f, -0.5f,
	    0.5f, 0.5f,
	    0.0f, 0.0f,
	    -0.5f, 0.5f
	};

	tessAddContour(tess, 2, vertices, sizeof(float) * 2, 5);
	tessTesselate(tess, TESS_WINDING_NONZERO, TESS_POLYGONS, 3, 2, NULL);

	int numVertices = tessGetVertexCount(tess);
	int numIndecies = tessGetElementCount(tess);

	const float* tessVertices = tessGetVertices(tess);
	const int* tessIndices = tessGetElements(tess);

	Mesh *ret = new Mesh;
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
	ret->CreateMesh(verts, indexVector, gameTestTextures.tile);
	return (ret);
}
