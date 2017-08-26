#include "scene_obj.h"
#include "scene.h"
#include "runner.h"

SceneObject::SceneObject(Scene *scene) : scene_(scene), dir_(Direction::DOWN), action_state_(ActionState::IDLE), type_(ObjectType::RUNNER), color_(0xff0388a0)
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
		object = new Runner(scene);
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

void SceneObject::SetPixPosition(position2di posi)
{
	pix_posi_ = posi;
	grid_posi_.X = pix_posi_.X / GRID_LENGTH;
	grid_posi_.Y = pix_posi_.Y / GRID_LENGTH;
}

void SceneObject::Update(long long interval)
{
	if (interval % GRID_LENGTH == 0)
		this->Thinking(interval);
}
