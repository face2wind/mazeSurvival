#include "a_star_path_finder.h"
#include <set>


AStarPathFinder::AStarPathFinder(): is_direction8_(false)
{
}

AStarPathFinder::~AStarPathFinder()
{
	
}

void AStarPathFinder::SetMapData(MapDataType map_data[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT])
{
	for (int x = 0; x < SCENE_MAP_WIDTH; ++x)
		for (int y = 0; y < SCENE_MAP_HEIGHT; ++y)
		{
			map_data_[x][y].Reset(x, y);
			map_data_[x][y].mapType = map_data[x][y];
		}
}

std::vector<Point2d> AStarPathFinder::FindPath(Point2d start_point, Point2d end_point)
{
	std::vector<Point2d> path;
	AStarPoint *end_apoint = nullptr;

	do
	{
		if (!this->PointInMap(start_point) || !this->PointInMap(end_point))
		{
			break;
		}

		if (MapDataType::EMPTY != map_data_[start_point.X][start_point.Y].mapType || MapDataType::EMPTY != map_data_[end_point.X][end_point.Y].mapType)
		{
			break;
		}

		std::set<AStarPoint*> openList;
		char closeList[SCENE_MAP_WIDTH*SCENE_MAP_HEIGHT] = { 0 };

		end_apoint = &map_data_[end_point.X][end_point.Y];
		end_apoint->prePoint = nullptr;		// 重置一下终点的指向

		AStarPoint *start_apoint = &map_data_[start_point.X][start_point.Y];
		start_apoint->h = (std::abs(end_point.X - start_apoint->x) + std::abs(end_point.Y - start_apoint->y)) * 10;	// 曼哈顿方法求H值
		start_apoint->f = start_apoint->h;	// 一开始g是0，f跟h相等
		start_apoint->prePoint = nullptr;	// 重置一下起点的指向
		openList.insert(start_apoint);

		while (!openList.empty())
		{
			// 从开放列表里取出一个点
			AStarPoint *cur_point = *openList.begin();
			openList.erase(openList.begin());

			// 检测周围节点
			std::vector<AStarPoint*> surround_point_list = this->GetSurroundingPoint(*cur_point);
			for (AStarPoint* point : surround_point_list)
			{
				if (closeList[point->y * SCENE_MAP_WIDTH + point->x] > 0) // 在关闭列表里，不处理
					continue;

				int tmp_h = (std::abs(end_point.X - point->x) + std::abs(end_point.Y - point->y)) * 10;	// 曼哈顿方法求H值
				int tmp_g = 10;
				if (point->x != cur_point->x && point->y != cur_point->y) // 处于对角线，G值是14
					tmp_g = 14;
				int tmp_f = tmp_g + tmp_h;

				// 开放列表里已有这个i节点，而且旧的F值小于新的F值，说明旧的路线更优，忽略
				if (openList.find(point) != openList.end() && tmp_f > point->f)
					continue;

				openList.insert(point);

				point->level = cur_point->level + 1;
				point->f = tmp_f;
				point->g = tmp_g;
				point->h = tmp_h;
				point->prePoint = cur_point;
			}

			// 把当前点放入close列表
			closeList[cur_point->y * SCENE_MAP_WIDTH + cur_point->x] = 1;
		}

	} while (false);

	AStarPoint *cur_apoint = end_apoint;	// 从终点往回遍历指针，记录路线
	while (nullptr != cur_apoint)
	{
		path.push_back(Point2d(cur_apoint->x, cur_apoint->y));
		cur_apoint = cur_apoint->prePoint;
	}

	if (path.back() != start_point) // 找到的路线的终点并不是起点，清空路线
		path.clear();

	return path;
}

std::vector<AStarPathFinder::AStarPoint*> AStarPathFinder::GetSurroundingPoint(const AStarPoint &curPoint)
{
	std::vector<AStarPathFinder::AStarPoint*> surround_point_list;
	
	if (0 < curPoint.x  && (MapDataType::EMPTY == map_data_[curPoint.x - 1][curPoint.y].mapType)) // 左
		surround_point_list.push_back(&map_data_[curPoint.x - 1][curPoint.y]);
	if (SCENE_MAP_WIDTH > curPoint.x && (MapDataType::EMPTY == map_data_[curPoint.x + 1][curPoint.y].mapType)) // 右
		surround_point_list.push_back(&map_data_[curPoint.x + 1][curPoint.y]);
	if (0 < curPoint.y && (MapDataType::EMPTY == map_data_[curPoint.x][curPoint.y - 1].mapType)) // 上
		surround_point_list.push_back(&map_data_[curPoint.x][curPoint.y - 1]);
	if (SCENE_MAP_HEIGHT > curPoint.y && (MapDataType::EMPTY == map_data_[curPoint.x][curPoint.y + 1].mapType)) // 下
		surround_point_list.push_back(&map_data_[curPoint.x][curPoint.y + 1]);

	if (!is_direction8_)
		return surround_point_list;

	if (0 < curPoint.x && 0 < curPoint.y &&
		(MapDataType::EMPTY == map_data_[curPoint.x - 1][curPoint.y - 1].mapType) && // 左上
		(MapDataType::EMPTY == map_data_[curPoint.x - 1][curPoint.y].mapType && !(MapDataType::EMPTY == map_data_[curPoint.x][curPoint.y - 1].mapType))) // 没有被两边夹住
		surround_point_list.push_back(&map_data_[curPoint.x - 1][curPoint.y - 1]);

	if (SCENE_MAP_WIDTH > curPoint.x && 0 < curPoint.y &&
		(MapDataType::EMPTY == map_data_[curPoint.x + 1][curPoint.y - 1].mapType) && // 右上
		(MapDataType::EMPTY == map_data_[curPoint.x][curPoint.y - 1].mapType && !(MapDataType::EMPTY == map_data_[curPoint.x + 1][curPoint.y].mapType))) // 没有被两边夹住
		surround_point_list.push_back(&map_data_[curPoint.x + 1][curPoint.y - 1]);

	if (SCENE_MAP_WIDTH > curPoint.x && SCENE_MAP_HEIGHT > curPoint.y &&
		(MapDataType::EMPTY == map_data_[curPoint.x + 1][curPoint.y + 1].mapType) && // 右下
		(MapDataType::EMPTY == map_data_[curPoint.x][curPoint.y + 1].mapType && !(MapDataType::EMPTY == map_data_[curPoint.x + 1][curPoint.y].mapType))) // 没有被两边夹住
		surround_point_list.push_back(&map_data_[curPoint.x + 1][curPoint.y + 1]);

	if (0 < curPoint.x && SCENE_MAP_HEIGHT > curPoint.y &&
		(MapDataType::EMPTY == map_data_[curPoint.x - 1][curPoint.y + 1].mapType) && // 左下
		(MapDataType::EMPTY == map_data_[curPoint.x][curPoint.y + 1].mapType && !(MapDataType::EMPTY == map_data_[curPoint.x - 1][curPoint.y].mapType))) // 没有被两边夹住
		surround_point_list.push_back(&map_data_[curPoint.x - 1][curPoint.y + 1]);

	return surround_point_list;
}

std::vector<Point2d> AStarPathFinder::CreateFinalPath(Point2d start_point, Point2d end_point)
{
	std::vector<Point2d> path;

	return path;
}

bool AStarPathFinder::PointInMap(const Point2d &p) const
{
	return (p.X >= 0 && p.X <= SCENE_MAP_WIDTH);
}
