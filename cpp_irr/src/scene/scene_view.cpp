#include "scene_view.h"
#include "scene_obj.h"

#include <elegance/common/random.hpp>
#include <vector>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using face2wind::Random;

SceneView::~SceneView()
{
	
}

SceneView::SceneView()
{
}

int SceneView::UpdateView(irr::video::IVideoDriver* driver)
{
	// 渲染场景
	for (int x = 0; x <= SCENE_MAP_WIDTH; ++x)
		for (int y = 0; y <= SCENE_MAP_HEIGHT; ++y)
		{
			if (MapDataType::BLOCK == map_data_[x][y])
				driver->draw2DRectangle(video::SColor(0xff000000),//255, 255, 255, 255),
					core::rect<s32>(x * GRID_LENGTH, y * GRID_LENGTH, x * GRID_LENGTH + GRID_LENGTH, y * GRID_LENGTH + GRID_LENGTH));
			else if (MapDataType::EXPLORE == map_data_[x][y])
				driver->draw2DRectangle(video::SColor(0xff00aa00),//255, 255, 255, 255),
					core::rect<s32>(x * GRID_LENGTH, y * GRID_LENGTH, x * GRID_LENGTH + GRID_LENGTH, y * GRID_LENGTH + GRID_LENGTH));
		}


	// 渲染场景对象
	for (auto obj_it : object_map_)
	{
		const position2di &pos = obj_it.second.pix_posi;
		driver->draw2DRectangle(video::SColor(obj_it.second.color),//255, 255, 255, 255),
			core::rect<s32>(pos.X, pos.Y, pos.X + GRID_LENGTH, pos.Y + GRID_LENGTH));
	}

	return 0;
}

void SceneView::GetMapData(MapDataType other_map_data[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT])
{
	memcpy(other_map_data, map_data_, sizeof(map_data_));
}
