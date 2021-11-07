#ifndef __MODULE_FILE_MANAGER_H__
#define __MODULE_FILE_MANAGER_H__

#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "Assimp/include/mesh.h"

#define CHECKERS_HEIGHT 128
#define CHECKERS_WIDTH 128

typedef unsigned char GLubyte;

class Mesh;

class ModuleFileManager : public Module
{
public:
	ModuleFileManager(Application* app, bool start_enabled = true);
	~ModuleFileManager();

	bool Init();
	bool Start();
	bool CleanUp();

	void LoadMesh(const char* filePath);

public:
	std::vector<Mesh*> meshList;
};

#endif // !__MODULE_FILE_MANAGER_H__