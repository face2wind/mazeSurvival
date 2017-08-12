#pragma once

#include <vector>

#include "i_path_finder.h"

class AStarPathFinder : public IPathFinder
{
	struct AStarPoint
	{
		AStarPoint() {}

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

	virtual std::vector<irr::core::position2di> FindPath(irr::core::position2di start_point, irr::core::position2di end_point);

protected:
	/**
	* 放入开放列表，插入排序，保证列表最后一个元素的F值最小
	* @param newPoint
	*/
	void AddToOpenList(const AStarPoint &newPoint);

	/**
	* 返回指定点的周围节点
	* @param curPoint
	* @return
	*/
	std::vector<AStarPoint> GetSurroundingPoint(const AStarPoint &curPoint);

	/**
	* 计算当前节点的寻路数值 G, H, F
	* @param preAp 前一个节点（父节点）
	* @param curAp 要计算的节点
	*/
	void CalculateGHF(AStarPoint &preAp, AStarPoint &curAp);

	/**
	* 根据寻路结果生成对应的路径
	* @return
	*/
	std::vector<irr::core::position2di> CreateFinalPath();

	/**
	* 检测指定点是否在地图里
	* @param p
	* @return
	*/
	bool PointInMap(const irr::core::position2di &p) const;


private:
	/*
	 * 地图数据
	 */
	MapDataType map_data_[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT];

	/**
	* 开启列表
	*/
	std::vector<AStarPoint> openList_;
	/**
	* 关闭列表
	*/
	std::vector<AStarPoint> closeList_;
	/**
	* 当前寻路的起始点
	*/
	AStarPoint startAPoint_;
	/**
	* 当前寻路的终点
	*/
	AStarPoint endAPoint_;

	/**
	* 是否8个方向寻路，否则表示4方向寻路
	*/
	bool is_direction8;
};
