#include <elegance/common/random.hpp>
#include <vector>

#include "basic_type.h"
#include "scene.h"
#include "explorer.h"

Explorer::Explorer(Scene *scene) : SceneObject(scene)
{
	scene->GetMapData(view_map_data_);

	path_finder_ = IPathFinder::CreatePathFinder(PathFinderType::Astar);
	path_finder_->SetMapData(view_map_data_);
}

Explorer::~Explorer()
{
	
}

void Explorer::Thinking(long long interval)
{
	if (ActionState::IDLE == action_state_ && cur_moving_path_.empty() && interval % 100 == 0)
	{
		cur_moving_path_ = path_finder_->FindPath(grid_posi_, scene_->GetRandomMovingPoint2d());
		if (cur_moving_path_.size() > 0)
		{
			cur_moving_path_.pop_back(); // �����ɾ��
		}

		if (cur_moving_path_.size() > 0)
		{
			next_move_point_ = grid_posi_;
			action_state_ = ActionState::MOVING;
		}
	}

	if (pix_posi_.X % GRID_LENGTH == 0 && pix_posi_.Y % GRID_LENGTH == 0) // ��ǰû�п��ڰ������֮��
	{
		// ����趨����
		if (false)
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

		// ��·�����ҵ���һ������
		if (next_move_point_ == grid_posi_)
		{
			if (cur_moving_path_.empty())
			{
				action_state_ = ActionState::IDLE;
			}
			else
			{
				next_move_point_ = cur_moving_path_.back();
				cur_moving_path_.pop_back();

				if (next_move_point_.X != grid_posi_.X && next_move_point_.Y != grid_posi_.Y) // �������������ƫ�ƣ��޷��ƶ�����������б�ߣ�
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
		}
	}
}
