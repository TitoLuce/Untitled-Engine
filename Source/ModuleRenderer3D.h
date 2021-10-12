#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

struct MeshData
{
	uint id_vertex = 0;       // Vertex Buffer
	uint num_vertex = 0;      // num of Vertex
	float* vertex = nullptr;  // Vertex Array

	uint id_index = 0;        // Index Buffer
	uint num_index = 0;       // num of Index
	uint* index = nullptr;    // Index Array
};

struct SceneData
{
	std::vector<MeshData*> myMeshes;
};

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	void OnResize(int width, int height);

	bool GetWireframe();
	void ToggleWireframe();

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;


private:
	bool vSync = false;
	bool wireframe = false;

	SceneData* scene = nullptr;
};