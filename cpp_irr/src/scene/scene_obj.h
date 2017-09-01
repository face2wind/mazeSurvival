#pragma once

#include <position2d.h>
#include <basic_type.h>
#include "path_finder/a_star_path_finder.h"
#include "scene_view.h"

using irr::core::position2di;

class Scene;

enum class Direction // 场景物体的方向
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,

	COUNT
};

enum class ActionState
{
	IDLE = 0,			// 静止状态
	MOVING,				// 移动状态
};

enum class ObjectType // 场景对象类型
{
	RUNNER = 0,
	MONSTER,
};

class SceneObject
{
public:
	SceneObject(Scene *scene);
	virtual ~SceneObject();

	void SetID(int new_obj_id);
	ObjectID GetID() const { return object_id_; }

	static SceneObject * CreateObject(Scene *scene, ObjectType type);
	
	SceneView *GetSceneView() { return &scene_view_; }
	void UpdateSceneViewSelfObject();

	Direction GetDir() const { return dir_; }
	void SetDir(Direction dir) { dir_ = dir; }
	
	ActionState GetActionState() const { return action_state_; }
	void SetActionState(ActionState state) { action_state_ = state; }

	ObjectType GetType() const { return type_; }
	void SetType(ObjectType type) { type_ = type; }

	unsigned int GetColor() const { return color_; }
	const position2di & GetPixelPosition() const { return pix_posi_; }
	const position2di & GetGridPosition() const { return grid_posi_; }
	void SetGridPosition(position2di posi);
	void SetPixPosition(position2di posi);

	void Update(long long interval);

	virtual void Thinking(long long interval) {}

protected:
	Scene *scene_;

	SceneView scene_view_;

	ObjectID object_id_;

	Direction dir_;
	ActionState action_state_;
	ObjectType type_;
	unsigned int color_;

	Point2d pix_posi_;
	Point2d grid_posi_;
};
