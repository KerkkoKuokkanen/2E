
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

static uint32_t RenderGenerateUniqueKey()
{
	static uint32_t ret = 0;
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

bool RenderSystem::AddRenderObject(RenderObj *obj, int layer, uint32_t key)
{
	for (int i = 0; i < renderLayers.size(); i++)
	{
		if (renderLayers[i].layerNumber == layer)
		{
			renderLayers[i].imagess[key] = obj;
			return (true);
		}
	}
	return (false);
}

bool RenderSystem::RemoveRenderObject(RenderObj *obj, int layer, uint32_t key)
{
	if (deleting)
		return (true);
	for (int i = 0; i < renderLayers.size(); i++)
	{
		if (i == layer)
		{
			auto it = renderLayers[i].imagess.find(key);
			if (it == renderLayers[i].imagess.end())
				return (false);
			renderLayers[i].imagess.erase(key);
			return (true);
		}
	}
	return (false);
}

void RenderSystem::RenderAll()
{
	for (int i = 0; i < renderLayers.size(); i++)
	{
		if (renderLayers[i].fbo != NULL)
		{
			renderLayers[i].fbo->Bind();
			glClear(GL_COLOR_BUFFER_BIT);
		}
		std::vector<RenderObj*> objs = {};
		for (auto [key, obj] : renderLayers[i].imagess)
			objs.push_back(obj);
		if (renderLayers[i].sortType == n_SortTypes::Y_SORT)
			std::sort(objs.begin(), objs.end(), SortLayerYSort);
		else if (renderLayers[i].sortType == n_SortTypes::DEPTH_SORT)
			std::sort(objs.begin(), objs.end(), SortLayerDepthSort);
		else if (renderLayers[i].sortType == n_SortTypes::DEPTH_Y_SORT)
			std::sort(objs.begin(), objs.end(), SortLayerDepthAndYSort);
		else if (renderLayers[i].sortType == n_SortTypes::TEXT_LAYER)
			NewImgUiFrame();
		for (int j = 0; j < objs.size(); j++)
		{
			RenderObj *obj = objs[j];
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
		for (auto [key, obj] : renderLayers[i].imagess)
			delete obj;
		renderLayers[i].imagess.clear();
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
	bool ret = universalRenderingSystem.RemoveRenderObject(this, layer, uniqueKey);
}

void InitRenderSystem()
{
	defaultFboShader = new Shader("shaders/default_fbo_vert.glsl", "shaders/default_fbo_frag.glsl");
}
