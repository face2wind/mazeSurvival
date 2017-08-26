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

	virtual std::vector<Point2d> FindPath(Point2d start_point, Point2d end_point);

protected:
	/**
	* ����ָ�������Χ�ڵ�
	* @param curPoint
	* @return
	*/
	std::vector<AStarPoint*> GetSurroundingPoint(const AStarPoint &curPoint);
	
	/**
	* ����Ѱ·������ɶ�Ӧ��·��
	* @return
	*/
	std::vector<Point2d> CreateFinalPath(Point2d start_point, Point2d end_point);

	/**
	* ���ָ�����Ƿ��ڵ�ͼ��
	* @param p
	* @return
	*/
	bool PointInMap(const Point2d &p) const;


private:
	/*
	 * ��ͼ����
	 */
	AStarPoint map_data_[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT];
	
	/**
	* �Ƿ�8������Ѱ·�������ʾ4����Ѱ·
	*/
	bool is_direction8_;
};
