//	GLFW - Create a Window
//	GLEW - OPEN GL Extension Wrangler (access openGHL APIs)
// 
//	GLM	- MATH

#include "Context.h"
#include "Time.h"
#include "Render.h"
#include "GLM/ext.hpp"

using namespace aie;

int main()
{
	Context window;
	window.Init(1280, 720, "TO THE WALL");

	Time timer;

	//	init vert positions for basic triangle
	Vertex triVerts[] =
	{
		{ 
			{ -0.5f, -0.5f, 0, 1},
			{ 1.0f, 0, 0, 1}
		},
		{ 
			{ 0.5f, -0.5f, 0, 1},
			{ 0, 1.0f, 0, 1}
		},
		{ 
			{ 0, 0.5f, 0, 1},
			{ 0, 0, 0.0f, 1}
		}
	};

	//	these verts are placed in clip space, which will normalized

	Vertex planeVerts[] =
	{
		{ { -0.25f, 0.25f, 0, 1} },
		{ { -0.25f, -0.25f, 0, 1} },
		{ { 0.25f, -0.25f, 0, 1} },
		{ { 0.25f, 0.25f, 0, 1} }
	};

	//	init tri indices
	unsigned int triIndices[] = { 0, 1, 2 };
	//	CCW winding order

	unsigned int planeIndices[] = { 0, 1, 2, 0, 2, 3 };

	Geometry basicTriangleGeo = MakeGeometry(triVerts, 3, triIndices, 3);
	Geometry basicPlaneGeo = MakeGeometry(planeVerts, 4, planeIndices, 6);

	Shader basicLoadedShad = LoadShader("res/shaders/cameras.vert", "res/shaders/cameras.frag");

	//	create the model matrix
	//	in this case just the identity matrix, can translate, rotate, and scale as needed
	
	glm::mat4 triangle_model = glm::identity<glm::mat4>();
	//	create the view matrix
	glm::mat4 cam_view = glm::lookAt(
		glm::vec3(0,1,20),					//	eye vector (where is the camera?)
		glm::vec3(0,5,0),					//	look at vector (what is the camera looking at?)
		glm::vec3(0,1,0)					//	the up vector (relative to the camera)
	);
	//	create the projection matrix
	glm::mat4 cam_proj = glm::perspective(
		glm::radians(45.f),					//	vertical FOV
		4.0f / 3.0f,						//	aspect ratio
		0.1f,								//	near-plane
		1000.0f								//	far-plane
	);

	SetUniform(basicLoadedShad, 0, cam_proj);
	SetUniform(basicLoadedShad, 1, cam_view);
	SetUniform(basicLoadedShad, 2, triangle_model);

	while (!window.ShouldClose())
	{
		timer.Tick();

		SetUniform(basicLoadedShad, 3, timer.CurrentTime());

		window.Tick();
		window.Clear();

		//	draw test tri
		Draw(basicLoadedShad, basicTriangleGeo);
		//	draw test plane
		//	Draw(basicShad, basicPlaneGeo);
	}

	window.Term();

	return 0;
}