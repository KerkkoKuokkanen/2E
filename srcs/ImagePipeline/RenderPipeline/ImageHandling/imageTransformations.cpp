
#include "imageTransforms.h"
#include "pillarBoxes.h"
#include "screen.h"

float __ScreenSpaceUsedWidth;
float __ScreenSpaceUsedHeight;

float __CameraX;
float __CameraY;

t_Point TransformCoordinateToScreenSpace(float x, float y)
{
	float widthUnit = 1.0f - GetWidthMinus();
	float heightUnit = 1.0f - GetHeightMinus();
	float widthScale = widthUnit / __ScreenSpaceUsedWidth;
	float heightScale = heightUnit / __ScreenSpaceUsedHeight;
	float wPosition = widthScale * x;
	float hPosition = heightScale * y;
	return (t_Point{wPosition, hPosition});
}

t_Point TransformCoordinateToScreenSpaceCamera(float x, float y)
{
	x = x - __CameraX;
	y = y - __CameraY;
	float widthUnit = 1.0f - GetWidthMinus();
	float heightUnit = 1.0f - GetHeightMinus();
	float widthScale = widthUnit / __ScreenSpaceUsedWidth;
	float heightScale = heightUnit / __ScreenSpaceUsedHeight;
	float wPosition = widthScale * x;
	float hPosition = heightScale * y;
	return (t_Point{wPosition, hPosition});
}

void ResetImageTransformations()
{
	__ScreenSpaceUsedWidth = 10.0f;
	__ScreenSpaceUsedHeight = 10.0f;
	__CameraX = 5.0f;
	__CameraY = 5.0f;
}
