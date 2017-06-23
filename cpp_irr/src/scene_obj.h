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
	ObjectType GetType() const { return type_; }
	void SetType(ObjectType type) { type_ = type; }

	unsigned int GetColor() const { return color_; }
	const position2di & GetPixelPosition() const { return pix_posi_; }
	const position2di & GetGridPosition() const { return grid_posi_; }
	void SetGridPosition(position2di posi);

	void MoveStep();
	void Update(long long interval);

	virtual void Thinking() {}

protected:
	Scene *scene_;

	Direction dir_;
	ObjectType type_;
	unsigned int color_;

	position2di pix_posi_;
	position2di grid_posi_;
};
