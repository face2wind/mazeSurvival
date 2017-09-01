#pragma once
#include "scene_obj.h"
#include "basic_type.h"
#include "path_finder/a_star_path_finder.h"

/*
 * ̽���ߣ�����̽��·�ߣ�ֱ���ҵ�����
 */
class Explorer : public SceneObject
{
public:
	Explorer(Scene *scene);
	~Explorer();

	virtual void Thinking(long long interval);

protected:
	void MovingWithPath();
	void StartMove();
	void StopMove();

	void StartExplore();
	void StopExplore();

	void ReInitExploreData();							// ����һ���Լ�����Ұ��ͼ�����¸������е�̽������
	void UpdateVision();								// �ӳ������ȡ��ǰ����Ұ�����µ��Լ�����Ұ��ͼ������ı�̽������
	
protected:
	//MapDataType view_map_data_[SCENE_MAP_WIDTH][SCENE_MAP_HEIGHT];
	IPathFinder *path_finder_;

	std::vector<Point2d> cur_moving_path_;
	Point2d next_move_point_;

	bool exploring_;									// �Ƿ�����̽��ģʽ
	std::vector<Point2d> explorer_point_list_;			// ̽����
};
