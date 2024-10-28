
#include "convexOverlap.h"
#include <math.h>

static t_Point VecSub(t_Point &one, t_Point &two)
{
	return ((t_Point){one.x - two.x, one.y - two.y});
}

static float DotProduct(t_Point &one, t_Point &two)
{
	return (one.x * two.x + one.y * two.y);
}

static void projectOntoAxis(t_BoundingB &rect, t_Point &axis, float& min, float& max)
{
	float dotProd = DotProduct(rect.leftTop, axis);
	min = dotProd;
	max = dotProd;
	dotProd = DotProduct(rect.rightTop, axis);
	if (dotProd < min) min = dotProd;
	if (dotProd > max) max = dotProd;
	dotProd = DotProduct(rect.leftBottom, axis);
	if (dotProd < min) min = dotProd;
	if (dotProd > max) max = dotProd;
	dotProd = DotProduct(rect.rightBottom, axis);
	if (dotProd < min) min = dotProd;
	if (dotProd > max) max = dotProd;
}

static bool overlapOnAxis(t_BoundingB &rect1, t_BoundingB &rect2, t_Point &axis)
{
	float min1, max1, min2, max2;
	projectOntoAxis(rect1, axis, min1, max1);
	projectOntoAxis(rect2, axis, min2, max2);
	return !(max1 < min2 || max2 < min1);
}

bool ReactangleScreenOverlap(t_BoundingB &rect)
{
	static t_BoundingB screen = {{-1.0f, 1.0f}, {1.0f, 1.0f}, {-1.0f, -1.0f}, {1.0f, -1.0f}};
	const float axes[] = {
		screen.rightTop.x - screen.leftTop.x, screen.rightTop.y - screen.leftTop.y,
		screen.rightBottom.x - screen.leftBottom.x, screen.rightBottom.y - screen.leftBottom.y,
		rect.rightTop.x - rect.leftTop.x, rect.rightTop.y - rect.rightTop.y,
		rect.rightBottom.x - rect.leftBottom.x, rect.rightBottom.y - rect.leftBottom.y,
	};
	for (int i = 0; i < 8; i += 2)
	{
		float axisX = axes[i];
		float axisY = axes[i + 1];
		float length = sqrt(axisX * axisX + axisY * axisY);
		t_Point normalizedAxis = {axisX / length, axisY / length};
		if (!overlapOnAxis(screen, rect, normalizedAxis))
			return (false);
	}
	return (true);
}
