#ifndef __MODULE_FILE_MANAGER_H__
#define __MODULE_FILE_MANAGER_H__

#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "Assimp/include/mesh.h"

class ModuleFileManager : public Module
{
public:

	ModuleFileManager(Application* app, bool start_enabled = true);
	virtual ~ModuleFileManager();

	bool Init();
	bool CleanUp();

	void LoadGeometry(const char* path);
	Primitive* LoadModel(aiMesh* mesh);

public:

	std::vector<Primitive*> meshList;
};

#endif // !__MODULE_FILE_MANAGER_H__