#pragma warning ( push )
#pragma warning ( disable : 26495 )

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "raylib.h"
#include "Game.h"
#include "glfw/glfw3.h"

#ifdef _DEBUG
	#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
	// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
	// allocations to be of _CLIENT_BLOCK type
#else
	#define DBG_NEW new
#endif

ContentManager CM = {};
EntityManager EM = {};
FactoryManager FM = {};
KnightMath M = {};
ParticleManager Particles = {};
Camera TheCamera = {};
Vector2 FieldSize = {};
TheScore Score = {};
TheScore HighScore = {};

#ifdef _DEBUG
int main()
#else
int WinMain()
#endif
{
	static Game game;

	int windowHeight = 960; //height
	int windowWidth = 1280; //width

	InitWindow(windowWidth, windowHeight, "Asteroids Reimagined - RC 4.67.808");
	InitAudioDevice();

	Image icon = LoadImage("icon.png");
	ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
	SetWindowIcon(icon);

	if (IsWindowState(FLAG_VSYNC_HINT)) ClearWindowState(FLAG_VSYNC_HINT);
	glfwSwapInterval(0);
	SetTargetFPS(120);


	// Define the camera to look into our 3D world.
	// Camera position
	TheCamera.position = { 0.0f, 0.0f, -500.0f };
	// Camera looking at point
	TheCamera.target = { 0.0f, 0.0f, 0.0f };
	// Camera up vector (rotation towards target)
	TheCamera.up = { 0.0f, -1.0f, 0.0f };
	//Camera/World space is 1:1 screen space.
	TheCamera.fovy = (float)GetScreenHeight();
	// Camera mode type
	TheCamera.projection = CAMERA_ORTHOGRAPHIC;
	// The Managers needs a reference to The Camera
	EM.SetCamera(TheCamera);

	Particles.Initialize();
	Particles.SetEntityManager(EM);
	FM.SetEntityManager(EM);
	FM.Initialize();
	FM.BeginRun();
	game.Initialize();
	game.Load();
	game.BeginRun();

	while (!WindowShouldClose())
	{
		game.ProcessInput();

		float deltaTime = GetFrameTime();

	#if _DEBUG
		if (deltaTime > 0.05f) deltaTime = 0.05f;
	#endif

		EM.AlwaysUpdate(deltaTime);

		if (game.Logic->State != GameState::Pause && game.Logic->State != GameState::Ended)
		{
			EM.Input();
			EM.Update(deltaTime);
			EM.FixedUpdate(deltaTime);
		}

		BeginDrawing();
		ClearBackground({ 8, 2, 16, 100 });
		BeginMode3D(TheCamera);
		game.Draw3D();
		EM.Draw3D();
		EndMode3D();
		EM.Draw2D();
		game.Draw2D();
		EndDrawing();
	}

	UnloadImage(icon);

	CloseWindow();
	CloseAudioDevice();
}