#include "scene.h"
#include "scene_obj.h"

#include <elegance/common/random.hpp>
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

Scene::Scene(): device_(nullptr), driver_(nullptr), showing_scene_view_(&scene_view_), cur_max_object_id_(0)
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

	s32 fps = driver_->getFPS();
	core::stringw str = L"Maze Survival";
	str += " FPS:";
	str += fps;

	device_->setWindowCaption(str.c_str());
	driver_->beginScene(true, true, video::SColor(0xffA8A8A8));// 255, 255, 255, 255));

	if (nullptr != showing_scene_view_)
		showing_scene_view_->UpdateView(driver_);

	driver_->endScene();

	return 0;
}

int Scene::UpdateLogic(long long interval)
{
	for (auto obj : object_list_)
	{
		obj->Update(interval);

		// 移动逻辑
		if (ActionState::MOVING == obj->GetActionState())
		{
			Point2d cur_grid_posi = obj->GetGridPosition();
			Point2d starget_pix_posi = obj->GetPixelPosition();

			//（测试当前方向是否移动完毕，比如上下移动的过程中肯定不允许左右移动）
			bool allow_up_down = false; // 是否可以上下移动
			bool allow_left_right = false; // 是否可以左右移动
			{
				if (starget_pix_posi.X == (cur_grid_posi.X * GRID_LENGTH))
				{
					allow_up_down = true;
				}
				if (starget_pix_posi.Y == (cur_grid_posi.Y * GRID_LENGTH))
				{
					allow_left_right = true;
				}
			}
			Direction dir = obj->GetDir();
			if (allow_up_down)
			{
				if (Direction::UP == dir)
					--starget_pix_posi.Y;
				else if (Direction::DOWN == dir)
					++starget_pix_posi.Y;
			}
			if (allow_left_right)
			{
				if (Direction::LEFT == dir)
					--starget_pix_posi.X;
				else if (Direction::RIGHT == dir)
					++starget_pix_posi.X;
			}

			Point2d target_pos1, target_pos2;
			target_pos1.X = starget_pix_posi.X;
			target_pos1.Y = starget_pix_posi.Y;
			target_pos2.X = target_pos1.X + GRID_LENGTH - 1;
			target_pos2.Y = target_pos1.Y + GRID_LENGTH - 1;

			target_pos1.X /= GRID_LENGTH;
			target_pos1.Y /= GRID_LENGTH;
			target_pos2.X /= GRID_LENGTH;
			target_pos2.Y /= GRID_LENGTH;

			// 检测目格子四个点所在的格子都可走
			if (MapDataCanMove(scene_view_.map_data_[target_pos1.X][target_pos1.Y]) && MapDataCanMove(scene_view_.map_data_[target_pos1.X][target_pos2.Y]) &&
				MapDataCanMove(scene_view_.map_data_[target_pos2.X][target_pos1.Y]) && MapDataCanMove(scene_view_.map_data_[target_pos2.X][target_pos2.Y]))
				obj->SetPixPosition(starget_pix_posi);

			obj->UpdateSceneViewSelfObject();
		}

		SceneViewObj &sv_obj = scene_view_.object_map_[obj->GetID()];
		sv_obj.pix_posi = obj->GetPixelPosition();
		sv_obj.grid_posi = obj->GetGridPosition();
	}

	return 0;
}

void Scene::GetMapData(MapDataType other_map_data[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT])
{
	memcpy(other_map_data, scene_view_.map_data_, sizeof(scene_view_.map_data_));
}

#define CHECK_SIDE_POS(XPOS, YPOS) \
{\
	MapDataType &real_type = scene_view_.map_data_[(XPOS)][(YPOS)];\
	MapDataType &target_type = target_view.map_data_[(XPOS)][(YPOS)];\
	if (MapDataType::EXPLORE != target_type && target_type != real_type)\
	{\
		if (MapDataType::EMPTY == real_type)\
		{\
			explore_point_list.push_back(Point2d((XPOS), (YPOS)));\
			target_type = MapDataType::EXPLORE;\
		}\
		else\
		{\
			target_type = real_type; \
		}\
	}\
}

