#include "scene.h"
#include <common/random.hpp>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

Scene & Scene::Instance()
{
	static Scene instance;
	return instance;
}

Scene::~Scene()
{
	
}

Scene::Scene(): device_(nullptr), driver_(nullptr)
{
}

void Scene::SetDevice(irr::IrrlichtDevice *device)
{
	if (nullptr == device) return;

	device_ = device;
	driver_ = device->getVideoDriver();

	driver_->getMaterial2D().TextureLayer[0].BilinearFilter = true;
	driver_->getMaterial2D().AntiAliasing = video::EAAM_FULL_BASIC;
}

void Scene::RestartScene()
{
	this->CreateNewMap();
}

int Scene::UpdateView()
{
	if (nullptr == device_ || nullptr == driver_) return -1;
	if (!device_->isWindowActive()) return -2;

	driver_->beginScene(true, true, video::SColor(0xffA8A8A8));// 255, 255, 255, 255));

	for (int x = 0; x <= SCENE_MAP_WIDTH; ++x)
		for (int y = 0; y <= SCENE_MAP_HEIGHT; ++y)
			if (MapDataType::BLOCK == map_data_[x][y]) // 只画障碍区
			driver_->draw2DRectangle(video::SColor(0xff000000),//255, 255, 255, 255),
				core::rect<s32>(x * GRID_LENGTH, y * GRID_LENGTH, x * GRID_LENGTH + GRID_LENGTH, y * GRID_LENGTH + GRID_LENGTH));
/*
	s32 MOVE_WIDTH = 800;
	s32 xpos = (device_->getTimer()->getTime() / 4) % MOVE_WIDTH;
	if (xpos > MOVE_WIDTH/2) xpos = MOVE_WIDTH - xpos;
	driver_->draw2DPolygon(core::vector2d<int>(xpos, 30), 10, video::SColor(255, 255, 255, 255));
*/
	driver_->endScene();

	return 0;
}

int Scene::UpdateLogic()
{
	return 0;
}

void Scene::CreateNewMap()
{
	memset(map_data_, 0, sizeof(map_data_));
	// 先初始化墙
	for (int x = 0; x <= SCENE_MAP_WIDTH; ++x)
		for (int y = 0; y <= SCENE_MAP_HEIGHT; ++y)
			if (0 == x || (SCENE_MAP_WIDTH) == x || 0 == y || (SCENE_MAP_HEIGHT) == y)
				map_data_[x][y] = MapDataType::BLOCK;
			else if (x % 2 == 0 || y % 2 == 0)
				map_data_[x][y] = MapDataType::BLOCK;

	// 再随机拆墙
	for (int x = 0; x < SCENE_MAP_WIDTH; x += 4)
		for (int y = 0; y < SCENE_MAP_HEIGHT; y += 4)
		{
			// 内部连通
			int wall_type = face2wind::Random::RandomNum(4);
			switch (wall_type)
			{
			case 0:
				map_data_[x + 1][y + 2] = MapDataType::EMPTY;
				map_data_[x + 2][y + 3] = MapDataType::EMPTY;
				map_data_[x + 3][y + 2] = MapDataType::EMPTY;
				break;
			case 1:
				map_data_[x + 2][y + 1] = MapDataType::EMPTY;
				map_data_[x + 2][y + 3] = MapDataType::EMPTY;
				map_data_[x + 3][y + 2] = MapDataType::EMPTY;
				break;
			case 2:
				map_data_[x + 2][y + 1] = MapDataType::EMPTY;
				map_data_[x + 1][y + 2] = MapDataType::EMPTY;
				map_data_[x + 3][y + 2] = MapDataType::EMPTY;
				break;
			case 3:
				map_data_[x + 2][y + 1] = MapDataType::EMPTY;
				map_data_[x + 1][y + 2] = MapDataType::EMPTY;
				map_data_[x + 2][y + 3] = MapDataType::EMPTY;
				break;
			}

			// 跟隔壁连通
			wall_type = face2wind::Random::RandomNum(4);
			switch (wall_type)
			{
			case 0:
				if (y > 0)
					map_data_[x + 1][y] = MapDataType::EMPTY;
				break;
			case 1:
				if (x > 0)
					map_data_[x][y + 1] = MapDataType::EMPTY;
				break;
			case 2:
				if (x > 0)
					map_data_[x][y + 3] = MapDataType::EMPTY;
				break;
			case 3:
				if (y > 0)
					map_data_[x + 3][y] = MapDataType::EMPTY;
				break;
			}
		}
}