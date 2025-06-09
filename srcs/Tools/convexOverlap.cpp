
#include "convexOverlap.h"
#include <cmath>
#include <algorithm>

static t_BoundingB screen = {{-1.0f, 1.0f}, {1.0f, 1.0f}, {-1.0f, -1.0f}, {1.0f, -1.0f}};

static float projectPoint(const t_Point& point, const t_Point& axis) {
	return (point.x * axis.x + point.y * axis.y);
}

// Helper function to check for overlap on a specific axis
static bool overlapOnAxis(const t_BoundingB& box1, const t_BoundingB& box2, const t_Point& axis) {
	// Project all four corners of both boxes onto the axis
	float box1Min = projectPoint(box1.leftTop, axis);
	float box1Max = box1Min;

	float projections[3] = {
		projectPoint(box1.rightTop, axis),
		projectPoint(box1.leftBottom, axis),
		projectPoint(box1.rightBottom, axis)
	};
	for (float proj : projections) {
		box1Min = std::min(box1Min, proj);
		box1Max = std::max(box1Max, proj);
	}

	float box2Min = projectPoint(box2.leftTop, axis);
	float box2Max = box2Min;

	float projections2[3] = {
		projectPoint(box2.rightTop, axis),
		projectPoint(box2.leftBottom, axis),
		projectPoint(box2.rightBottom, axis)
	};
	for (float proj : projections2) {
		box2Min = std::min(box2Min, proj);
		box2Max = std::max(box2Max, proj);
	}

	// Check for overlap
	return (box1Max >= box2Min && box2Max >= box1Min);
}

// Main function to check if two bounding boxes overlap
bool ReactangleScreenOverlap(t_BoundingB& rect) {
	// Define the axes to check: perpendicular to each edge of both bounding boxes
	t_Point axes[4] = {
		{ screen.rightTop.x - screen.leftTop.x, screen.rightTop.y - screen.leftTop.y },
		{ screen.rightTop.x - screen.rightBottom.x, screen.rightTop.y - screen.rightBottom.y },
		{ rect.rightTop.x - rect.leftTop.x, rect.rightTop.y - rect.leftTop.y },
		{ rect.rightTop.x - rect.rightBottom.x, rect.rightTop.y - rect.rightBottom.y }
	};

	// Normalize each axis and check for overlap
	for (const auto& axis : axes) {
		float length = std::sqrt(axis.x * axis.x + axis.y * axis.y);
		t_Point normalizedAxis = { axis.x / length, axis.y / length };

		// If there is no overlap on this axis, the rectangles do not overlap
		if (!overlapOnAxis(screen, rect, normalizedAxis))
			return false;
	}

	// If all axes have overlap, the rectangles overlap
	return true;
}
