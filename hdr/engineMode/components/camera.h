
#ifndef CAMERA_COMPONENT_H
# define CAMERA_COMPONENT_H

# include "customComponent.h"
# include "componentRegistry.h"

class Camera : public CustomComponent
{
	private:
		float x = 0.0f;
		float y = 0.0f;
		float zoom = 10.0f;
	public:
		void Init(void *data, size_t size) override;
		void SetCameraPosition(float x, float y);
		void SetCameraZoom(float zoom);
		void EngineUpdate() override;
};

REGISTER_COMPONENT(Camera);

#endif
