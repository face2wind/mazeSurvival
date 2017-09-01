#pragma once
//#include <IrrlichtDevice.h>s
#include <irrlicht.h>
#include <list>

#include "basic_type.h"
#include "path_finder/a_star_path_finder.h"
#include "scene_view.h"
#include <stack>

class SceneObject;

class Scene
{
public:
	static Scene & Instance();

	~Scene();
	
	void SetDevice(irr::IrrlichtDevice *device);

	void RestartScene();

	int UpdateView();
	int UpdateLogic(long long interval);

	void GetMapData(MapDataType map_data_[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT]);
	std::vector<Point2d> UpdateVision(Point2d &point, SceneView &target_view);

	Point2d GetRandomMovingPoint2d() const;

protected:
	Scene();

	void CreateMapPureRandom();	// �Լ�д�������ǽ����ͼ
	void CreateMapPrim();		// ��Prim�㷨���ɵ�ͼ

	int GetNewObjID();			// ��ȡ�µĿ���ObjID
	void CreateInitObjs();

	irr::IrrlichtDevice *device_;
	irr::video::IVideoDriver* driver_;

	SceneView scene_view_;
	std::list<SceneObject*> object_list_;
	//MapDataType map_data_[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT];

	SceneView *showing_scene_view_;	// ��ǰչʾ����ͼ

	ObjectID cur_max_object_id_;			// ��ǰ�ѷ�������objID
	std::stack<ObjectID> recycle_object_id_list_; // �ѻ��յĿ���objID
};
