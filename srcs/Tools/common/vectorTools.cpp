
#include "box.h"
#include <math.h>

float VectorMagnitude(t_Point vec)
{
	float mag = sqrt(vec.x * vec.x + vec.y * vec.y);
	return (mag);
}

t_Point VectorRotate(t_Point vec, float angle)
{
	float rotatedX = vec.x * cos(angle) - vec.y * sin(angle);
	float rotatedY = vec.x * sin(angle) + vec.y * cos(angle);
	return (t_Point{rotatedX, rotatedY});
}
