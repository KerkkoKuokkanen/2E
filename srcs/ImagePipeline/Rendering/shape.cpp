
#include "shape.h"
#include "tesselator.h"
#include "commonTools.h"
#include "convexOverlap.h"
#include "lineDrawing.h"

#define BIG_VALUE 9999999.9f
#define SMALL_VALUE -9999999.9f

Shader *defaultShader;
static int colorLocation = 0;
static int positionLocation = 0;
static int scaleLocation = 0;
static int rotationLocation = 0;
static int pivotPointLocation = 0;

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
	int tester = tessTesselate(tess, TESS_WINDING_NONZERO, TESS_POLYGONS, 3, 2, NULL);
	if (tester == 0)
		printf("tessalation fail\n");

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
		Vertex vert = {glm::vec3(x, y, 0.0f), glm::vec2(x, -y)};
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

GLShape *CreateGLShape(std::vector<float> &points, GLuint texture, int useType)
{
	t_DataForShape used = CreateGLShapeData(points);
	GLShape *shape = new GLShape(used.vertexData, used.indexData, texture, defaultShader, used.bBox, useType);
	return (shape);
}

static t_Point RotatePoint(t_Point xy, float centerX, float centerY, float cosTheta, float sinTheta)
{
	float xPos = xy.x - centerX;
	float yPos = xy.y - centerY;
	float xNew = xPos * sinTheta - yPos * cosTheta;
	float yNew = xPos * cosTheta + yPos * sinTheta;
	t_Point ret = {xNew + centerX, yNew + centerY};
	return (ret);
}

static t_Point GetCenter(t_BoundingB &boundBox)
{
	float xCenter = (boundBox.leftBottom.x + boundBox.leftTop.x + boundBox.rightBottom.x + boundBox.rightTop.x) / 4.0f;
	float yCenter = (boundBox.leftBottom.y + boundBox.leftTop.y + boundBox.rightBottom.y + boundBox.rightTop.y) / 4.0f;
	return ((t_Point){xCenter, yCenter});
}

GLShape::GLShape(std::vector<Vertex> &verts, std::vector<GLuint> &inds, GLuint texture, Shader *shader, t_BoundingB boundingBox, int useType)
{
	GLShape::shader = (shader == NULL) ? defaultShader : shader;
	GLenum usage = GL_STATIC_DRAW;
	if (useType == n_DrawUseTypes::DATA_CHANGES_LOT)
		usage = GL_DYNAMIC_DRAW;
	else if (useType == n_DrawUseTypes::ONE_USE_ONLY)
		usage = GL_STREAM_DRAW;
	mesh.CreateMesh(verts, inds, texture, usage);
	boundBox = boundingBox;
	rotatedBoundBox = boundBox;
	width = 1.0f;
	height = 1.0f;
	pivotPoint = GetCenter(boundBox);
}

void GLShape::Draw()
{
	if (shader == NULL)
		return ;
	if (!ReactangleScreenOverlap(rotatedBoundBox))
		return ;
	shader->Activate();
	mesh.VAO.Bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.texture);

	glUniform4f(colorLocation, imageColor.x, imageColor.y, imageColor.z, imageColor.w);
	glUniform2f(positionLocation, position.x, position.y);
	glUniform2f(scaleLocation, width, height);
	glUniform1f(rotationLocation, angle);
	glUniform2f(pivotPointLocation, pivotPoint.x, pivotPoint.y);

	glDrawElements(GL_TRIANGLES, mesh.indecies.size(), GL_UNSIGNED_INT, 0);
}

void GLShape::SetPosition(float x, float y)
{
	t_Point center = GetCenter(boundBox);
	float xAdd = x - center.x;
	float yAdd = y - center.y;
	boundBox.leftBottom.x += xAdd;
	boundBox.leftBottom.y += yAdd;
	boundBox.leftTop.x += xAdd;
	boundBox.leftTop.y += yAdd;
	boundBox.rightBottom.x += xAdd;
	boundBox.rightBottom.y += yAdd;
	boundBox.rightTop.x += xAdd;
	boundBox.rightTop.y += yAdd;
	rotatedBoundBox.leftBottom.x += xAdd;
	rotatedBoundBox.leftBottom.y += yAdd;
	rotatedBoundBox.leftTop.x += xAdd;
	rotatedBoundBox.leftTop.y += yAdd;
	rotatedBoundBox.rightBottom.x += xAdd;
	rotatedBoundBox.rightBottom.y += yAdd;
	rotatedBoundBox.rightTop.x += xAdd;
	rotatedBoundBox.rightTop.y += yAdd;
	position = {x, y};
}

void GLShape::SetRotatedBoundBox()
{
	rotatedBoundBox = boundBox;
	t_Point center = GetCenter(boundBox);
	float cosTheta = std::cos(angle);
	float sinTheta = std::sin(angle);
	rotatedBoundBox.leftBottom = RotatePoint(boundBox.leftBottom, center.x, center.y, cosTheta, sinTheta);
	rotatedBoundBox.leftTop = RotatePoint(boundBox.leftTop, center.x, center.y, cosTheta, sinTheta);
	rotatedBoundBox.rightBottom = RotatePoint(boundBox.rightBottom, center.x, center.y, cosTheta, sinTheta);
	rotatedBoundBox.rightTop = RotatePoint(boundBox.rightTop, center.x, center.y, cosTheta, sinTheta);
}

void GLShape::SetRotation(float angle)
{
	GLShape::angle = angle;
	SetRotatedBoundBox();
}

void GLShape::SetHeight(float h)
{
	float w = h;
	float wUsage = w / 2.0f;
	t_Point center = GetCenter(boundBox);
	boundBox.leftTop.x = center.x + wUsage;
	boundBox.rightTop.x = center.x - wUsage;
	boundBox.leftBottom.x = center.x + wUsage;
	boundBox.rightBottom.x = center.x - wUsage;
	SetRotatedBoundBox();
	height = h;
}

void GLShape::SetWidth(float w)
{
	float h = w;
	float hUsage = h / 2.0f;
	t_Point center = GetCenter(boundBox);
	boundBox.leftTop.y = center.y - hUsage;
	boundBox.rightTop.y = center.y - hUsage;
	boundBox.leftBottom.y = center.y + hUsage;
	boundBox.rightBottom.y = center.y + hUsage;
	SetRotatedBoundBox();
	width = w;
}

void GLShape::Delete()
{
	mesh.Delete();
}

void InitShapes(Shader *shaderProgram)
{
	defaultShader = shaderProgram;
	colorLocation = glGetUniformLocation(shaderProgram->ID, "imageColor");
	positionLocation = glGetUniformLocation(shaderProgram->ID, "uPosition");
	scaleLocation = glGetUniformLocation(shaderProgram->ID, "uScale");
	rotationLocation = glGetUniformLocation(shaderProgram->ID, "uRotation");
	pivotPointLocation = glGetUniformLocation(shaderProgram->ID, "uPivot");
}
