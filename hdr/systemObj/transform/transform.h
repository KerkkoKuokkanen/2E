
#ifndef TRANSFORM_H
# define TRANSFORM_H

# include "image.h"
# include "structure.h"

# define LINE_LAYER 6

class Transform
{
	private:
		std::vector<Image*> images;
		std::vector<Structure*> structures;
		t_Point ownPosition = {0.0f, 0.0f};
		float ownWidth = 1.0f;
		float ownHeight = 1.0f;
		float ownAngle = 0.0f;
		t_Point GetAngleForObj(t_Point pos, float angle);
		t_Point VecWidthDirection();
		t_Point VecHeighDirection();
	public:
		size_t SaveDataSize();
		void *CollectSaveData(void *buffer, size_t buffSize, size_t &size);
		void AddImage(Image *img) {images.push_back(img);};
		void AddStructure(Structure *str) {structures.push_back(str);};
		void RemoveImage(Image *img);
		void RemoveStructure(Structure *str);

		void SetOwnPosition(float x, float y) {ownPosition = {x, y};};
		void SetOwnAngle(float angle) {ownAngle = angle;};
		void SetOwnScale(float width, float height) {ownWidth = width; ownHeight = height;};

		void Position(float x, float y);
		void Angle(float angle);
		void Width(float width);
		void Height(float height);
};

#endif
