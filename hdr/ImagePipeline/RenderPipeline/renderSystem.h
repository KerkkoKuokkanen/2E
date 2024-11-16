
#ifndef RENDER_SYSTEM_H
# define RENDER_SYSTEM_H

# include <vector>
# include <unordered_map>
# include "FBO.h"
# include "FBORender.h"

# define PI 3.1415926

namespace n_TransformTypes
{
	enum {
		NO_TRANSFORM,
		TRANSFORM_STATIC,
		TRANSFORM_CAMERA
	};
};

namespace n_SortTypes
{
	enum {
		NO_SORT,
		Y_SORT,
		DEPTH_SORT,
		DEPTH_Y_SORT,
		TEXT_LAYER
	};
};

class RenderObj
{
	private:
		unsigned int uniqueKey = 0;
	protected:
		int layer = 0;
		void AddToRenderSystem(int layer);
	public:
		virtual void Draw() = 0;
		virtual ~RenderObj();
		float drawY = 0.0f;
		float drawDepth = 0.0f;
		bool drawActive = true;
};

typedef struct s_RenderLayer
{
	int						layerNumber;
	int						sortType;
	FBO						*fbo;
	FBORender				*fboRenderObj;
	std::vector<RenderObj*>	images;
}				t_RenderLayer;

class RenderSystem
{
	private:
		bool deleting = false;
		std::vector<t_RenderLayer> renderLayers = {};
		std::unordered_map<unsigned int, size_t> keyToIndexMap = {};
	public:
		bool AddRenderObject(RenderObj *obj, int layer, unsigned int key);
		bool RemoveRenderObject(RenderObj *obj, int layer, unsigned int key);
		void AddLayer(int layerNumber, int sortType);
		void RenderAll();
		void ClearRenderSystem();
};

extern RenderSystem universalRenderingSystem;

void InitRenderSystem();

#endif
