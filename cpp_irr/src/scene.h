#pragma once
#include <IrrlichtDevice.h>

static const int GRID_LENGTH = 8;						// 格子边长（像素）

static const int SCENE_MAP_WIDTH = 61;					// 场景宽度（格子数） 地图格子长度用单数，因为两边是墙
static const int SCENE_MAP_HEIGHT = 41;					// 场景高度（格子数）

enum class MapDataType
{
	EMPTY = 0,		// 空白区域（可走）
	BLOCK,			// 障碍区
};

class Scene
{
public:
	static Scene & Instance();

	~Scene();
	
	void SetDevice(irr::IrrlichtDevice *device);

	void RestartScene();

	int UpdateView();
	int UpdateLogic();

protected:
	Scene();

	void CreateNewMap();

	irr::IrrlichtDevice *device_;
	irr::video::IVideoDriver* driver_;

	MapDataType map_data_[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT];
};
