#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__


#include "glmath.h"
#include "Color.h"

typedef float GLfloat;
typedef unsigned char GLubyte;

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder,
	Custom_Primitive
};

struct PrimitiveData
{
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* indices = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertices = nullptr;
};

struct Texture
{
	std::string name = "Unnamed Texture";
	std::string path = "Unknown path";
	uint id = -1;
	GLubyte* data = nullptr;
	int width = -1;
	int height = -1;

	~Texture()
	{
		name.clear();
		path.clear();
		//delete data;
		data = nullptr;
	}
};


class Primitive
{
public:

	Primitive();

	void GenerateBuffers();

	bool SetTexture(Texture* texture);
	void SetCheckersTexture();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const vec3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	mat4x4 transform;
	bool axis,wire;

	uint vertexBuffer = -1;
	int vertexAmount = -1;
	float* vertices = nullptr;

	uint indexBuffer = -1;
	int indexAmount = -1;
	uint* indices = nullptr;

	uint normalsBuffer;
	float* normals;

	uint textureBuffer = -1;
	uint textureID;
	float* texCoords = nullptr;
	float* colors;
	Texture* texture;

	const char* name;

protected:
	PrimitiveTypes type;
};

// ============================================
class Cube : public Primitive
{
public :
	Cube();
};

// ============================================
class Sphere : public Primitive
{
public:
	Sphere();
	Sphere(float radius);
	void InnerRender() const;
public:
	float radius;
};

// ============================================
class Cylinder : public Primitive
{
public:
	Cylinder();
	Cylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class Line : public Primitive
{
public:
	Line();
	Line(float x, float y, float z);
	void InnerRender() const;
public:
	vec3 origin;
	vec3 destination;
};

// ============================================
class Plane : public Primitive
{
public:
	Plane();
	Plane(float x, float y, float z, float d);
	void Render() const;
public:
	vec3 normal;
	float constant;
};

class CustomPrimitive :public Primitive
{
public:
	CustomPrimitive(PrimitiveData* _data);
	void InnerRender() const;

public:
	PrimitiveData* data;
};

#endif //!__PRIMITIVES_H__