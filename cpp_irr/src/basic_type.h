#pragma once

static const int GRID_LENGTH = 5;						// ���ӱ߳������أ�

static const int SCENE_MAP_WIDTH = 255;					// ������ȣ��������� ��ͼ���ӳ����õ�������Ϊ������ǽ
static const int SCENE_MAP_HEIGHT = 155;					// �����߶ȣ���������

enum class MapDataType
{
	EMPTY = 0,		// �հ����򣨿��ߣ�
	BLOCK,			// �ϰ���
};