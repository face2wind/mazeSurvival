#pragma once
#include "i_path_finder.h"

class AStarPathFinder : public IPathFinder
{
public:
	AStarPathFinder();
	virtual ~AStarPathFinder();

	virtual void setMapData(MapDataType map_data_[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT]);

	virtual std::vector<irr::core::position2di> findPath(irr::core::position2di start_point, irr::core::position2di end_point);
};
