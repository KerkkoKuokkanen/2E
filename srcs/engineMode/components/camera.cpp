
#include "camera.h"
#include "imageTransforms.h"
#include "mouse.h"
#include "envHandler.h"

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

	if (WheelIn())
		zoom *= 0.98f;
	else if (WheelOut())
		zoom *= 1.02f;

	if (MouseKeyHeld(n_MouseKeys::MOUSE_LEFT) && moving)
	{
		t_Point place = GetMouseXYZoom();
		x = offSet.x - (place.x - pressPlace.x);
		y = offSet.y - (place.y - pressPlace.y);
	}
	else
		moving = false;
	if (MouseKeyPressed(n_MouseKeys::MOUSE_LEFT) && !OverImgui())
	{
		pressPlace = GetMouseXYZoom();
		offSet = {x, y};
		moving = true;
	}
}
