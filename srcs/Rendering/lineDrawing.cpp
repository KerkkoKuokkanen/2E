
#include "ShaderClass.h"
#include <glm/gtc/type_ptr.hpp>
#include "box.h"

static Shader *lineShader = NULL;
static GLint colorLocation = 0;

void InitLines()
{
	lineShader = new Shader("shaders/line_vert.glsl", "shaders/line_frag.glsl");
	colorLocation = glGetUniformLocation(lineShader->ID, "lineColor");
}

void DrawLineWithColor(t_Point one, t_Point two, glm::vec4 color)
{
	float lineVertices[] = {one.x, one.y, two.x, two.y};

	// Create a Vertex Array Object (VAO) and Vertex Buffer Object (VBO) for the line
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	// Bind the VBO and copy the line vertices to the buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

	// Specify the layout of the vertex data (location = 0 in the vertex shader)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Use the shader program for drawing the line
	glUseProgram(lineShader->ID);

	glUniform4fv(colorLocation, 1, glm::value_ptr(color));

	// Bind the VAO and draw the line
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);  // Drawing two vertices as a line

	// Unbind and clean up
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void DrawLine(t_Point one, t_Point two)
{
	DrawLineWithColor(one, two, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}
