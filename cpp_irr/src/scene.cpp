#include "scene.h"
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
	this->CreateNewMap();
}

int Scene::UpdateView()
{
	if (nullptr == device_ || nullptr == driver_) return -1;
	if (!device_->isWindowActive()) return -2;

	driver_->beginScene(true, true, video::SColor(0xffA8A8A8));// 255, 255, 255, 255));

	for (int x = 0; x <= SCENE_MAP_WIDTH; ++x)
		for (int y = 0; y <= SCENE_MAP_HEIGHT; ++y)
			if (MapDataType::BLOCK == map_data_[x][y]) // 只画障碍区
			driver_->draw2DRectangle(video::SColor(0xff000000),//255, 255, 255, 255),
				core::rect<s32>(x * GRID_LENGTH, y * GRID_LENGTH, x * GRID_LENGTH + GRID_LENGTH, y * GRID_LENGTH + GRID_LENGTH));

	driver_->endScene();

	return 0;
}

int Scene::UpdateLogic()
{
	return 0;
}

void Scene::CreateMapPureRandom()
{
	// 先初始化墙
	for (int x = 0; x <= SCENE_MAP_WIDTH; ++x)
		for (int y = 0; y <= SCENE_MAP_HEIGHT; ++y)
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
	for (int x = 0; x <= SCENE_MAP_WIDTH; ++x)
		for (int y = 0; y <= SCENE_MAP_HEIGHT; ++y)
			if (0 == x || (SCENE_MAP_WIDTH) == x || 0 == y || (SCENE_MAP_HEIGHT) == y)
				map_data_[x][y] = MapDataType::BLOCK;
			else if (x % 2 == 0 || y % 2 == 0)
				map_data_[x][y] = MapDataType::BLOCK;
	
	struct Point
	{
		Point(int x = 0, int y = 0) { x_pos = x; y_pos = y; }
		bool operator <(const Point &other) const { return (x_pos * SHRT_MAX + y_pos) < (other.x_pos * SHRT_MAX + other.y_pos); }
		int x_pos;
		int y_pos;
	};

	std::set<Point> in_path; // 通路列表
	std::vector<Point> check_wall_list; // 待检查的墙列表
	
	int cur_x_pos = Random::RandomNum(SCENE_MAP_WIDTH/2 - 1) * 2 + 1;
	int cur_y_pos = Random::RandomNum(SCENE_MAP_HEIGHT/2 - 1) * 2 + 1;
	in_path.insert(Point(cur_x_pos, cur_y_pos));

	{
		Point tmp_p(cur_x_pos - 1, cur_y_pos);
		if (tmp_p.x_pos > 0 && map_data_[tmp_p.x_pos][tmp_p.y_pos] == MapDataType::BLOCK)
			check_wall_list.push_back(tmp_p);

		tmp_p.x_pos = cur_x_pos + 1;
		tmp_p.y_pos = cur_y_pos;
		if (tmp_p.x_pos < SCENE_MAP_WIDTH - 1 && map_data_[tmp_p.x_pos][tmp_p.y_pos] == MapDataType::BLOCK)
			check_wall_list.push_back(tmp_p);

		tmp_p.x_pos = cur_x_pos;
		tmp_p.y_pos = cur_y_pos - 1;
		if (tmp_p.y_pos > 0 && map_data_[tmp_p.x_pos][tmp_p.y_pos] == MapDataType::BLOCK)
			check_wall_list.push_back(tmp_p);

		tmp_p.x_pos = cur_x_pos;
		tmp_p.y_pos = cur_y_pos + 1;
		if (tmp_p.y_pos < SCENE_MAP_HEIGHT - 1 && map_data_[tmp_p.x_pos][tmp_p.y_pos] == MapDataType::BLOCK)
			check_wall_list.push_back(tmp_p);
	}

	Point wall_p;
	Point p1, p2;
	while (check_wall_list.size() > 0)
	{
		// 取出一个并在列表里删除此元素
		int ran_index = Random::RandomNum(static_cast<int>(check_wall_list.size()));
		wall_p = check_wall_list[ran_index];
		for (std::vector<Point>::iterator item_it = check_wall_list.begin(); item_it != check_wall_list.end(); ++item_it)
			if (item_it->x_pos == wall_p.x_pos && item_it->y_pos == wall_p.y_pos)
			{
				check_wall_list.erase(item_it);
				break;
			}

		if (wall_p.x_pos % 2 == 0) // 左右分割的墙
		{
			p1.x_pos = wall_p.x_pos - 1;
			p1.y_pos = wall_p.y_pos;
			p2.x_pos = wall_p.x_pos + 1;
			p2.y_pos = wall_p.y_pos;
		}
		else // 上下分割的墙
		{
			p1.x_pos = wall_p.x_pos;
			p1.y_pos = wall_p.y_pos - 1;
			p2.x_pos = wall_p.x_pos;
			p2.y_pos = wall_p.y_pos + 1;
		}

		Point target_pos;
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

		if (target_pos.x_pos > 0 && target_pos.y_pos > 0)
		{
			map_data_[wall_p.x_pos][wall_p.y_pos] = MapDataType::EMPTY;

			Point tmp_p(target_pos.x_pos - 1, target_pos.y_pos);
			if (tmp_p.x_pos > 0 && map_data_[tmp_p.x_pos][tmp_p.y_pos] == MapDataType::BLOCK)
			{
				check_wall_list.push_back(tmp_p);
			}

			tmp_p.x_pos = target_pos.x_pos + 1;
			tmp_p.y_pos = target_pos.y_pos;
			if (tmp_p.x_pos < SCENE_MAP_WIDTH - 1 && map_data_[tmp_p.x_pos][tmp_p.y_pos] == MapDataType::BLOCK)
			{
				check_wall_list.push_back(tmp_p);
			}

			tmp_p.x_pos = target_pos.x_pos;
			tmp_p.y_pos = target_pos.y_pos - 1;
			if (tmp_p.y_pos > 0 && map_data_[tmp_p.x_pos][tmp_p.y_pos] == MapDataType::BLOCK)
			{
				check_wall_list.push_back(tmp_p);
			}

			tmp_p.x_pos = target_pos.x_pos;
			tmp_p.y_pos = target_pos.y_pos + 1;
			if (tmp_p.y_pos < SCENE_MAP_HEIGHT - 1 && map_data_[tmp_p.x_pos][tmp_p.y_pos] == MapDataType::BLOCK)
			{
				check_wall_list.push_back(tmp_p);
			}
		}
	}
}

void Scene::CreateNewMap()
{
	//this->CreateMapPureRandom();
	this->CreateMapPrim();
}