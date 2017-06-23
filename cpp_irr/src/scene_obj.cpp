#include "scene_obj.h"
#include "scene.h"
#include "maze_runner.h"

SceneObject::SceneObject(Scene *scene) : scene_(scene), dir_(Direction::DOWN), type_(ObjectType::RUNNER), color_(0xff0388a0)
{
}

SceneObject::~SceneObject()
{
	
}

SceneObject * SceneObject::CreateObject(Scene *scene, ObjectType type)
{
	SceneObject *object;

	switch (type)
	{
	case ObjectType::RUNNER:
		object = new MazeRunner(scene);
		break;

	default:
		object = new SceneObject(scene);
		break;
	}

	object->type_ = type;
	return object;
}

void SceneObject::SetGridPosition(position2di posi)
{
	grid_posi_ = posi;
	pix_posi_.X = grid_posi_.X * GRID_LENGTH;
	pix_posi_.Y = grid_posi_.Y * GRID_LENGTH;
}

void SceneObject::MoveStep()
{
	if (Direction::UP == dir_)
	{
		--pix_posi_.Y;
    }
	else if (Direction::DOWN == dir_)
    {
        ++pix_posi_.Y;
    }
    else if(Direction::LEFT == dir_)
    {
		--pix_posi_.X;
    }
    else if (Direction::RIGHT == dir_)
    {
        ++pix_posi_.X;
	}

	grid_posi_.X = pix_posi_.X / GRID_LENGTH;
	grid_posi_.Y = pix_posi_.Y / GRID_LENGTH;
}

void SceneObject::Update(long long interval)
{
	if (interval % 20 == 0)
		this->Thinking();
}