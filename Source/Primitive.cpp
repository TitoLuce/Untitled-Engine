#include "Globals.h"

#include "Glew/include/glew.h"
#include "SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Primitive.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//#pragma comment (lib, "glut/glut32.lib")

// ------------------------------------------------------------
Mesh::Mesh()
{
	vertexBuffer = 0;
	numVertices = 0;
	vertices = nullptr;

	//VAO = 0;

	indexBuffer = 0;
	numIndices = 0;
	indices = nullptr;

	normalsBuffer = 0;
	normals = nullptr;

	textureBuffer = 0;
	textureCoords = nullptr;
	textureID = 0;

	checkerTextureID = 0;
}

Mesh::~Mesh()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vertexBuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &normalsBuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &textureBuffer);
}

void Mesh::Initialize()
{
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 3, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * numIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &normalsBuffer);
	glBindBuffer(GL_NORMAL_ARRAY, normalsBuffer);
	glBufferData(GL_NORMAL_ARRAY, sizeof(float) * numVertices * 3, normals, GL_STATIC_DRAW);

	glGenBuffers(1, &textureBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textureBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * numVertices * 2, textureCoords, GL_STATIC_DRAW);
}

void Mesh::Render() const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// Vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	// Normals
	glBindBuffer(GL_NORMAL_ARRAY, normalsBuffer);
	glNormalPointer(GL_FLOAT, 0, NULL);

	// Coord
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	// Checker Texture
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	//glBindTexture(GL_TEXTURE_2D, checkerTextureID);

	// VAO
	//glBindVertexArray(VAO);

	// Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);

	// Unbind Buffers
	//glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_NORMAL_ARRAY, 0);
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

uint Mesh::CreateTexture(const char* path)
{
	uint tmpTextureID;
	int height;
	int width;
	int compPerPixel;
	unsigned char* pixels = stbi_load(path, &width, &height, &compPerPixel, STBI_rgb);
	GLint internalFormat = GL_RGBA;
	if (compPerPixel == 3) internalFormat = GL_RGB;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &tmpTextureID);
	glBindTexture(GL_TEXTURE_2D, tmpTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, pixels);

	stbi_image_free(pixels);

	return tmpTextureID;
}

void Mesh::CreateCheckerTexture()
{
	int CHECKERS_WIDTH = 128;
	int CHECKERS_HEIGHT = 128;

	GLubyte checkerImage[128][128][4];
	for (int i = 0; i < CHECKERS_HEIGHT; i++)
	{
		for (int j = 0; j < CHECKERS_WIDTH; j++)
		{
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &checkerTextureID);
	glBindTexture(GL_TEXTURE_2D, checkerTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);

	glBindTexture(GL_TEXTURE_2D, 0);
}

// ------------------------------------------------------------
Primitive::Primitive() : transform(IdentityMatrix), color(White), wire(false), axis(false), type(PrimitiveTypes::Primitive_Cube)
{}

Primitive::~Primitive()
{}

// ------------------------------------------------------------
PrimitiveTypes Primitive::GetType() const
{
	return type;
}

// ------------------------------------------------------------
void Primitive::Render() const
{
	glPushMatrix();
	glMultMatrixf(transform.M);

	if (axis == true)
	{
		// Draw Axis Grid
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f);      glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.1f, 0.0f);      glVertex3f(1.1f, -0.1f, 0.0f);
		glVertex3f(1.1f, 0.1f, 0.0f);	   glVertex3f(1.0f, -0.1f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f);	   glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, 1.25f, 0.0f);   glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.05f, 1.25f, 0.0f);	   glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.0f, 1.15f, 0.0f);	   glVertex3f(0.0f, 1.05f, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f);	   glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.05f, 0.1f, 1.05f);   glVertex3f(0.05f, 0.1f, 1.05f);
		glVertex3f(0.05f, 0.1f, 1.05f);    glVertex3f(-0.05f, -0.1f, 1.05f);
		glVertex3f(-0.05f, -0.1f, 1.05f);  glVertex3f(0.05f, -0.1f, 1.05f);

		glEnd();

		glLineWidth(1.0f);
	}

	glColor3f(color.r, color.g, color.b);

	if (wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (type != PrimitiveTypes::Imported_Mesh)
		InnerRender();

	glPopMatrix();
}

