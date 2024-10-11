
#include "init.h"
#include "sprite.h"
#include "Textures.h"
#include "commonTools.h"
#include "poller.h"
#include "tessTest.h"

SDL_Window *window = NULL;
Shader *shaderProgram = NULL;

void DrawTestMesh(Mesh *testMesh)
{
	shaderProgram->Activate();
	testMesh->VAO.Bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, testMesh->texture);

	int alphaLocation = glGetUniformLocation(shaderProgram->ID, "uniformAlpha");
	glUniform1f(alphaLocation, 1.0f);

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
