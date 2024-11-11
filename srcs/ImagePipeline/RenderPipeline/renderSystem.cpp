
#include "renderSystem.h"
#include "pillarBoxes.h"
#include "screen.h"
#include "ShaderClass.h"
#include "mainTools.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

RenderSystem universalRenderingSystem;
Shader *defaultFboShader = NULL;

static unsigned int RenderGenerateUniqueKey()
{
	static unsigned int ret = 0;
	ret += 1;
	return (ret);
}

static bool RFAlmostEqual(float a, float b, float precision = 0.00001f)
{
	return (std::abs(a - b) < precision);
}

static bool CompareLayers(const t_RenderLayer &a, const t_RenderLayer &b)
{
	return (a.layerNumber < b.layerNumber);
}

static bool SortLayerYSort(RenderObj *obj1, RenderObj *obj2)
{
	return (obj1->drawY > obj2->drawY);
}

static bool SortLayerDepthSort(RenderObj *obj1, RenderObj *obj2)
{
	return (obj1->drawDepth < obj2->drawDepth);
}

static bool SortLayerDepthAndYSort(RenderObj *obj1, RenderObj *obj2)
{
	if (!(std::abs(obj1->drawDepth - obj2->drawDepth) < 0.00001f))
		return (obj1->drawDepth < obj2->drawDepth);
	return (obj1->drawY > obj2->drawY);
}

static void NewImgUiFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void RenderSystem::AddLayer(int layerNumber, int sortType)
{
	int sortNum = sortType;
	t_RenderLayer add;
	if (sortNum == n_SortTypes::TEXT_LAYER)
	{
		add.layerNumber = layerNumber;
		add.sortType = sortNum;
		add.fbo = NULL;
		add.fboRenderObj = NULL;
		renderLayers.push_back(add);
		return ;
	}
	if (sortNum < 0 || sortNum > n_SortTypes::DEPTH_Y_SORT)
		sortNum = n_SortTypes::NO_SORT;
	add.layerNumber = layerNumber;
	add.sortType = sortNum;
	add.fbo = new FBO(__currentScreenWidth, __currentScreenHeight);
	add.fboRenderObj = new FBORender(defaultFboShader);
	renderLayers.push_back(add);
	std::sort(renderLayers.begin(), renderLayers.end(), CompareLayers);
}

bool RenderSystem::AddRenderObject(RenderObj *obj, int layer, unsigned int key)
{
	for (int i = 0; i < renderLayers.size(); i++)
	{
		if (renderLayers[i].layerNumber == layer)
		{
			renderLayers[i].images.push_back(obj);
			keyToIndexMap[key] = renderLayers[i].images.size() - 1;
			return (true);
		}
	}
	return (false);
}

bool RenderSystem::RemoveRenderObject(RenderObj *obj, int layer, unsigned int key)
{
	if (deleting)
		return (true);
	auto &images = renderLayers[layer].images;
	auto mapIt = keyToIndexMap.find(key);
	if (mapIt == keyToIndexMap.end()) {
		return false;
	}
	int index = mapIt->second;
	if (index != images.size() - 1)
	{
		images[index] = images.back();
		keyToIndexMap[key] = index;
	}
	images.pop_back();
	keyToIndexMap.erase(mapIt);
	return true;
}

void RenderSystem::RenderAll()
{
	for (int i = 0; i < renderLayers.size(); i++)
	{
		renderLayers[i].fbo->Bind();
		glClear(GL_COLOR_BUFFER_BIT);
		if (renderLayers[i].sortType == n_SortTypes::Y_SORT)
			std::sort(renderLayers[i].images.begin(), renderLayers[i].images.end(), SortLayerYSort);
		else if (renderLayers[i].sortType == n_SortTypes::DEPTH_SORT)
			std::sort(renderLayers[i].images.begin(), renderLayers[i].images.end(), SortLayerDepthSort);
		else if (renderLayers[i].sortType == n_SortTypes::DEPTH_Y_SORT)
			std::sort(renderLayers[i].images.begin(), renderLayers[i].images.end(), SortLayerDepthAndYSort);
		else if (renderLayers[i].sortType == n_SortTypes::TEXT_LAYER)
			NewImgUiFrame();
		for (int j = 0; j < renderLayers[i].images.size(); j++)
		{
			RenderObj *obj = renderLayers[i].images[j];
			if (obj->drawActive)
				obj->Draw();
		}
	}
	BindScreenForUse();
	for (int i = 0; i < renderLayers.size(); i++)
	{
		if (renderLayers[i].sortType == n_SortTypes::TEXT_LAYER)
		{
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			continue ;
		}
		renderLayers[i].fboRenderObj->SetTexture(renderLayers[i].fbo->GetTexture());
		renderLayers[i].fboRenderObj->Draw();
	}
}

void RenderSystem::ClearRenderSystem()
{
	deleting = true;
	for (int i = 0; i < renderLayers.size(); i++)
	{
		for (int j = 0; j < renderLayers[i].images.size(); j++)
			delete (renderLayers[i].images[j]);
		renderLayers[i].images.clear();
		delete renderLayers[i].fbo;
		delete renderLayers[i].fboRenderObj;
	}
	renderLayers.clear();
	deleting = false;
}

void RenderObj::AddToRenderSystem(int layer)
{
	uniqueKey = RenderGenerateUniqueKey();
	RenderObj::layer = layer;
	universalRenderingSystem.AddRenderObject(this, layer, uniqueKey);
}

RenderObj::~RenderObj()
{
	universalRenderingSystem.RemoveRenderObject(this, layer, uniqueKey);
}

void InitRenderSystem()
{
	defaultFboShader = new Shader("shaders/default_fbo_vert.glsl", "shaders/default_fbo_frag.glsl");
}
