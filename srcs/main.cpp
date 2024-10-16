
#include "init.h"
#include "sprite.h"
#include "Textures.h"
#include "commonTools.h"
#include "poller.h"
#include "tessTest.h"
#include <glm/gtc/type_ptr.hpp>

SDL_Window *window = NULL;
Shader *shaderProgram = NULL;

void DrawTestMesh(Mesh *testMesh)
{
	shaderProgram->Activate();
	testMesh->VAO.Bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, testMesh->texture);

	glm::mat4 transform = glm::mat4(1.0f);
	float scaleFactor = 1.5f;
	transform = glm::scale(transform, glm::vec3(scaleFactor, scaleFactor, scaleFactor));

	int alphaLocation = glGetUniformLocation(shaderProgram->ID, "uniformAlpha");
	glUniform1f(alphaLocation, 1.0f);
	int transformLocation = glGetUniformLocation(shaderProgram->ID, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

	glDrawElements(GL_TRIANGLES, testMesh->indecies.size(), GL_UNSIGNED_INT, 0);
}

void MainLoop()
{
	GLSprite test(glm::vec2(-0.5f, 0.5f), glm::vec2(1.0f, 1.0f), gameTestTextures.tile, shaderProgram);
	GLSprite test2(glm::vec2(-0.5f, 0.5f), glm::vec2(1.0f, 1.0f), gameTestTextures.hamis, shaderProgram);
	Mesh *ret = TessalationTest();

	clock_t start, end;

	while(true)
	{
		start = clock();
		Poller();
		ClearWindow();

		shaderProgram->Activate();
		//test.Draw();
		//test2.Draw();
		DrawTestMesh(ret);

		SDL_GL_SwapWindow(window);
		end = clock();
		SDL_Delay(figure_the_delay(start, end));
	}
}

int main()
{
	window = Init();
	Shader shader("shaders/sprite_vert.glsl", "shaders/sprite_frag.glsl");
	InitGLSprite(shader);
	LoadTextures();
	shaderProgram = &shader;
	MainLoop();
	return (0);
}
