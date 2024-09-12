
#include <stdio.h>
#include "../SDL2/SDL.h"
#include <OpenGL/gl3.h>
#include "../hdr/init.h"

float vertices[] = {
    // Positions         // Texture Coords
    -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,   // Top-left
     0.5f,  0.5f, 0.0f,  1.0f, 1.0f,   // Top-right
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f,   // Bottom-right
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f    // Bottom-left
};

unsigned int indices[] = {
    0, 1, 2,   // First triangle
    2, 3, 0    // Second triangle
};

void Poller()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			exit(0);
	}
}

void ClearWindow()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

GLuint LoadTexture(const char* file) {
	SDL_Surface* surface = IMG_Load(file);
	if (!surface)
	{
		printf("IMG_Load Error: %s\n", SDL_GetError());
		return (0);
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int mode = surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);

	return (textureID);
}

GLuint compileShader(const char* shaderSource, GLenum shaderType) {
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	// Check for compilation errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("CompileShaderError\n");
	}

	return shader;
}

char* readFile(const char* filePath)
{
	FILE* file = fopen(filePath, "r");
	if (!file)
	{
		printf("Failed to open shader file: %s\n", filePath);
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long length = ftell(file);
	rewind(file);

	char* content = (char*)malloc((length + 1) * sizeof(char));
	if (!content)
	{
		printf("Failed to allocate memory for shader content\n");
		fclose(file);
		return NULL;
	}

	fread(content, 1, length, file);
	content[length] = '\0';  // Null-terminate the string

	fclose(file);
	return content;
}

GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource)
{
	char *content1 = readFile(vertexSource);
	char *content2 = readFile(fragmentSource);
	GLuint vertexShader = compileShader(content1, GL_VERTEX_SHADER);
	GLuint fragmentShader = compileShader(content2, GL_FRAGMENT_SHADER);
	free(content1);
	free(content2);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check for linking errors
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

int main()
{
	SDL_Window *window = Init();
	GLuint shaderProgram = createShaderProgram("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
	GLuint text = LoadTexture("Roof2_Color.png");

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the VAO
	glBindVertexArray(VAO);

	// Bind and fill the VBO with vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Bind and fill the EBO with index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Set vertex attribute pointers
	// Position attribute (location = 0 in the shader)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture coordinate attribute (location = 1 in the shader)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind the VAO (optional, but good practice)
	glBindVertexArray(0);

	while(true)
	{
		Poller();
		ClearWindow();

		glUseProgram(shaderProgram);
		glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, text);

		glBindVertexArray(VAO);

		// Draw the quad (2 triangles)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Unbind the VAO (optional)
		glBindVertexArray(0);

		SDL_GL_SwapWindow(window);
	}
	return (0);
}
