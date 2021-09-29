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


	update_status status = UPDATE_CONTINUE;

	bool fullScreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fullDesktop = false;
};


#endif // __ModuleGuiManager_H__