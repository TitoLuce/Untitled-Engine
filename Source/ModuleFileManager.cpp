#include "Globals.h"
#include "Application.h"
#include "ModuleFileManager.h"
#include "ModuleSceneIntro.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"



ModuleFileManager::ModuleFileManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

// Destructor
ModuleFileManager::~ModuleFileManager()
{}

bool ModuleFileManager::Init()
{
	if (App->gui != nullptr) { App->gui->LogConsole(LOG("Init File System")); };

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	LoadGeometry("Assets/warrior.fbx");

	return true;
}

// Called before quitting
bool ModuleFileManager::CleanUp()
{
	if (App->gui != nullptr) { App->gui->LogConsole(LOG("Destroying File System")); };

	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

void ModuleFileManager::LoadGeometry(const char* path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
			meshList.push_back(LoadModel(scene->mMeshes[i]));
		}
		aiReleaseImport(scene);
	}
	else
	{
		if (App->gui != nullptr) App->gui->LogConsole(LOG("Error loading %s", path));
	}
}

Primitive* ModuleFileManager::LoadModel(aiMesh* mesh)
{
	PrimitiveData* ourMesh = new PrimitiveData;

	ourMesh->num_vertex = mesh->mNumVertices;
	ourMesh->vertices = new float[ourMesh->num_vertex * 3];
	memcpy(ourMesh->vertices, mesh->mVertices, sizeof(float) * ourMesh->num_vertex * 3);
	if (App->gui != nullptr) { App->gui->LogConsole(LOG("Loaded new mesh with %d vertex", ourMesh->num_vertex)); }

	// copy faces
	if (mesh->HasFaces())
	{
		ourMesh->num_index = mesh->mNumFaces * 3;
		ourMesh->indices = new uint[ourMesh->num_index]; // assume each face is a triangle
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			if (mesh->mFaces[i].mNumIndices != 3)
			{
				App->gui->LogConsole(LOG("WARNING, geometry face with != 3 indices!"));
			}	
			else
			{
				memcpy(&ourMesh->indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}


		CustomPrimitive* p = new CustomPrimitive(ourMesh);
		meshList.push_back(p);
		return p;
	}
	
	return nullptr;
}
