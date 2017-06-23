#include "scene.h"
#include "scene_obj.h"

#include <common/random.hpp>
#include <set>
#include <vector>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using face2wind::Random;

Scene & Scene::Instance()
{
	static Scene instance;
	return instance;
}

Scene::~Scene()
{
	
}

Scene::Scene(): device_(nullptr), driver_(nullptr)
{
}

void Scene::SetDevice(irr::IrrlichtDevice *device)
{
	if (nullptr == device) return;

	device_ = device;
	driver_ = device->getVideoDriver();

	driver_->getMaterial2D().TextureLayer[0].BilinearFilter = true;
	driver_->getMaterial2D().AntiAliasing = video::EAAM_FULL_BASIC;
}

void Scene::RestartScene()
{
	//this->CreateMapPureRandom();
	this->CreateMapPrim();
	this->CreateInitObjs();
}

int Scene::UpdateView()
{
	if (nullptr == device_ || nullptr == driver_) return -1;
	if (!device_->isWindowActive()) return -2;

	driver_->beginScene(true, true, video::SColor(0xffA8A8A8));// 255, 255, 255, 255));

	// 渲染场景
	for (int x = 0; x <= SCENE_MAP_WIDTH; ++x)
		for (int y = 0; y <= SCENE_MAP_HEIGHT; ++y)
			if (MapDataType::BLOCK == map_data_[x][y]) // 只画障碍区
			driver_->draw2DRectangle(video::SColor(0xff000000),//255, 255, 255, 255),
				core::rect<s32>(x * GRID_LENGTH, y * GRID_LENGTH, x * GRID_LENGTH + GRID_LENGTH, y * GRID_LENGTH + GRID_LENGTH));

	// 渲染场景对象
	for (SceneObject *obj : object_list_)
	{
		const position2di &pos = obj->GetPixelPosition();
		driver_->draw2DRectangle(video::SColor(obj->GetColor()),//255, 255, 255, 255),
			core::rect<s32>(pos.X, pos.Y, pos.X + GRID_LENGTH, pos.Y + GRID_LENGTH));
	}

	driver_->endScene();

	return 0;
}

int Scene::UpdateLogic(long long interval)
{
	for (auto obj : object_list_)
	{
		obj->Update(interval);

		// 移动逻辑
		{
			position2di target_posi = obj->GetGridPosition();
			Direction dir = obj->GetDir();
			switch (dir)
			{
			case Direction::UP:
				--target_posi.Y;
				break;

			case Direction::DOWN:
				++target_posi.Y;
				break;

			case Direction::LEFT:
				--target_posi.X;
				break;

			case Direction::RIGHT:
				++target_posi.X;
				break;
			}

			if (map_data_[target_posi.X][target_posi.Y] == MapDataType::EMPTY)
				obj->MoveStep();
		}
	}

	return 0;
}

void Scene::CreateMapPureRandom()
{
	// 先初始化墙
	for (int x = 0; x < SCENE_MAP_WIDTH; ++x)
		for (int y = 0; y < SCENE_MAP_HEIGHT; ++y)
			if (0 == x || (SCENE_MAP_WIDTH) == x || 0 == y || (SCENE_MAP_HEIGHT) == y)
				map_data_[x][y] = MapDataType::BLOCK;
			else if (x % 2 == 0 || y % 2 == 0)
				map_data_[x][y] = MapDataType::BLOCK;

	// 再随机拆墙
	for (int x = 0; x < SCENE_MAP_WIDTH; x += 4)
		for (int y = 0; y < SCENE_MAP_HEIGHT; y += 4)
		{
			// 内部连通
			int wall_type = Random::RandomNum(4);
			switch (wall_type)
			{
			case 0:
				map_data_[x + 1][y + 2] = MapDataType::EMPTY;
				map_data_[x + 2][y + 3] = MapDataType::EMPTY;
				map_data_[x + 3][y + 2] = MapDataType::EMPTY;
				break;
			case 1:
				map_data_[x + 2][y + 1] = MapDataType::EMPTY;
				map_data_[x + 2][y + 3] = MapDataType::EMPTY;
				map_data_[x + 3][y + 2] = MapDataType::EMPTY;
				break;
			case 2:
				map_data_[x + 2][y + 1] = MapDataType::EMPTY;
				map_data_[x + 1][y + 2] = MapDataType::EMPTY;
				map_data_[x + 3][y + 2] = MapDataType::EMPTY;
				break;
			case 3:
				map_data_[x + 2][y + 1] = MapDataType::EMPTY;
				map_data_[x + 1][y + 2] = MapDataType::EMPTY;
				map_data_[x + 2][y + 3] = MapDataType::EMPTY;
				break;
			}

			// 跟隔壁连通
			wall_type = Random::RandomNum(4);
			switch (wall_type)
			{
			case 0:
				if (y > 0)
					map_data_[x + 1][y] = MapDataType::EMPTY;
				break;
			case 1:
				if (x > 0)
					map_data_[x][y + 1] = MapDataType::EMPTY;
				break;
			case 2:
				if (x > 0)
					map_data_[x][y + 3] = MapDataType::EMPTY;
				break;
			case 3:
				if (y > 0)
					map_data_[x + 3][y] = MapDataType::EMPTY;
				break;
			}
		}
}

/*
 * Prim算法：
 * 初始V为空，所有e∈E标记为0
 * 随机选一个点e
 * 将与e相连的边的集合{Ve}并入入V，e标记为1
 * 从V中随机选一条边v
 * 判断v两端情况
 *   均为1：无动作
 *   一个0一个1：将为0的点e标记为1，绘制v,e,将e连接的边并入V
 *   均为0：不可能
 * 从V中删除v
 * 当所有e∈E均被标记为1，结束，否则返回 3. 。
 * ps:可以维护一个包含所有v∈V的标记表，防止被重复并入V，提高效率
 */
