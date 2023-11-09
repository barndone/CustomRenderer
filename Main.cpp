//	GLFW - Create a Window
//	GLEW - OPEN GL Extension Wrangler (access openGHL APIs)
// 
//	GLM	- MATH

#include "Context.h"
#include "Render.h"

using namespace aie;

int main()
{
	Context window;
	window.Init(640, 480, "TO THE WALL");

	//	init vert positions for basic triangle
	Vertex triVerts[] =
	{
		{ { -0.5f, -0.5f, 0, 1} },
		{ { 0.5f, -0.5f, 0, 1} },
		{ { 0, 0.5f, 0, 1} }
	};
	//	init tri indices
	unsigned int triIndices[] = { 0, 1, 2 };
	//	CCW winding order

	Geometry basicTriangleGeo = MakeGeometry(triVerts, 3, triIndices, 3);

	const char* basicVert =
		"#version 430 core\n"
		"layout (location = 0) in vec4 position;\n"
		"void main() { gl_Position = position; }";

	const char* basicFrag =
		"#version 430 core\n"
		"out vec4 vertColor;\n"
		"void main() { vertColor = vec4(0.0, 1.0, 1.0, 1.0); }";

	Shader basicShad = MakeShader(basicVert, basicFrag);

	while (!window.ShouldClose())
	{
		window.Tick();
		window.Clear();

		Draw(basicShad, basicTriangleGeo);
	}

	window.Term();

	return 0;
}