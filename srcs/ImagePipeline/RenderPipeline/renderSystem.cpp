
#include "systemObj.h"
#include "renderSystem.h"
#include "pillarBoxes.h"
#include "screen.h"
#include "ShaderClass.h"
#include "mainTools.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "snapshot.h"
#include "saveInterface.h"
#include "commonTools.h"
#include <thread>

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

std::vector<std::tuple<int, int>> RenderSystem::GetLayerData()
{
	std::vector<std::tuple<int, int>> ret = {};
	for (int i = 0; i < renderLayers.size(); i++)
		ret.push_back({renderLayers[i].layerNumber, renderLayers[i].sortType});
	return (ret);
}

bool RenderSystem::RemoveLayer(int layerNumber)
{
	for (int i = 0; i < renderLayers.size(); i++)
	{
		if (renderLayers[i].layerNumber == layerNumber)
		{
			if (renderLayers[i].imagess.size() > 0)
				return (false);
			renderLayers.erase(renderLayers.begin() + i);
			SaveLayers();
			return (true);
		}
	}
	return (false);
}

void RenderSystem::Init()
{
	uint32_t key = SetAskedData("saves/renderer/layers.2E");
	void *state = NULL;
	for (int i = 0; i < 15; i++)
	{
		bool checker = CollectAskedState(key, &state);
		if (checker)
			break ;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	if (state == NULL)
		return ;
	SnapShot data = MakeIntoSnapshot(state);
	size_t offset = 0;
	char *cast = (char*)data.data;
	while (offset < data.size)
	{
		int layerNumber = 0;
		int sortType = 0;
		memcpy(&layerNumber, cast + offset, sizeof(int));
		offset += sizeof(int);
		memcpy(&sortType, cast + offset, sizeof(int));
		offset += sizeof(int);
		AddLayerOwn(layerNumber, sortType);
	}
	free(state);
	free(data.data);
}

void RenderSystem::SaveLayers()
{
	size_t offset = 0;
	size_t size = (sizeof(int) + sizeof(int)) * renderLayers.size();
	void *data = malloc(size);
	char *cast = (char*)data;
	for (int i = 0; i < renderLayers.size(); i++)
	{
		memcpy(cast + offset, &renderLayers[i].layerNumber, sizeof(int));
		offset += sizeof(int);
		memcpy(cast + offset, &renderLayers[i].sortType, sizeof(int));
		offset += sizeof(int);
	}
	uint64_t hash = HashData64(data, size);
	SaveSnapShot({hash, (uint32_t)size, data}, "saves/renderer/layers.2E");
	SaveSnapShot({hash, (uint32_t)size, data}, "saves/renderer/layers.2E");
}

void RenderSystem::AddLayerOwn(int layerNumber, int sortType)
{
	for (int i = 0; i < renderLayers.size(); i++)
	{
		if (renderLayers[i].layerNumber == layerNumber)
			return ;
	}
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

void RenderSystem::AddLayer(int layerNumber, int sortType)
{
	AddLayerOwn(layerNumber, sortType);
	SaveLayers();
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
		if (renderLayers[i].layerNumber == layer)
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
		{
			obj->BeforeDraw();
			if (obj->OffscreenDetection())
				continue ;
			obj->SetDrawY();
			objs.push_back(obj);
		}
		if (renderLayers[i].sortType == n_SortTypes::Y_SORT)
			std::sort(objs.begin(), objs.end(), SortLayerYSort);
		else if (renderLayers[i].sortType == n_SortTypes::DEPTH_SORT)
			std::sort(objs.begin(), objs.end(), SortLayerDepthSort);
		else if (renderLayers[i].sortType == n_SortTypes::DEPTH_Y_SORT)
			std::sort(objs.begin(), objs.end(), SortLayerDepthAndYSort);
		else if (renderLayers[i].sortType == n_SortTypes::TEXT_LAYER)
			NewImgUiFrame();
		else if (renderLayers[i].sortType == n_SortTypes::MULTI_LAYER)
			std::sort(objs.begin(), objs.end(), SortLayerDepthSort);
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

void RenderObj::ChangeLayer(int layer)
{
	if (layer == RenderObj::layer)
		return ;
	universalRenderingSystem.RemoveRenderObject(this, RenderObj::layer, uniqueKey);
	universalRenderingSystem.AddRenderObject(this, layer, uniqueKey);
	RenderObj::layer = layer;
}

RenderObj::~RenderObj()
{
	if (self != NULL)
	{
		SystemObj *obj = (SystemObj*)self;
		obj->RemoveComponent(id);
	}
	universalRenderingSystem.RemoveRenderObject(this, layer, uniqueKey);
}

void InitRenderSystem()
{
	defaultFboShader = new Shader("shaders/default_fbo_vert.glsl", "shaders/default_fbo_frag.glsl");
}