std::vector<Point2d> Scene::UpdateVision(Point2d &point, SceneView &target_view)
{
	std::vector<Point2d> explore_point_list;
	if (point.X < 0 || point.X >= SCENE_MAP_WIDTH ||
		point.Y < 0 || point.Y >= SCENE_MAP_HEIGHT)
		return explore_point_list;

	// 当前坐标先同步一下
	target_view.map_data_[point.X][point.Y] = scene_view_.map_data_[point.X][point.Y];

	// 向右查看视野
	for (int xpos = point.X + 1; xpos < SCENE_MAP_WIDTH; ++xpos)
	{
		target_view.map_data_[xpos][point.Y] = scene_view_.map_data_[xpos][point.Y];

		if (!MapDataCanMove(target_view.map_data_[xpos][point.Y])) // 遇到不可走区域，终止往前搜索
			break;

		// 两边格子也要检测，因为视野上是能看到的
		CHECK_SIDE_POS(xpos, point.Y - 1);
		CHECK_SIDE_POS(xpos, point.Y + 1);
	}

	// 向左查看视野
	for (int xpos = point.X - 1; xpos >= 0; --xpos)
	{
		target_view.map_data_[xpos][point.Y] = scene_view_.map_data_[xpos][point.Y];

		if (!MapDataCanMove(target_view.map_data_[xpos][point.Y])) // 遇到墙，终止往前搜索
			break;

		// 两边格子也要检测，因为视野上是能看到的
		CHECK_SIDE_POS(xpos, point.Y - 1);
		CHECK_SIDE_POS(xpos, point.Y + 1);
	}

	// 向下查看视野
	for (int ypos = point.Y + 1; ypos < SCENE_MAP_HEIGHT; ++ypos)
	{
		target_view.map_data_[point.X][ypos] = scene_view_.map_data_[point.Y][ypos];

		if (!MapDataCanMove(target_view.map_data_[point.X][ypos])) // 遇到墙，终止往前搜索
			break;

		// 两边格子也要检测，因为视野上是能看到的
		CHECK_SIDE_POS(point.X - 1, ypos);
		CHECK_SIDE_POS(point.X + 1, ypos);
	}

	// 向上查看视野
	for (int ypos = point.Y - 1; ypos >= 0; --ypos)
	{
		target_view.map_data_[point.X][ypos] = scene_view_.map_data_[point.Y][ypos];

		if (!MapDataCanMove(target_view.map_data_[point.X][ypos])) // 遇到墙，终止往前搜索
			break;

		// 两边格子也要检测，因为视野上是能看到的
		CHECK_SIDE_POS(point.X - 1, ypos);
		CHECK_SIDE_POS(point.X + 1, ypos);
	}

	return explore_point_list;
}

Point2d Scene::GetRandomMovingPoint2d() const
{
	int random_x = Random::RandomNum(SCENE_MAP_WIDTH / 2 - 1) * 2 + 1;
	int random_y = Random::RandomNum(SCENE_MAP_HEIGHT / 2 - 1) * 2 + 1;
	return Point2d(random_x, random_y);
}

