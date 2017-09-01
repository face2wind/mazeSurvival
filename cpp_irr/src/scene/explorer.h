#pragma once
#include "scene_obj.h"
#include "basic_type.h"
#include "path_finder/a_star_path_finder.h"

/*
 * 探索者，不断探索路线，直到找到出口
 */
class Explorer : public SceneObject
{
public:
	Explorer(Scene *scene);
	~Explorer();

	virtual void Thinking(long long interval);

protected:
	void MovingWithPath();
	void StartMove();
	void StopMove();

	void StartExplore();
	void StopExplore();

	void ReInitExploreData();							// 遍历一遍自己的视野地图，重新更新所有的探索数据
	void UpdateVision();								// 从场景里获取当前的视野并更新到自己的视野地图里，增量改变探索数据
	
protected:
	//MapDataType view_map_data_[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT];
	IPathFinder *path_finder_;

	std::vector<Point2d> cur_moving_path_;
	Point2d next_move_point_;

	bool exploring_;									// 是否开启了探索模式
	std::vector<Point2d> explorer_point_list_;			// 探索点
};
