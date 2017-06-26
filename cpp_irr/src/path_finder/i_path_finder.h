#pragma once
#include "../basic_type.h"
#include <vector>
#include <position2d.h>

/**
* ��ͼѰ·�ӿ�
* @author face2wind
*/
class IPathFinder
{
public:
	IPathFinder() {}
	virtual ~IPathFinder() {}

	/**
	* ��ͼ��ά���飬�����ֵ�ο� MapDataType
	* @param data
	*/
	virtual void setMapData(MapDataType map_data_[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT]) = 0;

	/**
	* ����·��
	* @param startPoint
	* @param endPoint
	* @return ����Ѱ�õ�·�������飬��������·���ϵĵ�
	*/
	virtual std::vector<irr::core::position2di> findPath(irr::core::position2di start_point, irr::core::position2di end_point) = 0;
};
