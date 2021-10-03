#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);
	float GetBrightness();
	void SetBrightness(float bright);
	void SetWidth(int w);
	int GetWidth();
	void SetHeight(int h);
	int GetHeight();
	void GetMaxWindow(int& w, int& h);
	uint GetRefreshRate();
	bool GetFullscreen();
	void SetFullscreen(bool f);
	bool GetResizable();
	void SetResizable(bool r);
	bool GetBorderless();
	void SetBorderless(bool b);
	bool GetFullscreenDesktop();
	void SetFullscreenDesktop(bool fd);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:
	int screenWidth = 1280;
	int screenHeight = 720;
	bool fullscreen = false;
	bool resizable = true;
	bool borderless = false;
	bool fullscreenDesktop = false;
};

#endif // __ModuleWindow_H__