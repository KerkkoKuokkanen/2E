
#include "LoadTexture.h"
#include "../frameworks/SDL2/SDL.h"

GLuint LoadTexture(const char* file)
{
	SDL_Surface* surface = IMG_Load(file);
	if (!surface)
	{
		printf("IMG_Load Error: %s\n", SDL_GetError());
		return (0);
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Check the format of the surface
	GLenum format;
	if (surface->format->BytesPerPixel == 4) { // 32-bit
		if (surface->format->Rmask == 0x000000ff)
			format = GL_RGBA; // This is the correct format if the R mask is in the least significant byte
		else
			format = GL_BGRA; // If R mask is in the most significant byte, use BGRA
	} else if (surface->format->BytesPerPixel == 3) { // 24-bit
		if (surface->format->Rmask == 0x000000ff)
			format = GL_RGB;
		else
			format = GL_BGR;
	} else {
		// Unsupported format
		printf("Unsupported image format\n");
		SDL_FreeSurface(surface);
		return (0);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);

	return (textureID);
}
