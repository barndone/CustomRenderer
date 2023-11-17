//	GLFW - Create a Window
//	GLEW - OPEN GL Extension Wrangler (access openGHL APIs)
// 
//	GLM	- MATH

#include "Context.h"
#include "Object.h"
#include "Time.h"
#include "Render.h"
#include "GLM/ext.hpp"

using namespace aie;

int main()
{
	Context window;
	window.Init(1280, 720, "TO THE WALL");

	Time timer;


	//	these verts are placed in clip space, which will normalized

	Geometry soulSpear = LoadObj("res/objs/soulspear.obj");
	Texture soulSpearTex = LoadTexture("res/textures/soulspear_diffuse.tga");
	PassSpawnedLights("res/shaders/template.frag", "res/shaders/cameras.frag", 10);
	Shader basicLoadedShad = LoadShader("res/shaders/cameras.vert", "res/shaders/cameras.frag");

	Object obj;	
	obj.Geo = &soulSpear;
	obj.shad = &basicLoadedShad;

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

	//	ambient color NOT light intensity
	glm::vec3 ambient(0.5f, 0.5f, 0.5f);
	glm::vec3 sunDir(-1,0,0);
	glm::vec4 sunColor(0, 1, 0, 1);

	SetUniform(basicLoadedShad, 0, cam_proj);
	SetUniform(basicLoadedShad, 1, cam_view);
	SetUniform(basicLoadedShad, 2, triangle_model);
	SetUniform(basicLoadedShad, 4, soulSpearTex, 0);
	SetUniform(basicLoadedShad, 5, ambient);
	SetUniform(basicLoadedShad, 7, sunColor);

	while (!window.ShouldClose())
	{
		timer.Tick();

		SetUniform(basicLoadedShad, 3, timer.CurrentTime());
		SetUniform(basicLoadedShad, 6, sunDir);

		window.Tick();
		window.Clear();

		obj.Tick(timer.DeltaTime());
		obj.Draw();
	}

	window.Term();

	return 0;
}