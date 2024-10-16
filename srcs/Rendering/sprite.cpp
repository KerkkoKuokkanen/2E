
#include "sprite.h"
#include <glm/gtc/type_ptr.hpp>

int alphaLocation = 0;
int transformLocation = 0;

GLSprite::GLSprite(glm::vec2 pos, glm::vec2 dim, GLuint sprite, Shader *shader)
{
	GLSprite::shader = shader;
	Vertex vert1 = {glm::vec3(pos.x, pos.y, 0.0f),
					glm::vec2(0.0f, 0.0f)};

	Vertex vert2 = {glm::vec3(pos.x, pos.y - dim.y, 0.0f),
					glm::vec2(0.0f, 1.0f)};

	Vertex vert3 = {glm::vec3(pos.x + dim.x, pos.y - dim.y, 0.0f),
					glm::vec2(1.0f, 1.0f)};

	Vertex vert4 = {glm::vec3(pos.x + dim.x, pos.y, 0.0f),
					glm::vec2(1.0f, 0.0f)};
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
	shader->Activate();
	mesh.VAO.Bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.texture);

	//set uniform
	glUniform1f(alphaLocation, alpha);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

	glDrawElements(GL_TRIANGLES, mesh.indecies.size(), GL_UNSIGNED_INT, 0);
}

void GLSprite::SetDest(glm::vec4 dest)
{
	mesh.VAO.Bind();
	mesh.VBO.Bind();
	Vertex* vertData = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	vertData[0].position = glm::vec3(dest.x, dest.y + dest.w, 0.0f);
	vertData[1].position = glm::vec3(dest.x, dest.y, 0.0f);
	vertData[2].position = glm::vec3(dest.x + dest.z, dest.y, 0.0f);
	vertData[3].position = glm::vec3(dest.x + dest.z, dest.y + dest.w, 0.0f);
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

void InitGLSprite(Shader &shader)
{
	alphaLocation = glGetUniformLocation(shader.ID, "uniformAlpha");
	transformLocation = glGetUniformLocation(shader.ID, "transform");
}
