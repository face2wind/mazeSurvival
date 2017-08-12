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
		* ����������Խ�������Խ�µ�·���������б�����������ɵ��б����ȼ�� ��������ȣ�
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
	* ���뿪���б��������򣬱�֤�б����һ��Ԫ�ص�Fֵ��С
	* @param newPoint
	*/
	void AddToOpenList(const AStarPoint &newPoint);

	/**
	* ����ָ�������Χ�ڵ�
	* @param curPoint
	* @return
	*/
	std::vector<AStarPoint> GetSurroundingPoint(const AStarPoint &curPoint);

	/**
	* ���㵱ǰ�ڵ��Ѱ·��ֵ G, H, F
	* @param preAp ǰһ���ڵ㣨���ڵ㣩
	* @param curAp Ҫ����Ľڵ�
	*/
	void CalculateGHF(AStarPoint &preAp, AStarPoint &curAp);

	/**
	* ����Ѱ·������ɶ�Ӧ��·��
	* @return
	*/
	std::vector<irr::core::position2di> CreateFinalPath();

	/**
	* ���ָ�����Ƿ��ڵ�ͼ��
	* @param p
	* @return
	*/
	bool PointInMap(const irr::core::position2di &p) const;


private:
	/*
	 * ��ͼ����
	 */
	MapDataType map_data_[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT];

	/**
	* �����б�
	*/
	std::vector<AStarPoint> openList_;
	/**
	* �ر��б�
	*/
	std::vector<AStarPoint> closeList_;
	/**
	* ��ǰѰ·����ʼ��
	*/
	AStarPoint startAPoint_;
	/**
	* ��ǰѰ·���յ�
	*/
	AStarPoint endAPoint_;

	/**
	* �Ƿ�8������Ѱ·�������ʾ4����Ѱ·
	*/
	bool is_direction8;
};
