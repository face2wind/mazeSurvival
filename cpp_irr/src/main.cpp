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

int TestMy2D()
{
	/*video::E_DRIVER_TYPE driverType = driverChoiceConsole();
	if (driverType == video::EDT_COUNT)
		driverType = video::EDT_OPENGL;*/

	IrrlichtDevice *device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(512, 384));
	if (nullptr == device)
		return 1; // could not create selected driver.

	device->setWindowCaption(L"Maze Test");

	video::IVideoDriver* driver = device->getVideoDriver();
	
	driver->getMaterial2D().TextureLayer[0].BilinearFilter = true;
	driver->getMaterial2D().AntiAliasing = video::EAAM_FULL_BASIC;

	while (device->run() && driver)
	{
		if (device->isWindowActive())
		{
			u32 time = device->getTimer()->getTime();

			driver->beginScene(true, true, video::SColor(255, 90, 80, 136));

			core::position2d<s32> m = device->getCursorControl()->getPosition();
			driver->draw2DRectangle(video::SColor(100, 255, 255, 255),
				core::rect<s32>(m.X - 20, m.Y - 20, m.X + 20, m.Y + 20));

			for (int x = 0; x < 20; x+=2)
				for (int y = 0; y < 20; y+=2)
					driver->draw2DRectangle(video::SColor(255, 255, 255, 255),
						core::rect<s32>(x*20, y*20, x*20 + 20, y*20 + 20));

			driver->draw2DPolygon(core::vector2d<int>((time/4)%400, 40), 10);

			driver->endScene();
		}
	}

	device->drop();
	return 0;
}

class GameViewThreadTask : public face2wind::IThreadTask
{
public:
	virtual void Run()
	{
		// Ïß³ÌÄÚÈÝ
	}
};

class MyNetworkHandler : public face2wind::ISerializeNetworkHandler
{
public:
	virtual void OnListenFail(Port port) {}
	virtual void OnAccept(IPAddr ip, Port port, Port local_port, NetworkID net_id) {}
	virtual void OnConnect(IPAddr ip, Port port, Port local_port, bool success, NetworkID net_id) {}

	virtual void OnRecv(NetworkID net_id, const SerializeBase *data) {}
	virtual void OnDisconnect(NetworkID net_id) {}
};

int main()
{
	TestMy2D();
	return 0;
	/*face2wind::Thread game_view_thread;
	game_view_thread.Run(new GameViewThreadTask());

	MyNetworkHandler handler;
	SerializeNetworkManager net_mgr;
	net_mgr.RegistSerializeHandler(&handler);
	net_mgr.SyncConnect("127.0.0.1", 1111);

	if (game_view_thread.IsRunning())
		game_view_thread.Join();
	return 0;*/
}