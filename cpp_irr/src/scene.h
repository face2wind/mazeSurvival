#pragma once
//#include <IrrlichtDevice.h>s
#include <irrlicht.h>
#include <list>
#include "basic_type.h"

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

protected:
	Scene();

	void CreateMapPureRandom();	// 自己写的随机拆墙建地图
	void CreateMapPrim();		// 用Prim算法生成地图

	void CreateInitObjs();

	irr::IrrlichtDevice *device_;
	irr::video::IVideoDriver* driver_;

	MapDataType map_data_[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT];
	std::list<SceneObject*> object_list_;
};
