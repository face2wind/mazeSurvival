#pragma once
#include <IrrlichtDevice.h>

static const int GRID_LENGTH = 5;						// ���ӱ߳������أ�

static const int SCENE_MAP_WIDTH = 251;					// ������ȣ��������� ��ͼ���ӳ����õ�������Ϊ������ǽ
static const int SCENE_MAP_HEIGHT = 151;				// �����߶ȣ���������

enum class MapDataType
{
	EMPTY = 0,		// �հ����򣨿��ߣ�
	BLOCK,			// �ϰ���
};

class Scene
{
public:
	static Scene & Instance();

	~Scene();
	
	void SetDevice(irr::IrrlichtDevice *device);

	void RestartScene();

	int UpdateView();
	int UpdateLogic();

protected:
	Scene();

	void CreateMapPureRandom();	// �Լ�д�������ǽ����ͼ
	void CreateMapPrim();		// ��Prim�㷨���ɵ�ͼ
	void CreateNewMap();

	irr::IrrlichtDevice *device_;
	irr::video::IVideoDriver* driver_;

	MapDataType map_data_[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT];
};
