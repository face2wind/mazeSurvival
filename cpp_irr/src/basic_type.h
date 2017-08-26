#pragma once

static const int GRID_LENGTH = 8;						// 格子边长（像素）

static const int SCENE_MAP_WIDTH = 101;					// 场景宽度（格子数） 地图格子长度用单数，因为两边是墙
static const int SCENE_MAP_HEIGHT = 71;					// 场景高度（格子数）

enum class MapDataType
{
	EMPTY = 0,		// 空白区域（可走）
	BLOCK,			// 障碍区
};
