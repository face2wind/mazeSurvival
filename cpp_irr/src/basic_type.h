#pragma once

static const int GRID_LENGTH = 20;						// ���ӱ߳������أ�

static const int SCENE_MAP_WIDTH = 51;					// ������ȣ��������� ��ͼ���ӳ����õ�������Ϊ������ǽ
static const int SCENE_MAP_HEIGHT = 21;				// �����߶ȣ���������


struct Position
{
	int x;
	int y;
};

enum class MapDataType
{
	EMPTY = 0,		// �հ����򣨿��ߣ�
	BLOCK,			// �ϰ���
};