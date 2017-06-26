#include "scene/scene.h"

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#include <elegance.hpp>
using namespace face2wind;

#include "driverChoice.h"
#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "elegance.lib")
#endif

bool game_running = true; // 游戏运行开关（协调两个线程）

class GameLogicThreadTask : public face2wind::IThreadTask
{
public:
	virtual void Run()
	{
		Scene &scene = Scene::Instance();
		scene.RestartScene(); // 创建地图也属于数据逻辑，所以放到逻辑线程，不阻塞显示线程
		long long interval = 0;

		// 线程内容
		while (game_running)
		{
			int ret = scene.UpdateLogic(interval);
			if (0 != ret)
			{
				std::cout << "Scene::UpdateLogic() return " << ret << ", exit thread" << std::endl;
				break;
			}

			Timer::Sleep(10);
			++interval;
			if (interval < 0)
				interval = 0;
		}

		game_running = false;
	}
};

int main()
{
    video::E_DRIVER_TYPE driverType=driverChoiceConsole();                                                                                                             
         if (driverType==video::EDT_COUNT)                                                                                                                                  
                 return 1; 
	IrrlichtDevice *device = createDevice(driverType, core::dimension2d<u32>(SCENE_MAP_WIDTH*GRID_LENGTH, SCENE_MAP_HEIGHT*GRID_LENGTH));
	if (nullptr == device)
		return 1;
	device->setWindowCaption(L"Maze Survival");

	face2wind::Thread game_view_thread;
	game_view_thread.Run(new GameLogicThreadTask());
	
	Scene &scene = Scene::Instance();

	scene.SetDevice(device);
	while (game_running && device->run())
	{
		scene.UpdateView();
	}

	game_running = false;
	device->drop();

	game_view_thread.Join();
	return 0;
}
