#include "i_path_finder.h"
#include "a_star_path_finder.h"

IPathFinder * IPathFinder::CreatePathFinder(PathFinderType type)
{
	IPathFinder *finder = nullptr;

	switch (type)
	{
	case PathFinderType::Astar:
		{
			finder = new AStarPathFinder();
		}
		break;
	}

	return finder;
}
