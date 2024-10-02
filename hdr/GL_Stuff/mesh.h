
#ifndef MESH_CLASS_H
# define MESH_CLASS_H

# include <string>
# include <vector>
# include <glm/glm.hpp>

# include "VAO.h"
# include "EBO.h"

class Mesh
{
public:
	std::vector<Vertex> vertecies;
	std::vector<GLuint> indecies;
	GLuint texture;
	glm::vec3 currPos = glm::vec3(0.0f, 0.0f, 0.0f);
	VAO VAO;
	VBO VBO;
	EBO EBO;

	void CreateMesh(std::vector<Vertex>& vertecies, std::vector<GLuint>& indecies, GLuint texture);
	void Delete();
};

#endif
