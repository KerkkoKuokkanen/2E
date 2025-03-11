
#include "camera.h"
#include "imageTransforms.h"

void Camera::Init(void *data, size_t size)
{
	AddToSaveTracking(&x, sizeof(float));
	AddToSaveTracking(&y, sizeof(float));
	AddToSaveTracking(&zoom, sizeof(float));
	CreateInputField("x", n_VarType::FLOAT, &x);
	CreateInputField("y", n_VarType::FLOAT, &y);
	CreateInputField("zoom", n_VarType::FLOAT, &zoom);
	if (size <= 1)
		return ;
	char *cast = (char*)data;
	memcpy(&x, cast, sizeof(float));
	memcpy(&y, cast + sizeof(float), sizeof(float));
	memcpy(&zoom, cast + sizeof(float) + sizeof(float), sizeof(float));
}

void Camera::SetCameraPosition(float x, float y)
{
	Camera::x = x;
	Camera::y = y;
	SetCameraCoordinates(x, y);
}

void Camera::SetCameraZoom(float zoom)
{
	Camera::zoom = zoom;
	SetScreenSpaceDimentions(zoom, zoom);
}

void Camera::EngineUpdate()
{
	SetCameraCoordinates(x, y);
	SetScreenSpaceDimentions(zoom, zoom);
}
