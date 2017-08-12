#pragma once
#include "scene_obj.h"
#include "../basic_type.h"
#include "path_finder/a_star_path_finder.h"

class MazeRunner : public SceneObject
{
public:
	MazeRunner(Scene *scene);
	~MazeRunner();

	virtual void Thinking();
	
protected:
	MapDataType view_map_data_[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT];

	IPathFinder *path_finder_;

};
