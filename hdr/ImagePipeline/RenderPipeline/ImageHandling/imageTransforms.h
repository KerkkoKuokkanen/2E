
#ifndef IMAGE_TRANSFORMS_H
# define IMAGE_TRANSFORMS_H

#include "box.h"

extern float __ScreenSpaceUsedWidth;
extern float __ScreenSpaceUsedHeight;

extern float __CameraX;
extern float __CameraY;

t_Point TransformCoordinateToScreenSpace(float x, float y);
t_Point TransformCoordinateToScreenSpaceCamera(float x, float y);
void ResetImageTransformations();

#endif
