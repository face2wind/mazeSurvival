#pragma once

static const int GRID_LENGTH = 40;						// ���ӱ߳������أ�

static const int SCENE_MAP_WIDTH = 21;					// ������ȣ��������� ��ͼ���ӳ����õ�������Ϊ������ǽ
static const int SCENE_MAP_HEIGHT = 15;					// �����߶ȣ���������

enum class MapDataType
{
	EMPTY = 0,		// �հ����򣨿��ߣ�
	BLOCK,			// �ϰ���

	EXPLORE,		// ̽����
};

inline bool MapDataCanMove(MapDataType type) { return MapDataType::BLOCK != type; }

typedef unsigned int ObjectID;							// �����ڵĶ���ΨһID
