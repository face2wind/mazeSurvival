#pragma once

#include <irrlicht.h>
#include <map>
#include "basic_type.h"
#include "path_finder/a_star_path_finder.h"

struct SceneViewObj
{
	ObjectID object_id;

	unsigned int color;

	Point2d pix_posi;
	Point2d grid_posi;
};

class SceneView
{
public:
	SceneView();
	~SceneView();
	
	int UpdateView(irr::video::IVideoDriver* driver_);

	void GetMapData(MapDataType map_data_[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT]);

	MapDataType map_data_[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT];
	std::map<ObjectID, SceneViewObj> object_map_;
};
