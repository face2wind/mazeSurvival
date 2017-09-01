#include <elegance/common/random.hpp>
#include <vector>

#include "basic_type.h"
#include "scene.h"
#include "explorer.h"

Explorer::Explorer(Scene *scene) : SceneObject(scene), exploring_(false)
{
	// Ĭ�ϵ���������ͼ���ǲ���������
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
	// ��ǰû�п��ڰ������֮�䣬������˼���������Լ��ľ�ȷ���겻�����߼����꣬������ƶ������������򲿵����
	if (pix_posi_.X % GRID_LENGTH != 0 || pix_posi_.Y % GRID_LENGTH != 0)
	{
		return;
	}

	// ��̽������ȡ����Ѱ·
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
		if (cur_moving_path_.empty()) // û�п���·���ˣ�ֹͣ
		{
			action_state_ = ActionState::IDLE;
			return;
		}
		else // ��·�����ҵ���һ������
		{
			next_move_point_ = cur_moving_path_.back();
			cur_moving_path_.pop_back();
		}
	}

	if (next_move_point_.X != grid_posi_.X && next_move_point_.Y != grid_posi_.Y) // �������������ƫ�ƣ���׼�ƶ�����������б�ߣ�
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
