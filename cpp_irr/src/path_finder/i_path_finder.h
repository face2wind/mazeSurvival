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
* ��ͼѰ·�ӿ�
* @author face2wind
*/
class IPathFinder
{
public:

	IPathFinder() {}
	virtual ~IPathFinder() {}

	static IPathFinder * CreatePathFinder(PathFinderType type);

	/**
	* ��ͼ��ά���飬�����ֵ�ο� MapDataType
	* @param data
	*/
	virtual void SetMapData(MapDataType map_data_[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT]) = 0;

	/**
	* ����·��
	* @param startPoint
	* @param endPoint
	* @return ����Ѱ�õ�·�������飬��������·���ϵĵ�
	*/
	virtual std::vector<Point2d> FindPath(Point2d start_point, Point2d end_point) = 0;
};
