#pragma once
#include <position2d.h>

using irr::core::position2di;

class Scene;

enum class Direction // ��������ķ���
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,

	COUNT
};

enum class ActionState
{
	IDLE = 0,			// ��ֹ״̬
	MOVING,				// �ƶ�״̬
};

enum class ObjectType // ������������
{
	RUNNER = 0,
	MONSTER,
};

class SceneObject
{
public:
	SceneObject(Scene *scene);
	virtual ~SceneObject();

	static SceneObject * CreateObject(Scene *scene, ObjectType type);
	
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

	Direction dir_;
	ActionState action_state_;
	ObjectType type_;
	unsigned int color_;

	position2di pix_posi_;
	position2di grid_posi_;
};