// ------------------------------------------------------------
void Primitive::InnerRender() const
{
	glPointSize(5.0f);

	glBegin(GL_POINTS);

	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();

	glPointSize(1.0f);
}

// ------------------------------------------------------------
void Primitive::SetPos(float x, float y, float z)
{
	transform.translate(x, y, z);
}

// ------------------------------------------------------------
void Primitive::SetRotation(float angle, const vec3& u)
{
	transform.rotate(angle, u);
}

// ------------------------------------------------------------
void Primitive::Scale(float x, float y, float z)
{
	transform.scale(x, y, z);
}

// CUBE ============================================

//Not on use
Cube::Cube() : Mesh()
{

	//Generate Vertices
	vertices = new float[24]
	{
		//Bottom Vertices
		0.0f ,0.0f, 0.0f, //0
		1.0f ,0.0f, 0.0f, //1
		1.0f ,0.0f, 1.0f, //2
		0.0f ,0.0f, 1.0f, //3

		//Top Vertices
		0.0f, 1.0f, 0.0f, //4
		1.0f, 1.0f, 0.0f, //5
		1.0f, 1.0f, 1.0f, //6
		0.0f, 1.0f, 1.0f //7
	};

	//Generate triangles through vertices (Always do it counterclockwise)
	indices = new uint[36]
	{
		//Bottom face
		0,1,2, 2,3,0,
		//Front Face
		3,2,6, 6,7,3,
		//Left face
		7,4,0, 0,3,7,
		//Right face
		2,1,5, 5,6,2,
		//Back face
		1,0,4, 4,5,1,
		//Top face
		5,4,7, 7,6,5
	};

	//UVs
	textureCoords = new float[16]
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};

	numVertices = 8;
	numIndices = 36;

}

//Cube::Cube() : Primitive(), size(1.0f, 1.0f, 1.0f)
//{
//	type = PrimitiveTypes::Primitive_Cube;
//}
//
//Cube::Cube(float sizeX, float sizeY, float sizeZ) : Primitive(), size(sizeX, sizeY, sizeZ)
//{
//	type = PrimitiveTypes::Primitive_Cube;
//}
//
//void Cube::InnerRender() const
//{
//	float sx = size.x * 0.5f;
//	float sy = size.y * 0.5f;
//	float sz = size.z * 0.5f;
//
//	glBegin(GL_QUADS);
//
//	glNormal3f(0.0f, 0.0f, 1.0f);
//	glVertex3f(-sx, -sy, sz);
//	glVertex3f(sx, -sy, sz);
//	glVertex3f(sx, sy, sz);
//	glVertex3f(-sx, sy, sz);
//
//	glNormal3f(0.0f, 0.0f, -1.0f);
//	glVertex3f(sx, -sy, -sz);
//	glVertex3f(-sx, -sy, -sz);
//	glVertex3f(-sx, sy, -sz);
//	glVertex3f(sx, sy, -sz);
//
//	glNormal3f(1.0f, 0.0f, 0.0f);
//	glVertex3f(sx, -sy, sz);
//	glVertex3f(sx, -sy, -sz);
//	glVertex3f(sx, sy, -sz);
//	glVertex3f(sx, sy, sz);
//
//	glNormal3f(-1.0f, 0.0f, 0.0f);
//	glVertex3f(-sx, -sy, -sz);
//	glVertex3f(-sx, -sy, sz);
//	glVertex3f(-sx, sy, sz);
//	glVertex3f(-sx, sy, -sz);
//
//	glNormal3f(0.0f, 1.0f, 0.0f);
//	glVertex3f(-sx, sy, sz);
//	glVertex3f(sx, sy, sz);
//	glVertex3f(sx, sy, -sz);
//	glVertex3f(-sx, sy, -sz);
//
//	glNormal3f(0.0f, -1.0f, 0.0f);
//	glVertex3f(-sx, -sy, -sz);
//	glVertex3f(sx, -sy, -sz);
//	glVertex3f(sx, -sy, sz);
//	glVertex3f(-sx, -sy, sz);
//
//	glEnd();
//}

// SPHERE ============================================
Sphere::Sphere() : Primitive(), radius(1.0f)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

Sphere::Sphere(float radius) : Primitive(), radius(radius)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

