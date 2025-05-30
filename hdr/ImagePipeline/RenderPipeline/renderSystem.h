
#ifndef RENDER_SYSTEM_H
# define RENDER_SYSTEM_H

# include <vector>
# include <tuple>
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
		TEXT_LAYER,
		MULTI_LAYER
	};
};

class RenderObj
{
	private:
		uint32_t uniqueKey = 0;
		friend class SystemObj;
	protected:
		void *self = NULL;
		int layer = 0;
		void AddToRenderSystem(int layer);
		void ChangeLayer(int layer);
	public:
		uint32_t id = 0;
		virtual void SetDrawY() {};
		virtual void BeforeDraw() {};
		virtual bool OffscreenDetection() {return (false);};
		virtual void Draw() {};
		virtual ~RenderObj();
		float drawY = 0.0f;
		float drawX = 0.0f;
		float drawDepth = 0.0f;
		bool drawActive = true;
};

typedef struct s_RenderLayer
{
	int						layerNumber;
	int						sortType;
	FBO						*fbo;
	FBORender				*fboRenderObj;
	std::unordered_map<uint32_t, RenderObj*> imagess;
}				t_RenderLayer;

class RenderSystem
{
	private:
		bool deleting = false;
		std::vector<t_RenderLayer> renderLayers = {};
		void AddLayerOwn(int layerNumber, int sortType);
		void SaveLayers();
	public:
		std::vector<std::tuple<int, int>> GetLayerData();
		void Init();
		bool AddRenderObject(RenderObj *obj, int layer, uint32_t key);
		bool RemoveRenderObject(RenderObj *obj, int layer, uint32_t key);
		void AddLayer(int layerNumber, int sortType);
		void RenderAll();
		void ClearRenderSystem();
		bool RemoveLayer(int layerNumber);
};

extern RenderSystem universalRenderingSystem;

void InitRenderSystem();

#endif
