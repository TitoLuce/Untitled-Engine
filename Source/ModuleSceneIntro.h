#pragma once
#include "Module.h"
#include <vector>
#include "Globals.h"

#define BOUNCER_TIME 200

class Cube;
class Plane;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

public:
	int angle = 0;

	Plane* p;
	Cube* cube;
};
