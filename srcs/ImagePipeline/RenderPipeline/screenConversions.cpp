
#include "screen.h"
#include <math.h>

int GLXCoordinateToScreenPixelCoordinate(float GLX)
{
	float scale = (float)__currentScreenWidth / 2.0f;
	GLX += 1.0f;
	int position = round(scale * GLX);
	return (position);
}

int GLYCoordinateToScreenPixelCoordinate(float GLY)
{
	float scale = (float)__currentScreenHeight / 2.0f;
	GLY = (GLY * (-1)) + 1.0f;
	int position = round(scale * GLY);
	return (position);
}
