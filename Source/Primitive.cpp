#include "Globals.h"

#include "Glew/include/glew.h"
#include "SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Primitive.h"


//#pragma comment (lib, "glut/glut32.lib")

// ------------------------------------------------------------
Primitive::Primitive() : transform(IdentityMatrix), color(White), wire(false), axis(false), type(PrimitiveTypes::Primitive_Point), vertexBuffer(-1), vertexAmount(-1), vertices(nullptr), indexBuffer(-1), indexAmount(-1), indices(nullptr),
normalsBuffer(-1), textureBuffer(-1), textureID(-1), texCoords(nullptr), normals(nullptr), colors(nullptr), texture(nullptr)
{}


// ------------------------------------------------------------
PrimitiveTypes Primitive::GetType() const
{
	return type;
}

void Primitive::GenerateBuffers()
{
	//vertices
	glGenBuffers(1, (GLuint*)&vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexAmount * 3, vertices, GL_STATIC_DRAW);

	//normals
	glGenBuffers(1, (GLuint*)&(normalsBuffer));
	glBindBuffer(GL_NORMAL_ARRAY, normalsBuffer);
	glBufferData(GL_NORMAL_ARRAY, sizeof(float) * vertexAmount * 3, normals, GL_STATIC_DRAW);

	//textures
	glGenBuffers(1, (GLuint*)&(textureBuffer));
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexAmount * 2, texCoords, GL_STATIC_DRAW);

	//indices
	glGenBuffers(1, (GLuint*)&(indexBuffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indexAmount, indices, GL_STATIC_DRAW);
}

bool Primitive::SetTexture(Texture* texture)
{
	bool ret = false;

	if (texture != nullptr && texture->data != nullptr)
	{
		this->texture = texture;
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);
		glBindTexture(GL_TEXTURE_2D, 0);
		ret = true;
	}
	else
	{
		SetCheckersTexture();
	}
	
	return ret;
}

void Primitive::SetCheckersTexture()
{
	int CHECKERS_WIDTH = 64;
	int CHECKERS_HEIGHT = 64;

	GLubyte checkerImage[64][64][4];

	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, texture);

	glBindTexture(GL_TEXTURE_2D, 0);
}

// ------------------------------------------------------------
void Primitive::Render() const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	//normals
	glBindBuffer(GL_NORMAL_ARRAY, normalsBuffer);
	glNormalPointer(GL_FLOAT, 0, NULL);

	//textures
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindTexture(GL_TEXTURE_2D, textureID);

	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glPushMatrix();
	glMultMatrixf(transform.M);

	if (axis == true)
	{
		// Draw Axis Grid
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
		glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
		glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
		glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

		glEnd();

		glLineWidth(1.0f);
	}

	glColor3f(color.r, color.g, color.b);

	if (wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, indexAmount, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_NORMAL_ARRAY, 0);
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	

	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	////vertices
	//glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);

	////normals
	//glBindBuffer(GL_NORMAL_ARRAY, normalsBuffer);
	//glNormalPointer(GL_FLOAT, 0, NULL);

	////textures
	//glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	//glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	//glBindTexture(GL_TEXTURE_2D, textureID);

	////indices
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	//glPushMatrix();
	//glMultMatrixf(transform.M);

	//if(axis == true)
	//{
	//	// Draw Axis Grid
	//	glLineWidth(2.0f);

	//	glBegin(GL_LINES);

	//	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

	//	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	//	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	//	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

	//	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

	//	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	//	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	//	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	//	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

	//	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

	//	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	//	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	//	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	//	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

	//	glEnd();

	//	glLineWidth(1.0f);
	//}

	//glColor3f(color.r, color.g, color.b);

	//if(wire)
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//else
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//

	//glDrawElements(GL_TRIANGLES, indexAmount, GL_UNSIGNED_INT, NULL);

	//

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_NORMAL_ARRAY, 0);
	//glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//

	//glBindTexture(GL_TEXTURE_2D, 0);

	//glPopMatrix();

	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
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
void Primitive::SetRotation(float angle, const vec3 &u)
{
	transform.rotate(angle, u);
}

// ------------------------------------------------------------
void Primitive::Scale(float x, float y, float z)
{
	transform.scale(x, y, z);
}

// CUBE ============================================
Cube::Cube() : Primitive()
{
	type = PrimitiveTypes::Primitive_Cube;
	
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
	texCoords = new float[16]
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

	vertexAmount = 8;
	indexAmount = 36;

	SetTexture(nullptr);
	GenerateBuffers();
}

//// SPHERE ============================================
//Sphere::Sphere() : Primitive(), radius(1.0f)
//{
//	type = PrimitiveTypes::Primitive_Sphere;
//}
//
//Sphere::Sphere(float radius) : Primitive(), radius(radius)
//{
//	type = PrimitiveTypes::Primitive_Sphere;
//}
//
//void Sphere::InnerRender() const
//{
//	//glutSolidSphere(radius, 25, 25);
//}
//
//
//// CYLINDER ============================================
//Cylinder::Cylinder() : Primitive(), radius(1.0f), height(1.0f)
//{
//	type = PrimitiveTypes::Primitive_Cylinder;
//}
//
//Cylinder::Cylinder(float radius, float height) : Primitive(), radius(radius), height(height)
//{
//	type = PrimitiveTypes::Primitive_Cylinder;
//}
//
//void Cylinder::InnerRender() const
//{
//	int n = 30;
//
//	// Cylinder Bottom
//	glBegin(GL_POLYGON);
//	
//	for(int i = 360; i >= 0; i -= (360 / n))
//	{
//		float a = i * M_PI / 180; // degrees to radians
//		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a));
//	}
//	glEnd();
//
//	// Cylinder Top
//	glBegin(GL_POLYGON);
//	glNormal3f(0.0f, 0.0f, 1.0f);
//	for(int i = 0; i <= 360; i += (360 / n))
//	{
//		float a = i * M_PI / 180; // degrees to radians
//		glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
//	}
//	glEnd();
//
//	// Cylinder "Cover"
//	glBegin(GL_QUAD_STRIP);
//	for(int i = 0; i < 480; i += (360 / n))
//	{
//		float a = i * M_PI / 180; // degrees to radians
//
//		glVertex3f(height*0.5f,  radius * cos(a), radius * sin(a) );
//		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a) );
//	}
//	glEnd();
//}
//
//// LINE ==================================================
//Line::Line() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
//{
//	type = PrimitiveTypes::Primitive_Line;
//}
//
//Line::Line(float x, float y, float z) : Primitive(), origin(0, 0, 0), destination(x, y, z)
//{
//	type = PrimitiveTypes::Primitive_Line;
//}
//
//void Line::InnerRender() const
//{
//	glLineWidth(2.0f);
//
//	glBegin(GL_LINES);
//
//	glVertex3f(origin.x, origin.y, origin.z);
//	glVertex3f(destination.x, destination.y, destination.z);
//
//	glEnd();
//
//	glLineWidth(1.0f);
//}

// PLANE ==================================================
Plane::Plane() : Primitive(), normal(0, 1, 0), constant(1)
{
	type = PrimitiveTypes::Primitive_Plane;
}

Plane::Plane(float x, float y, float z, float d) : Primitive(), normal(x, y, z), constant(d)
{
	type = PrimitiveTypes::Primitive_Plane;
}

void Plane::Render() const
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