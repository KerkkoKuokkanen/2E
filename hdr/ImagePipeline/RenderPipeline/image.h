
#ifndef IMAGE_H
# define IMAGE_H

# include "sprite.h"
# include "renderSystem.h"

struct textData {
	float x;
	float y;
	float w;
	float h;
	float a;
};

class Image : public RenderObj
{
	private:
		GLSprite *sprite = NULL;
		float GetLowY();
		float GetLowX();
		uint64_t textIndex = 0;
		GLuint texture = 0;
		t_Point position = {0.0f, 0.0f};
		t_Point dimentions = {0.0f, 0.0f};
		float ownWidth = 1.0f;
		float ownHeight = 1.0f;
		float angle = 0.0f;
		t_Box color = {1.0f, 1.0f, 1.0f, 1.0f};
		int transformType = n_TransformTypes::TRANSFORM_CAMERA;
	public:
		bool active = true;
		Image(uint64_t texture, t_Box rect, float angle, int layer);
		Image(std::string texture, t_Box rect, float angle, int layer);
		~Image();
		void SetDrawY() override;
		bool OffscreenDetection() override;
		void BeforeDraw() override;
		size_t GetSaveDataSize();
		void *CollectSaveData(void *buffer, size_t buffSize, size_t &size);
		GLSprite *GetAccessToGLSprite() {return (sprite);};
		void SetTransformType(int tType) {transformType = tType;};
		void SetPosition(float x, float y);
		void SetAngle(float angle) {sprite->SetRotation(angle); Image::angle = angle;};
		void SetWidth(float width);
		void SetHeight(float height);
		void SetLayer(int layer) {ChangeLayer(layer);};
		void SetTexture(std::string name);
		void SetTexture(uint64_t hahs);
		void SetColor(float r, float g, float b, float a);
		t_Point GetPosition() {return (position);};
		float GetAngle() {return (angle);};
		float GetWidht() {return (ownWidth);};
		float GetHeight() {return (ownHeight);};
		t_Box GetColor() {return (color);};
		uint64_t GetTexure() {return (textIndex);};
		int GetLayer() {return (layer);};
		int GetTransformType() {return (transformType);};
		t_BoundingB GetBoundingBox() {return (sprite->GetBoundingB());};

		void SetTextureData(float x, float y, float w, float h, float a) {sprite->SetTextureData(x, y, w, h, a);};
		textData GetTextureData();

		void Draw() override;

};

void InitImage(Shader *usedShader);

#endif
