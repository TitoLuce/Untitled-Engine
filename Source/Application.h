#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleGUIManager.h"
#include "ModuleFileManager.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleGuiManager* gui;
	ModuleFileManager* fileManager;

	ConsoleBuffer* buff;

private:
	std::string appName = "Untitled Engine";
	std::string orgName = "UPC CITM";
	std::string frameName = "Framerate";

	std::vector<Module*> list_modules;

public:

	Application(ConsoleBuffer * _buff);
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void RequestBrowser(const char* url) const;
	void LoadPrefs();
	//void SavePrefs() const;
	void SetAppName(std::string _name);
	const char* GetAppName();
	void SetOrgName(std::string _name);
	const char* GetOrgName();
	void SetAppFrame(std::string _name);
	const char* GetFrameName();

	const int GetMaxFps() { return maxFps; }
	void SetMaxFps(int _maxFps) { maxFps = _maxFps; }
	const float GetFps() { return (float)currentFps; }
	const int GetAverageFps() { return averageFps; }
	const float GetMs() { return (float)lastFrameMs; }

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

private:

	Timer ms_timer;

	float dt = 0;
	int maxFps;
	int frameStart;
	float currentFps;
	float averageFps;
	int lastFrameMs;
	float currentPerf;
	Timer lastFrameMST;

	Uint32 totalFrameTicks = 0;
	Uint32 totalFrames = 0;
	Uint32 startTicks;
	Uint64 startPerf;
};