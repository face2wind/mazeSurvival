#pragma once

static const int GRID_LENGTH = 20;						// 格子边长（像素）

static const int SCENE_MAP_WIDTH = 51;					// 场景宽度（格子数） 地图格子长度用单数，因为两边是墙
static const int SCENE_MAP_HEIGHT = 21;				// 场景高度（格子数）


struct Position
{
	int x;
	int y;
};

enum class MapDataType
{
	EMPTY = 0,		// 空白区域（可走）
	BLOCK,			// 障碍区
};