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

	//LoadGeometry("Assets/BakerHouse.fbx");
	//CheckerTexture();
	//LoadTexture();

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

//void ModuleFileManager::CheckerTexture()
//{
//	GLubyte checkerImage[CHECKERS_WIDTH][CHECKERS_HEIGHT][4];
//	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
//		for (int j = 0; j < CHECKERS_WIDTH; j++) {
//			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
//			checkerImage[i][j][0] = (GLubyte)c;
//			checkerImage[i][j][1] = (GLubyte)c;
//			checkerImage[i][j][2] = (GLubyte)c;
//			checkerImage[i][j][3] = (GLubyte)255;
//		}
//	}
//	texture = (GLubyte*)checkerImage;
//}

void ModuleFileManager::LoadTexture()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, (GLuint*)textureID);
	glBindTexture(GL_TEXTURE_2D, (GLuint)textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
}