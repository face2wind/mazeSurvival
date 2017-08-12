#pragma once
#include <vector>
#include <position2d.h>

#include "basic_type.h"

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
	virtual std::vector<irr::core::position2di> FindPath(irr::core::position2di start_point, irr::core::position2di end_point) = 0;
};