void Scene::CreateMapPrim()
{
	memset(map_data_, 0, sizeof(map_data_));

	// 先初始化墙
	for (int x = 0; x < SCENE_MAP_WIDTH; ++x)
		for (int y = 0; y < SCENE_MAP_HEIGHT; ++y)
			if (0 == x || (SCENE_MAP_WIDTH-1) == x || 0 == y || (SCENE_MAP_HEIGHT-1) == y)
				map_data_[x][y] = MapDataType::BLOCK;
			else if (x % 2 == 0 || y % 2 == 0)
				map_data_[x][y] = MapDataType::BLOCK;
	/*
	struct Point
	{
		Point(int x = 0, int y = 0) { X = x; Y = y; }
		bool operator <(const Point &other) const { return (X * SHRT_MAX + Y) < (other.X * SHRT_MAX + other.Y); }
		int X;
		int Y;
	};
	*/
	std::set<position2di> in_path; // 通路列表
	std::vector<position2di> check_wall_list; // 待检查的墙列表
	
	int cur_X = Random::RandomNum(SCENE_MAP_WIDTH/2 - 1) * 2 + 1;
	int cur_Y = Random::RandomNum(SCENE_MAP_HEIGHT/2 - 1) * 2 + 1;
	in_path.insert(position2di(cur_X, cur_Y));

	{
		position2di tmp_p(cur_X - 1, cur_Y);
		if (tmp_p.X > 0 && map_data_[tmp_p.X][tmp_p.Y] == MapDataType::BLOCK)
			check_wall_list.push_back(tmp_p);

		tmp_p.X = cur_X + 1;
		tmp_p.Y = cur_Y;
		if (tmp_p.X < SCENE_MAP_WIDTH - 1 && map_data_[tmp_p.X][tmp_p.Y] == MapDataType::BLOCK)
			check_wall_list.push_back(tmp_p);

		tmp_p.X = cur_X;
		tmp_p.Y = cur_Y - 1;
		if (tmp_p.Y > 0 && map_data_[tmp_p.X][tmp_p.Y] == MapDataType::BLOCK)
			check_wall_list.push_back(tmp_p);

		tmp_p.X = cur_X;
		tmp_p.Y = cur_Y + 1;
		if (tmp_p.Y < SCENE_MAP_HEIGHT - 1 && map_data_[tmp_p.X][tmp_p.Y] == MapDataType::BLOCK)
			check_wall_list.push_back(tmp_p);
	}

	position2di wall_p;
	position2di p1, p2;
	while (check_wall_list.size() > 0)
	{
		// 取出一个并在列表里删除此元素
		int ran_index = Random::RandomNum(static_cast<int>(check_wall_list.size()));
		wall_p = check_wall_list[ran_index];
		for (std::vector<position2di>::iterator item_it = check_wall_list.begin(); item_it != check_wall_list.end(); ++item_it)
			if (item_it->X == wall_p.X && item_it->Y == wall_p.Y)
			{
				check_wall_list.erase(item_it);
				break;
			}

		if (wall_p.X % 2 == 0) // 左右分割的墙
		{
			p1.X = wall_p.X - 1;
			p1.Y = wall_p.Y;
			p2.X = wall_p.X + 1;
			p2.Y = wall_p.Y;
		}
		else // 上下分割的墙
		{
			p1.X = wall_p.X;
			p1.Y = wall_p.Y - 1;
			p2.X = wall_p.X;
			p2.Y = wall_p.Y + 1;
		}

		position2di target_pos;
		if (in_path.find(p1) == in_path.end())
		{
			in_path.insert(p1);
			target_pos = p1;
		}

		if (in_path.find(p2) == in_path.end())
		{
			in_path.insert(p2);
			target_pos = p2;
		}

		if (target_pos.X > 0 && target_pos.Y > 0)
		{
			map_data_[wall_p.X][wall_p.Y] = MapDataType::EMPTY;

			position2di tmp_p(target_pos.X - 1, target_pos.Y);
			if (tmp_p.X > 0 && map_data_[tmp_p.X][tmp_p.Y] == MapDataType::BLOCK)
			{
				check_wall_list.push_back(tmp_p);
			}

			tmp_p.X = target_pos.X + 1;
			tmp_p.Y = target_pos.Y;
			if (tmp_p.X < SCENE_MAP_WIDTH - 1 && map_data_[tmp_p.X][tmp_p.Y] == MapDataType::BLOCK)
			{
				check_wall_list.push_back(tmp_p);
			}

			tmp_p.X = target_pos.X;
			tmp_p.Y = target_pos.Y - 1;
			if (tmp_p.Y > 0 && map_data_[tmp_p.X][tmp_p.Y] == MapDataType::BLOCK)
			{
				check_wall_list.push_back(tmp_p);
			}

			tmp_p.X = target_pos.X;
			tmp_p.Y = target_pos.Y + 1;
			if (tmp_p.Y < SCENE_MAP_HEIGHT - 1 && map_data_[tmp_p.X][tmp_p.Y] == MapDataType::BLOCK)
			{
				check_wall_list.push_back(tmp_p);
			}
		}
	}
}

void Scene::CreateInitObjs()
{
	SceneObject *obj = SceneObject::CreateObject(this, ObjectType::RUNNER);
	obj->SetGridPosition(position2di(1, 1));
	object_list_.push_back(obj);

	obj = SceneObject::CreateObject(this, ObjectType::RUNNER);
	obj->SetGridPosition(position2di(11, 11));
	object_list_.push_back(obj);
}
