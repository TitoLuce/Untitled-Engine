#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "ModuleFileManager.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	App->gui->LogConsole(LOG("Loading Intro assets"));
	bool ret = true;

	App->camera->Move(vec3(0, 2, 0));
	App->camera->LookAt(vec3(0, 0, 0));
	
	//Draw plane
	p = new Plane();
	p->axis = true;

	//Draw stuff
	//cube= new Cube();

	App->fileManager->LoadMesh("Assets/Resources/Models/BakerHouse.fbx");


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	App->gui->LogConsole(LOG("Unloading Intro scene"));

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	p->Render();
	//cube->wire = App->renderer3D->GetWireframe();
	//cube->Render();

	std::vector<Mesh*>::iterator item = App->fileManager->meshList.begin();

	while (item != App->fileManager->meshList.end())
	{
		(*item)->Render();
		++item;
	}

	return UPDATE_CONTINUE;
}

