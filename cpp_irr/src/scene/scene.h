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

	void CreateMapPureRandom();	// 自己写的随机拆墙建地图
	void CreateMapPrim();		// 用Prim算法生成地图

	int GetNewObjID();			// 获取新的可用ObjID
	void CreateInitObjs();

	irr::IrrlichtDevice *device_;
	irr::video::IVideoDriver* driver_;

	SceneView scene_view_;
	std::list<SceneObject*> object_list_;
	//MapDataType map_data_[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT];

	SceneView *showing_scene_view_;	// 当前展示的视图

	ObjectID cur_max_object_id_;			// 当前已分配过最大objID
	std::stack<ObjectID> recycle_object_id_list_; // 已回收的空闲objID
};
