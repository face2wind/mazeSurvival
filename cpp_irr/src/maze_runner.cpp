#include "maze_runner.h"
#include "basic_type.h"
#include <common/random.hpp>

MazeRunner::MazeRunner(Scene *scene) : SceneObject(scene)
{
}

MazeRunner::~MazeRunner()
{
	
}

void MazeRunner::Thinking()
{
	if (pix_posi_.X % GRID_LENGTH == 0)
	{
		dir_ = static_cast<Direction>(face2wind::Random::RandomNum(static_cast<int>(Direction::COUNT)));
	}
}