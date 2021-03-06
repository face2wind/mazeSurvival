#pragma once
#include <vector>
#include <position2d.h>

#include "basic_type.h"

typedef irr::core::position2di Point2d;

enum class PathFinderType
{
	Astar = 0,
};

/**
* 地图寻路接口
* @author face2wind
*/
class IPathFinder
{
public:

	IPathFinder() {}
	virtual ~IPathFinder() {}

	static IPathFinder * CreatePathFinder(PathFinderType type);

	/**
	* 地图二维数组，里面的值参考 MapDataType
	* @param data
	*/
	virtual void SetMapData(MapDataType map_data_[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT]) = 0;

	/**
	* 查找路径
	* @param startPoint
	* @param endPoint
	* @return 返回寻好的路径点数组，包含所有路径上的点
	*/
	virtual std::vector<Point2d> FindPath(Point2d start_point, Point2d end_point) = 0;
};
