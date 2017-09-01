#include <elegance/common/random.hpp>
#include <vector>

#include "basic_type.h"
#include "scene.h"
#include "explorer.h"

Explorer::Explorer(Scene *scene) : SceneObject(scene), exploring_(false)
{
	// 默认当作整个地图都是不可走区域
	for (int x = 0; x < SCENE_MAP_WIDTH; ++x)
		for (int y = 0; y < SCENE_MAP_HEIGHT; ++y)
			scene_view_.map_data_[x][y] = MapDataType::BLOCK;

	path_finder_ = IPathFinder::CreatePathFinder(PathFinderType::Astar);
}

Explorer::~Explorer()
{
	
}

void Explorer::Thinking(long long interval)
{
	// 当前没有卡在半个格子之间，才允许思考，否则自己的精确坐标不等于逻辑坐标，会出现移动到不可走区域部的情况
	if (pix_posi_.X % GRID_LENGTH != 0 || pix_posi_.Y % GRID_LENGTH != 0)
	{
		return;
	}

	// 从探索点里取出来寻路
	if (ActionState::IDLE == action_state_ && !explorer_point_list_.empty())
	{
		Point2d target_point = explorer_point_list_.back();
		explorer_point_list_.pop_back();

		//cur_moving_path_ = path_finder_->FindPath(grid_posi_, scene_->GetRandomMovingPoint2d());
		cur_moving_path_ = path_finder_->FindPath(grid_posi_, target_point);
		if (cur_moving_path_.size() > 0)
		{
			this->StartMove();
		}
	}

	this->MovingWithPath();
	this->UpdateVision();
}

void Explorer::MovingWithPath()
{
	if (next_move_point_ == grid_posi_)
	{
		if (cur_moving_path_.empty()) // 没有可走路径了，停止
		{
			action_state_ = ActionState::IDLE;
			return;
		}
		else // 从路径里找到下一个方向
		{
			next_move_point_ = cur_moving_path_.back();
			cur_moving_path_.pop_back();
		}
	}

	if (next_move_point_.X != grid_posi_.X && next_move_point_.Y != grid_posi_.Y) // 如果两个方向都有偏移，不准移动（不可能走斜线）
		return;

	if (next_move_point_.X > grid_posi_.X)
		dir_ = Direction::RIGHT;
	else if (next_move_point_.X < grid_posi_.X)
		dir_ = Direction::LEFT;
	else if (next_move_point_.Y > grid_posi_.Y)
		dir_ = Direction::DOWN;
	else if (next_move_point_.Y < grid_posi_.Y)
		dir_ = Direction::UP;
}

void Explorer::StartMove()
{
	next_move_point_ = grid_posi_;
	action_state_ = ActionState::MOVING;
}

void Explorer::StopMove()
{
	next_move_point_ = grid_posi_;
	action_state_ = ActionState::IDLE;
}

void Explorer::StartExplore()
{
	if (exploring_)
		return;

	exploring_ = true;

	this->ReInitExploreData();
}

void Explorer::StopExplore()
{
	if (!exploring_)
		return;

	exploring_ = false;

	explorer_point_list_.clear();
}

void Explorer::ReInitExploreData()
{
	//explorer_point_list_.push_back(scene_->GetRandomMovingPoint2d());
}

void Explorer::UpdateVision()
{
	std::vector<Point2d> new_explore_point_list = scene_->UpdateVision(grid_posi_, scene_view_);
	for (Point2d point : new_explore_point_list)
	{
		//explorer_point_list_.push_back(scene_->GetRandomMovingPoint2d());
		explorer_point_list_.push_back(point);
	}

	path_finder_->SetMapData(scene_view_.map_data_);
}
