#pragma once

#include <vector>

#include "i_path_finder.h"

class AStarPathFinder : public IPathFinder
{
	struct AStarPoint
	{
		AStarPoint() { this->Reset(0, 0); }

		void Reset(int xpos, int ypos)
		{
			x = xpos;
			y = ypos;
			f = 0;
			g = 0;
			h = 0;
			level = 0;
			mapType = MapDataType::BLOCK;
			prePoint = nullptr;
		}

		int x;
		int y;
		int f;
		int g;
		int h;
		/**
		* 层数，数字越大代表是越新的路径，开放列表排序把新生成的列表优先检测 （深度优先）
		*/
		int level;
		MapDataType mapType;
		AStarPoint *prePoint;
	};

public:
	AStarPathFinder();
	virtual ~AStarPathFinder();

	virtual void SetMapData(MapDataType map_data[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT]);

	virtual std::vector<Point2d> FindPath(Point2d start_point, Point2d end_point);

protected:
	/**
	* 返回指定点的周围节点
	* @param curPoint
	* @return
	*/
	std::vector<AStarPoint*> GetSurroundingPoint(const AStarPoint &curPoint);
	
	/**
	* 根据寻路结果生成对应的路径
	* @return
	*/
	std::vector<Point2d> CreateFinalPath(Point2d start_point, Point2d end_point);

	/**
	* 检测指定点是否在地图里
	* @param p
	* @return
	*/
	bool PointInMap(const Point2d &p) const;


private:
	/*
	 * 地图数据
	 */
	AStarPoint map_data_[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT];
	
	/**
	* 是否8个方向寻路，否则表示4方向寻路
	*/
	bool is_direction8_;
};
