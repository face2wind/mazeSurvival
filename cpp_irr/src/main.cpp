#include "scene.h"

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

class GameViewThreadTask : public face2wind::IThreadTask
{
public:
	virtual void Run()
	{
		Scene &scene = Scene::Instance();

		scene.RestartScene();

		// 线程内容
		while (game_running)
		{
			int ret = scene.UpdateLogic();
			if (0 != ret)
			{
				std::cout << "Scene::UpdateLogic() return " << ret << ", exit thread" << std::endl;
				break;
			}
		}

		game_running = false;
	}
};
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 92
bool R[N][N], D[N][N], v[N][N];
int m, n;
void dfs(int r, int c)
{
	int d = rand() % 4, dddddd = rand() % 2 ? 1 : 3;
	v[r][c] = true;
	for (int i = 0; i < 4; i++) {
		int data1[4] = { -1, 0, 1, 0 };
		int data2[4] = { 0, -1, 0, 1 };
		int rr = r + data1[d],
			cc = c + data2[d];
		if ((unsigned)rr < m && (unsigned)cc < n && !v[rr][cc]) {
			if (d % 2)
				R[r][c - (d == 1)] = true;
			else
				D[r - (d == 0)][c] = true;
			dfs(rr, cc);
		}
		d = (d + dddddd) % 4;
	}
}
int maintest()
{
	m = 10;
	n = 10;
	dfs(0, 0);
	for (int c = 0; c < n; c++)
		printf("._");
	printf(".\n");
	for (int r = 0; r < m; r++) {
		printf("|");
		for (int c = 0; c < n; c++) {
			putchar(D[r][c] ? ' ' : '_');
			putchar(R[r][c] ? '.' : '|');
		}
		printf("\n");
	}
	return 0;
}
int main()
{
	IrrlichtDevice *device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(SCENE_MAP_WIDTH*GRID_LENGTH, SCENE_MAP_HEIGHT*GRID_LENGTH));
	if (nullptr == device)
		return 1; // could not create selected driver.
	device->setWindowCaption(L"Maze Survival");

	face2wind::Thread game_view_thread;
	game_view_thread.Run(new GameViewThreadTask());
	
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
	/*
	MyNetworkHandler handler;
	SerializeNetworkManager net_mgr;
	net_mgr.RegistSerializeHandler(&handler);
	net_mgr.SyncConnect("127.0.0.1", 1111);

	if (game_view_thread.IsRunning())
		game_view_thread.Join();
	return 0;*/
}