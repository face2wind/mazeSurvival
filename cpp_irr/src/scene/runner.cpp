
#include "basic_type.h"
#include "scene.h"
#include "runner.h"

Runner::Runner(Scene *scene) : Explorer(scene)
{
	scene->GetMapData(view_map_data_);

	path_finder_ = IPathFinder::CreatePathFinder(PathFinderType::Astar);
	path_finder_->SetMapData(view_map_data_);
}

Runner::~Runner()
{
	
}

void Runner::Thinking(long long interval)
{
	Explorer::Thinking(interval);

	if (pix_posi_.X % GRID_LENGTH == 0 && pix_posi_.Y % GRID_LENGTH == 0) // 当前没有卡在半个格子之间
	{
	}
}