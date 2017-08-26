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
		end_apoint->prePoint = nullptr;		// ����һ���յ��ָ��

		AStarPoint *start_apoint = &map_data_[start_point.X][start_point.Y];
		start_apoint->h = (std::abs(end_point.X - start_apoint->x) + std::abs(end_point.Y - start_apoint->y)) * 10;	// �����ٷ�����Hֵ
		start_apoint->f = start_apoint->h;	// һ��ʼg��0��f��h���
		start_apoint->prePoint = nullptr;	// ����һ������ָ��
		openList.insert(start_apoint);

		while (!openList.empty())
		{
			// �ӿ����б���ȡ��һ����
			AStarPoint *cur_point = *openList.begin();
			openList.erase(openList.begin());

			// �����Χ�ڵ�
			std::vector<AStarPoint*> surround_point_list = this->GetSurroundingPoint(*cur_point);
			for (AStarPoint* point : surround_point_list)
			{
				if (closeList[point->y * SCENE_MAP_WIDTH + point->x] > 0) // �ڹر��б��������
					continue;

				int tmp_h = (std::abs(end_point.X - point->x) + std::abs(end_point.Y - point->y)) * 10;	// �����ٷ�����Hֵ
				int tmp_g = 10;
				if (point->x != cur_point->x && point->y != cur_point->y) // ���ڶԽ��ߣ�Gֵ��14
					tmp_g = 14;
				int tmp_f = tmp_g + tmp_h;

				// �����б����������i�ڵ㣬���Ҿɵ�FֵС���µ�Fֵ��˵���ɵ�·�߸��ţ�����
				if (openList.find(point) != openList.end() && tmp_f > point->f)
					continue;

				openList.insert(point);

				point->level = cur_point->level + 1;
				point->f = tmp_f;
				point->g = tmp_g;
				point->h = tmp_h;
				point->prePoint = cur_point;
			}

			// �ѵ�ǰ�����close�б�
			closeList[cur_point->y * SCENE_MAP_WIDTH + cur_point->x] = 1;
		}

	} while (false);

	AStarPoint *cur_apoint = end_apoint;	// ���յ����ر���ָ�룬��¼·��
	while (nullptr != cur_apoint)
	{
		path.push_back(Point2d(cur_apoint->x, cur_apoint->y));
		cur_apoint = cur_apoint->prePoint;
	}

	if (path.back() != start_point) // �ҵ���·�ߵ��յ㲢������㣬���·��
		path.clear();

	return path;
}

std::vector<AStarPathFinder::AStarPoint*> AStarPathFinder::GetSurroundingPoint(const AStarPoint &curPoint)
{
	std::vector<AStarPathFinder::AStarPoint*> surround_point_list;
	
	if (0 < curPoint.x  && (MapDataType::EMPTY == map_data_[curPoint.x - 1][curPoint.y].mapType)) // ��
		surround_point_list.push_back(&map_data_[curPoint.x - 1][curPoint.y]);
	if (SCENE_MAP_WIDTH > curPoint.x && (MapDataType::EMPTY == map_data_[curPoint.x + 1][curPoint.y].mapType)) // ��
		surround_point_list.push_back(&map_data_[curPoint.x + 1][curPoint.y]);
	if (0 < curPoint.y && (MapDataType::EMPTY == map_data_[curPoint.x][curPoint.y - 1].mapType)) // ��
		surround_point_list.push_back(&map_data_[curPoint.x][curPoint.y - 1]);
	if (SCENE_MAP_HEIGHT > curPoint.y && (MapDataType::EMPTY == map_data_[curPoint.x][curPoint.y + 1].mapType)) // ��
		surround_point_list.push_back(&map_data_[curPoint.x][curPoint.y + 1]);

	if (!is_direction8_)
		return surround_point_list;

	if (0 < curPoint.x && 0 < curPoint.y &&
		(MapDataType::EMPTY == map_data_[curPoint.x - 1][curPoint.y - 1].mapType) && // ����
		(MapDataType::EMPTY == map_data_[curPoint.x - 1][curPoint.y].mapType && !(MapDataType::EMPTY == map_data_[curPoint.x][curPoint.y - 1].mapType))) // û�б����߼�ס
		surround_point_list.push_back(&map_data_[curPoint.x - 1][curPoint.y - 1]);

	if (SCENE_MAP_WIDTH > curPoint.x && 0 < curPoint.y &&
		(MapDataType::EMPTY == map_data_[curPoint.x + 1][curPoint.y - 1].mapType) && // ����
		(MapDataType::EMPTY == map_data_[curPoint.x][curPoint.y - 1].mapType && !(MapDataType::EMPTY == map_data_[curPoint.x + 1][curPoint.y].mapType))) // û�б����߼�ס
		surround_point_list.push_back(&map_data_[curPoint.x + 1][curPoint.y - 1]);

	if (SCENE_MAP_WIDTH > curPoint.x && SCENE_MAP_HEIGHT > curPoint.y &&
		(MapDataType::EMPTY == map_data_[curPoint.x + 1][curPoint.y + 1].mapType) && // ����
		(MapDataType::EMPTY == map_data_[curPoint.x][curPoint.y + 1].mapType && !(MapDataType::EMPTY == map_data_[curPoint.x + 1][curPoint.y].mapType))) // û�б����߼�ס
		surround_point_list.push_back(&map_data_[curPoint.x + 1][curPoint.y + 1]);

	if (0 < curPoint.x && SCENE_MAP_HEIGHT > curPoint.y &&
		(MapDataType::EMPTY == map_data_[curPoint.x - 1][curPoint.y + 1].mapType) && // ����
		(MapDataType::EMPTY == map_data_[curPoint.x][curPoint.y + 1].mapType && !(MapDataType::EMPTY == map_data_[curPoint.x - 1][curPoint.y].mapType))) // û�б����߼�ס
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
