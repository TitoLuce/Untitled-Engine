#include "Globals.h"
#include "Application.h"
#include "ModuleFileManager.h"
#include "ModuleSceneIntro.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "SDL_opengl.h"

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

	//LoadTexture();

	return true;
}

bool ModuleFileManager::Start()
{
	LoadGeometry("Assets/BakerHouse.fbx");
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
	Primitive* ourMesh = new Primitive;

	ourMesh->vertexAmount = mesh->mNumVertices;
	ourMesh->vertices = new float[ourMesh->vertexAmount * 3];
	memcpy(ourMesh->vertices, mesh->mVertices, sizeof(float) * ourMesh->vertexAmount * 3);
	if (App->gui != nullptr) { App->gui->LogConsole(LOG("Loaded new mesh with %d vertex", ourMesh->vertexAmount)); }

	// copy faces
	if (mesh->HasFaces())
	{
		ourMesh->indexAmount = mesh->mNumFaces * 3;
		ourMesh->indices = new uint[ourMesh->indexAmount]; // assume each face is a triangle
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
		if (mesh->mTextureCoords != NULL)
		{
			ourMesh->texCoords = new float[ourMesh->vertexAmount * 2];

			for (uint i = 0; i < ourMesh->vertexAmount; i++)
			{
				ourMesh->texCoords[i*2] = mesh->mTextureCoords[0][i].x;
				ourMesh->texCoords[(i*2)+1] = mesh->mTextureCoords[0][i].y;
			}
		}
		else
		{
			App->gui->LogConsole(LOG("Warning, No texture coordinates found"));
		}
		if (mesh->mTextureCoords != NULL)
		{
			ourMesh->normals = new float[ourMesh->vertexAmount * 3];

			for (uint i = 0; i < ourMesh->vertexAmount; i++)
			{
				ourMesh->normals[i * 2] = mesh->mNormals[i].x;
				ourMesh->normals[(i * 2) + 1] = mesh->mNormals[i].y;
				ourMesh->normals[(i * 2) + 2] = mesh->mNormals[i].z;
			}
		}
		else
		{
			App->gui->LogConsole(LOG("Warning, No normals coordinates found"));
		}



		ourMesh->SetTexture(nullptr);
		ourMesh->GenerateBuffers();
		
		meshList.push_back(ourMesh);
		return ourMesh;
	}
	
	return nullptr;
}