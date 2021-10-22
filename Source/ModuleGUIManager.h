#ifndef __ModuleGuiManager_H__
#define __ModuleGuiManager_H__

#include "Module.h"
#include "Globals.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"


class ModuleGuiManager : public Module
{
public:

	ModuleGuiManager(Application* app, bool start_enabled = true);
	~ModuleGuiManager();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	update_status MenuBar();
	void Config();

	void Console();
	void LogConsole(const char* buff);

	bool configOn = false;
	bool consoleOn = false;
	bool aboutOn = false;
	bool showcase = false;
	
	ImGuiTextBuffer LogConsoleText;

private:

	void UpdateFPS();

private:

	int maxFPS;
	std::vector<float> fpsLog;
	std::vector<float> msLog;
	int fpsCounter;
	int msCounter;
	int averageFps;
};


#endif // __ModuleGuiManager_H__