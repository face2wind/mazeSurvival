#pragma once
#include "scene_obj.h"

class MazeRunner : public SceneObject
{
public:
	MazeRunner(Scene *scene);
	~MazeRunner();

	virtual void Thinking();
	
protected:
};