void Sphere::InnerRender() const
{
	int stacks = 16;
	int slices = 16;

	int i, j;
	for (j = 0; j < stacks; j++)
	{
		double lat1 = (M_PI / stacks) * j - M_PI / 2;
		double lat2 = (M_PI / stacks) * (double(j) + 1) - M_PI / 2;
		double sinLat1 = sin(lat1);
		double cosLat1 = cos(lat1);
		double sinLat2 = sin(lat2);
		double cosLat2 = cos(lat2);
		glBegin(GL_QUAD_STRIP);
		for (i = 0; i <= slices; i++)
		{
			double longitude = (2 * M_PI / slices) * i;
			double sinLong = sin(longitude);
			double cosLong = cos(longitude);
			double x1 = cosLong * cosLat1;
			double y1 = sinLong * cosLat1;
			double z1 = sinLat1;
			double x2 = cosLong * cosLat2;
			double y2 = sinLong * cosLat2;
			double z2 = sinLat2;
			glNormal3d(x2, y2, z2);
			glVertex3d(radius * x2, radius * y2, radius * z2);
			glNormal3d(x1, y1, z1);
			glVertex3d(radius * x1, radius * y1, radius * z1);
		}
		glEnd();
	}
}

// CYLINDER ============================================
Cylinder::Cylinder() : Primitive(), radius(1.0f), height(1.0f)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

Cylinder::Cylinder(float radius, float height) : Primitive(), radius(radius), height(height)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

void Cylinder::InnerRender() const
{
	int n = 30;

	// Cylinder Bottom
	glBegin(GL_POLYGON);

	for (int i = 360; i >= 0; i -= (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(-height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder Top
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	for (int i = 0; i <= 360; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder "Cover"
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < 480; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians

		glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
		glVertex3f(-height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();
}

// LINE ==================================================
Line::Line() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
{
	type = PrimitiveTypes::Primitive_Line;
}

Line::Line(float x, float y, float z) : Primitive(), origin(0, 0, 0), destination(x, y, z)
{
	type = PrimitiveTypes::Primitive_Line;
}

void Line::InnerRender() const
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(destination.x, destination.y, destination.z);

	glEnd();

	glLineWidth(1.0f);
}

// PLANE ==================================================
Plane::Plane() : Primitive(), normal(0, 1, 0), constant(1)
{
	type = PrimitiveTypes::Primitive_Plane;
	color = White;
}

Plane::Plane(float x, float y, float z, float d) : Primitive(), normal(x, y, z), constant(d)
{
	type = PrimitiveTypes::Primitive_Plane;
	color = White;
}

void Plane::InnerRender() const
{
	glLineWidth(1.0f);

	glBegin(GL_LINES);

	float d = 200.0f;

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();
}

// PYRAMID ================================================
Pyramid::Pyramid() : Primitive(), base(1.0f, 1.0f), height(1.0f)
{
	type = PrimitiveTypes::Primitive_Pyramid;
}

Pyramid::Pyramid(float baseX, float baseZ, float height) : Primitive(), base(baseX, baseZ), height(height)
{
	type = PrimitiveTypes::Primitive_Pyramid;
}

void Pyramid::InnerRender() const
{
	float bx = base.x * 0.5f;
	float bz = base.y * 0.5f;
	float sh = height * 0.5f;

	glBegin(GL_QUADS);

	// Base
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-bx, -sh, bz);
	glVertex3f(bx, -sh, bz);
	glVertex3f(bx, -sh, -bz);
	glVertex3f(-bx, -sh, -bz);

	glEnd();

	glBegin(GL_TRIANGLES);

	// Faces
	float ang = atan(height / bz);
	glNormal3f(0.0f, ang, ang);

	glVertex3f(0, sh, 0);
	glVertex3f(-bx, -sh, bz);
	glVertex3f(bx, -sh, bz);

	ang = atan(height / bx);
	glNormal3f(ang, ang, 0.0f);

	glVertex3f(0, sh, 0);
	glVertex3f(bx, -sh, bz);
	glVertex3f(bx, -sh, -bz);


	ang = atan(height / -bz);
	glNormal3f(0.0f, ang, ang);

	glVertex3f(0, sh, 0);
	glVertex3f(bx, -sh, -bz);
	glVertex3f(-bx, -sh, -bz);


	ang = atan(height / -bx);
	glNormal3f(ang, ang, 0.0f);

	glVertex3f(0, sh, 0);
	glVertex3f(-bx, -sh, -bz);
	glVertex3f(-bx, -sh, bz);
	glEnd();
}