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
	MapDataType view_map_data_[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT];

	IPathFinder *path_finder_;

	std::vector<Point2d> cur_moving_path_;
	Point2d next_move_point_;

};
