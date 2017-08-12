#include "maze_runner.h"
#include "../basic_type.h"
#include <elegance/common/random.hpp>
#include <vector>
#include "scene.h"

MazeRunner::MazeRunner(Scene *scene) : SceneObject(scene)
{
	scene->GetMapData(view_map_data_);
}

MazeRunner::~MazeRunner()
{
	
}

void MazeRunner::Thinking()
{
	if (pix_posi_.X % GRID_LENGTH == 0 && pix_posi_.Y % GRID_LENGTH == 0)
	{
		std::vector<Direction> valid_dir_vec;
		if (view_map_data_[grid_posi_.X][grid_posi_.Y - 1] == MapDataType::EMPTY)
		{
			valid_dir_vec.push_back(Direction::UP);
		}
		if (view_map_data_[grid_posi_.X][grid_posi_.Y + 1] == MapDataType::EMPTY)
		{
			valid_dir_vec.push_back(Direction::DOWN);
		}
		if (view_map_data_[grid_posi_.X - 1][grid_posi_.Y] == MapDataType::EMPTY)
		{
			valid_dir_vec.push_back(Direction::LEFT);
		}
		if (view_map_data_[grid_posi_.X + 1][grid_posi_.Y] == MapDataType::EMPTY)
		{
			valid_dir_vec.push_back(Direction::RIGHT);
		}
		dir_ = valid_dir_vec[face2wind::Random::RandomNum(static_cast<int>(valid_dir_vec.size()))];
		//std::cout << "pix("<<pix_posi_.X << ","<<pix_posi_.Y<<"), ("<<grid_posi_.X<<", "<<grid_posi_.Y<<") - dir("<<int(dir_)<<")\n";
	}
}