#include "a_star_path_finder.h"


AStarPathFinder::AStarPathFinder(): is_direction8(false)
{
}

AStarPathFinder::~AStarPathFinder()
{
	
}

void AStarPathFinder::SetMapData(MapDataType map_data[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT])
{
	memcpy(map_data_, map_data, sizeof(map_data_));
}

std::vector<irr::core::position2di> AStarPathFinder::FindPath(irr::core::position2di start_point, irr::core::position2di end_point)
{


	return this->CreateFinalPath();
}

void AStarPathFinder::AddToOpenList(const AStarPoint &newPoint)
{
}

std::vector<AStarPathFinder::AStarPoint> AStarPathFinder::GetSurroundingPoint(const AStarPoint &curPoint)
{
	std::vector<AStarPathFinder::AStarPoint> surround_point_list;

	return surround_point_list;
}

void AStarPathFinder::CalculateGHF(AStarPoint &preAp, AStarPoint &curAp)
{
}

std::vector<irr::core::position2di> AStarPathFinder::CreateFinalPath()
{
	std::vector<irr::core::position2di> path;

	return path;
}

bool AStarPathFinder::PointInMap(const irr::core::position2di &p) const
{
	return true;
}
