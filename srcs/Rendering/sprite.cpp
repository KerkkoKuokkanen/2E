
#include "sprite.h"
#include "commonTools.h"
#include <glm/gtc/type_ptr.hpp>

#define BIG_VALUE 9999999.9f
#define SMALL_VALUE -9999999.9f

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

GLSprite::GLSprite(glm::vec2 pos, glm::vec2 dim, GLuint sprite, Shader *shader)
{
	w = dim.x;
	h = dim.y;
	bBox = {pos.x, pos.y, dim.x, dim.y};
	GLSprite::shader = shader;
	Vertex vert1 = {glm::vec3(pos.x, pos.y, 0.0f),
					glm::vec2(0.0f, 1.0f)};

	Vertex vert2 = {glm::vec3(pos.x, pos.y + dim.y, 0.0f),
					glm::vec2(0.0f, 0.0f)};

	Vertex vert3 = {glm::vec3(pos.x + dim.x, pos.y + dim.y, 0.0f),
					glm::vec2(1.0f, 0.0f)};

	Vertex vert4 = {glm::vec3(pos.x + dim.x, pos.y, 0.0f),
					glm::vec2(1.0f, 1.0f)};
	GLuint index[] = {
		0, 1, 2,
		2, 3, 0
	};
	std::vector<GLuint> ind(index, index + sizeof(index) / sizeof(GLuint));
	std::vector<Vertex> verts;
	verts.push_back(vert1);
	verts.push_back(vert2);
	verts.push_back(vert3);
	verts.push_back(vert4);
	mesh.CreateMesh(verts, ind, sprite);
}

void GLSprite::Draw()
{
	if (shader == NULL)
		return ;
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

void GLSprite::RotateGLSprite(float angle, Vertex *vertData)
{
	float cosTheta = std::cos(angle);
	float sinTheta = std::sin(angle);

	float xCenter = bBox.x + (bBox.w / 2.0f);
	float yCenter = bBox.y + (bBox.h / 2.0f);

	for (int i = 0; i < 4; i++)
	{
		float xPos = vertData[i].position.x - xCenter;
		float yPos = vertData[i].position.y - yCenter;
		float xNew = xPos * cosTheta - yPos * sinTheta;
		float yNew = xPos * sinTheta + yPos * cosTheta;
		vertData[i].position.x = xNew + xCenter;
		vertData[i].position.y = yNew + yCenter;
	}
}

void GLSprite::SetAngle(float angle)
{
	if (FAlmostEqual(angle, GLSprite::angle))
		return ;
	float add = angle - GLSprite::angle;
	mesh.VAO.Bind();
	mesh.VBO.Bind();
	Vertex* vertData = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	RotateGLSprite(add, vertData);
	SetBoundingBox(vertData);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	GLSprite::angle = angle;
}

void GLSprite::PositionSprite(float x, float y)
{
	SetDest(glm::vec4(x, y, w, h));
}

void GLSprite::SetDest(glm::vec4 dest)
{
	w = dest.z;
	h = dest.w;
	bBox = {dest.x, dest.y, dest.z, dest.w};
	mesh.VAO.Bind();
	mesh.VBO.Bind();
	Vertex* vertData = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	vertData[0].position = glm::vec3(dest.x, dest.y, 0.0f);
	vertData[1].position = glm::vec3(dest.x, dest.y + dest.w, 0.0f);
	vertData[2].position = glm::vec3(dest.x + dest.z, dest.y + dest.w, 0.0f);
	vertData[3].position = glm::vec3(dest.x + dest.z, dest.y, 0.0f);
	if (!FAlmostEqual(angle, 0.0f))
		RotateGLSprite(angle, vertData);
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void GLSprite::SetRect(glm::vec4 rect)
{
	mesh.VAO.Bind();
	mesh.VBO.Bind();
	Vertex* vertData = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	vertData[0].texUV = glm::vec2(rect.x, rect.y + rect.w);
	vertData[1].texUV = glm::vec2(rect.x, rect.y);
	vertData[2].texUV = glm::vec2(rect.x + rect.z, rect.y);
	vertData[3].texUV = glm::vec2(rect.x + rect.z, rect.y + rect.w);
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void GLSprite::SetBoundingBox(Vertex *vertData)
{
	float xMin = BIG_VALUE, yMin = BIG_VALUE;
	float xMax = SMALL_VALUE, yMax = SMALL_VALUE;
	for (int i = 0; i < 4; i++)
	{
		float x = vertData[i].position.x;
		float y = vertData[i].position.y;
		xMin = (x < xMin) ? xMin = x : xMin;
		xMax = (x > xMax) ? x : xMax;
		yMin = (y < yMin) ? y : yMin;
		yMax = (y > yMax) ? y : yMax;
	}
	bBox = {xMin, yMin, xMax - xMin, yMax - yMin};
}

void GLSprite::Delete()
{
	mesh.Delete();
}

void InitGLSprite(Shader *shader)
{
	alphaLocation = glGetUniformLocation(shader->ID, "uniformAlpha");
	transformLocation = glGetUniformLocation(shader->ID, "transform");
}
