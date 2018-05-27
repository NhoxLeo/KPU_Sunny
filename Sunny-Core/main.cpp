#pragma setlocale("kor")

#include <iostream>

#include <SUNNY.h>


#include "06_LAYER/FPSLayer2D.h"
#include "06_LAYER/MouseLayer2D.h"
#include "06_LAYER/LoadingLayer2D.h"

#include "06_LAYER/RoomLayer2D.h"
#include "06_LAYER/TestLayer2D.h"
#include "06_LAYER/TestLayer3D.h"


class Game : public Application
{
public:
	Game() : Application("sunny", { 1600, 900, false, true })
	{}

	~Game()
	{}

	void Init() override
	{
		Application::Init();

		ShaderFactory::Default3DForwardShader();
		ShaderFactory::Default2DShader();

		VFS::Get()->Mount("HLSL",    "02_HLSL");
		VFS::Get()->Mount("JSON",    "03_JSON");
		VFS::Get()->Mount("CUBE",    "04_ASSET/CUBE");
		VFS::Get()->Mount("OBJ",     "04_ASSET/OBJ");
		VFS::Get()->Mount("SUN",     "04_ASSET/SUN");
		VFS::Get()->Mount("TEXTURE", "04_ASSET/TEXTURE");
		VFS::Get()->Mount("RAW",     "04_ASSET/RAW");
		
		game::AssetLoader::LoadTexture("/JSON/TEXTURE/Rooms.json");

		Input::GetInputManager()->SetMouseCursor(SUNNY_NO_CURSOR);

		PushOverlay(new FPSLayer2D());
		PushOverlay(new MouseLayer2D());
		FPSLayer2D::SetColor(RGBA(0.f, 0.f, 0.f, 0.7f));
		PushLayer2D(new LoadingLayer2D());
	}
};


int main()
{
	std::cout << "Hello, Sunny Project!" << std::endl;

	Game game;

	game.Start();

	return 0;
}