void Scene::CreateMapPureRandom()
{
	// 先初始化墙
	for (int x = 0; x < SCENE_MAP_WIDTH; ++x)
		for (int y = 0; y < SCENE_MAP_HEIGHT; ++y)
			if (0 == x || (SCENE_MAP_WIDTH) == x || 0 == y || (SCENE_MAP_HEIGHT) == y)
				scene_view_.map_data_[x][y] = MapDataType::BLOCK;
			else if (x % 2 == 0 || y % 2 == 0)
				scene_view_.map_data_[x][y] = MapDataType::BLOCK;

	// 再随机拆墙
	for (int x = 0; x < SCENE_MAP_WIDTH; x += 4)
		for (int y = 0; y < SCENE_MAP_HEIGHT; y += 4)
		{
			// 内部连通
			int wall_type = Random::RandomNum(4);
			switch (wall_type)
			{
			case 0:
				scene_view_.map_data_[x + 1][y + 2] = MapDataType::EMPTY;
				scene_view_.map_data_[x + 2][y + 3] = MapDataType::EMPTY;
				scene_view_.map_data_[x + 3][y + 2] = MapDataType::EMPTY;
				break;
			case 1:
				scene_view_.map_data_[x + 2][y + 1] = MapDataType::EMPTY;
				scene_view_.map_data_[x + 2][y + 3] = MapDataType::EMPTY;
				scene_view_.map_data_[x + 3][y + 2] = MapDataType::EMPTY;
				break;
			case 2:
				scene_view_.map_data_[x + 2][y + 1] = MapDataType::EMPTY;
				scene_view_.map_data_[x + 1][y + 2] = MapDataType::EMPTY;
				scene_view_.map_data_[x + 3][y + 2] = MapDataType::EMPTY;
				break;
			case 3:
				scene_view_.map_data_[x + 2][y + 1] = MapDataType::EMPTY;
				scene_view_.map_data_[x + 1][y + 2] = MapDataType::EMPTY;
				scene_view_.map_data_[x + 2][y + 3] = MapDataType::EMPTY;
				break;
			}

			// 跟隔壁连通
			wall_type = Random::RandomNum(4);
			switch (wall_type)
			{
			case 0:
				if (y > 0)
					scene_view_.map_data_[x + 1][y] = MapDataType::EMPTY;
				break;
			case 1:
				if (x > 0)
					scene_view_.map_data_[x][y + 1] = MapDataType::EMPTY;
				break;
			case 2:
				if (x > 0)
					scene_view_.map_data_[x][y + 3] = MapDataType::EMPTY;
				break;
			case 3:
				if (y > 0)
					scene_view_.map_data_[x + 3][y] = MapDataType::EMPTY;
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
	memset(scene_view_.map_data_, 0, sizeof(scene_view_.map_data_));

	// 先初始化墙
	for (int x = 0; x < SCENE_MAP_WIDTH; ++x)
		for (int y = 0; y < SCENE_MAP_HEIGHT; ++y)
			if (0 == x || (SCENE_MAP_WIDTH - 1) == x || 0 == y || (SCENE_MAP_HEIGHT - 1) == y)
				scene_view_.map_data_[x][y] = MapDataType::BLOCK;
			else if (x % 2 == 0 || y % 2 == 0)
				scene_view_.map_data_[x][y] = MapDataType::BLOCK;

	std::set<Point2d> in_path; // 通路列表
	std::vector<Point2d> check_wall_list; // 待检查的墙列表

	int cur_X = Random::RandomNum(SCENE_MAP_WIDTH / 2 - 1) * 2 + 1;
	int cur_Y = Random::RandomNum(SCENE_MAP_HEIGHT / 2 - 1) * 2 + 1;
	in_path.insert(Point2d(cur_X, cur_Y));

	{
		Point2d tmp_p(cur_X - 1, cur_Y);
		if (tmp_p.X > 0 && !MapDataCanMove(scene_view_.map_data_[tmp_p.X][tmp_p.Y]))
			check_wall_list.push_back(tmp_p);

		tmp_p.X = cur_X + 1;
		tmp_p.Y = cur_Y;
		if (tmp_p.X < SCENE_MAP_WIDTH - 1 && !MapDataCanMove(scene_view_.map_data_[tmp_p.X][tmp_p.Y]))
			check_wall_list.push_back(tmp_p);

		tmp_p.X = cur_X;
		tmp_p.Y = cur_Y - 1;
		if (tmp_p.Y > 0 && !MapDataCanMove(scene_view_.map_data_[tmp_p.X][tmp_p.Y]))
			check_wall_list.push_back(tmp_p);

		tmp_p.X = cur_X;
		tmp_p.Y = cur_Y + 1;
		if (tmp_p.Y < SCENE_MAP_HEIGHT - 1 && !MapDataCanMove(scene_view_.map_data_[tmp_p.X][tmp_p.Y]))
			check_wall_list.push_back(tmp_p);
	}

	Point2d wall_p;
	Point2d p1, p2;
	while (check_wall_list.size() > 0)
	{
		// 取出一个并在列表里删除此元素
		int ran_index = Random::RandomNum(static_cast<int>(check_wall_list.size()));
		wall_p = check_wall_list[ran_index];
		for (std::vector<Point2d>::iterator item_it = check_wall_list.begin(); item_it != check_wall_list.end(); ++item_it)
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

		Point2d target_pos;
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
			scene_view_.map_data_[wall_p.X][wall_p.Y] = MapDataType::EMPTY;

			Point2d tmp_p(target_pos.X - 1, target_pos.Y);
			if (tmp_p.X > 0 && !MapDataCanMove(scene_view_.map_data_[tmp_p.X][tmp_p.Y]))
			{
				check_wall_list.push_back(tmp_p);
			}

			tmp_p.X = target_pos.X + 1;
			tmp_p.Y = target_pos.Y;
			if (tmp_p.X < SCENE_MAP_WIDTH - 1 && !MapDataCanMove(scene_view_.map_data_[tmp_p.X][tmp_p.Y]))
			{
				check_wall_list.push_back(tmp_p);
			}

			tmp_p.X = target_pos.X;
			tmp_p.Y = target_pos.Y - 1;
			if (tmp_p.Y > 0 && !MapDataCanMove(scene_view_.map_data_[tmp_p.X][tmp_p.Y]))
			{
				check_wall_list.push_back(tmp_p);
			}

			tmp_p.X = target_pos.X;
			tmp_p.Y = target_pos.Y + 1;
			if (tmp_p.Y < SCENE_MAP_HEIGHT - 1 && !MapDataCanMove(scene_view_.map_data_[tmp_p.X][tmp_p.Y]))
			{
				check_wall_list.push_back(tmp_p);
			}
		}
	}
}

int Scene::GetNewObjID()
{
	if (!recycle_object_id_list_.empty())
	{
		ObjectID free_obj_id = recycle_object_id_list_.top();
		recycle_object_id_list_.pop();
		return free_obj_id;
	}

	return ++cur_max_object_id_;
}

void Scene::CreateInitObjs()
{
	SceneObject *obj = SceneObject::CreateObject(this, ObjectType::RUNNER);
	obj->SetGridPosition(this->GetRandomMovingPoint2d());
	obj->SetID(this->GetNewObjID());
	object_list_.push_back(obj);

	SceneViewObj &sv_obj = scene_view_.object_map_[obj->GetID()];
	sv_obj.object_id = obj->GetID();
	sv_obj.color = obj->GetColor();
	sv_obj.grid_posi = obj->GetGridPosition();
	sv_obj.pix_posi = obj->GetPixelPosition();

	// 选择对象视角来展示
	showing_scene_view_ = obj->GetSceneView();
	// 删除obj的时候记得检查showing_scene_view_
}
