
#include "sprite.h"
int alphaLocation = 0;

GLSprite::GLSprite(glm::vec2 pos, glm::vec2 dim, GLuint sprite, Shader &shader)
{
	GLSprite::shader = &shader;
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
	glDisable(GL_DEPTH_TEST);
	shader->Activate();
	mesh.VAO.Bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.texture);

	//set uniform
	glUniform1f(alphaLocation, alpha);

	glDrawElements(GL_TRIANGLES, mesh.indecies.size(), GL_UNSIGNED_INT, 0);
	glEnable(GL_DEPTH_TEST);
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

void GLSprite::DrawRect(glm::vec4 dest, glm::vec4 rect)
{
	if (shader == NULL)
		return ;
	mesh.VAO.Bind();
	mesh.VBO.Bind();
	Vertex* vertData = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	vertData[0].position = glm::vec3(dest.x, dest.y + dest.w, 0.0f);
	vertData[0].texUV = glm::vec2(rect.x, rect.y + rect.w);
	vertData[1].position = glm::vec3(dest.x, dest.y, 0.0f);
	vertData[1].texUV = glm::vec2(rect.x, rect.y);
	vertData[2].position = glm::vec3(dest.x + dest.z, dest.y, 0.0f);
	vertData[2].texUV = glm::vec2(rect.x + rect.z, rect.y);
	vertData[3].position = glm::vec3(dest.x + dest.z, dest.y + dest.w, 0.0f);
	vertData[3].texUV = glm::vec2(rect.x + rect.z, rect.y + rect.w);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	shader->Activate();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.texture);

	//set uniform
	glUniform1f(alphaLocation, alpha);

	glDrawElements(GL_TRIANGLES, mesh.indecies.size(), GL_UNSIGNED_INT, 0);
	Vertex* vertData2 = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	vertData2[0].position = mesh.vertecies[0].position;
	vertData2[0].texUV = mesh.vertecies[0].texUV;
	vertData2[1].position = mesh.vertecies[1].position;
	vertData2[1].texUV = mesh.vertecies[1].texUV;
	vertData2[2].position = mesh.vertecies[2].position;
	vertData2[2].texUV = mesh.vertecies[2].texUV;
	vertData2[3].position = mesh.vertecies[3].position;
	vertData2[3].texUV = mesh.vertecies[3].texUV;
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void InitGLSprite(Shader &shader)
{
	alphaLocation = glGetUniformLocation(shader.ID, "uniformAlpha");
}
