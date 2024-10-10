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

TheManagers Managers = {};
Camera TheCamera = {};
Vector2 FieldSize = {};

#ifdef _DEBUG
int main()
#else
int WinMain()
#endif
{
	static Game game;

	int windowHeight = 960; //height
	int windowWidth = 1280; //width

	InitWindow(windowWidth, windowHeight, "Asteroids Reimagined - RC 4.37.97");
	InitAudioDevice();

	Image icon = LoadImage("icon.png");
	ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
	SetWindowIcon(icon);

	if (IsWindowState(FLAG_VSYNC_HINT)) ClearWindowState(FLAG_VSYNC_HINT);
	glfwSwapInterval(0);
	SetTargetFPS(120);

	static Utilities TheUtilities = {};
	static GameLogic* Logic = {};

	Managers.EM.SetUtilities(&TheUtilities);

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
	Managers.SetCamera(TheCamera);

	game.Initialize(&TheUtilities);
	game.Load();
	game.BeginRun();

	while (!WindowShouldClose())
	{
		game.ProcessInput();
		game.Update(GetFrameTime());
		BeginDrawing();
		ClearBackground({ 8, 2, 16, 100 });
		game.Draw3D();
		game.Draw2D();
#ifdef _DEBUG
		DrawFPS(5, 5);
#endif
		EndDrawing();
	}

	UnloadImage(icon);

	CloseWindow();
	CloseAudioDevice();
}