#include "ModuleGuiManager.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

Application::Application(ConsoleBuffer* _buff)
{
	buff = _buff;
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	gui = new ModuleGuiManager(this);


	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);

	AddModule(scene_intro);

	AddModule(gui);
	AddModule(renderer3D);
}

Application::~Application()
{
	std::vector<Module*>::reverse_iterator item = list_modules.rbegin();

	while (item != list_modules.rend())
	{
		delete* item;
		++item;
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	std::vector<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init();
		++item;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Start();
		++item;
	}
	
	return ret;
}


// ---------------------------------------------
void Application::PrepareUpdate()
{
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	std::vector<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate();
		++item;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update();
		++item;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate();
		++item;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::vector<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		++item;
	}
	return ret;
}


void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::RequestBrowser(const char* url) const
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

void Application::SetAppName(std::string _name)
{
	if (!_name.empty())
	{
		appName = _name;
		window->SetTitle(_name.c_str());
	}
}

const char* Application::GetAppName()
{
	return appName.c_str();
}

void Application::SetOrgName(std::string _name)
{
	if (!_name.empty())
	{
		orgName = _name;
	}
}
const char* Application::GetOrgName()
{
	return orgName.c_str();
}

//void Application::LoadPrefs()
//{
//	char* buffer = nullptr;
//	fs->Load(SETTINGS_FOLDER "config.json", &buffer);
//
//	if (buffer != nullptr)
//	{
//		Config config((const char*)buffer);
//
//		if (config.IsValid() == true)
//		{
//			LOG("Loading Engine Preferences");
//
//			ReadConfiguration(config.GetSection("App"));
//
//			Config section;
//			for (list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
//			{
//				section = config.GetSection((*it)->GetName());
//				//if (section.IsValid())
//				(*it)->Load(&section);
//			}
//		}
//		else
//			LOG("Cannot load Engine Preferences: Invalid format");
//
//		RELEASE_ARRAY(buffer);
//	}
//}