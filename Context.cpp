#include "Context.h"

#include "GLEW/GL/glew.h"       //	include glew before glfw
#include "GLFW/glfw3.h"

#include <iostream>

//	Error callback called by OpenGl whenever a problem occurs (vendor-dependent)
void APIENTRY errorCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, const void* userParam)
{
	std::cerr << message << std::endl;
}

bool aie::Context::Init(int width, int height, const char* title)
{
	//	init glfw
	glfwInit();

	//	create window
	window = glfwCreateWindow(width, height, title, nullptr, nullptr);

	//	designate specified window as rendering context
	glfwMakeContextCurrent(window);

	//	init glew
	GLenum err = glewInit();
	//	check if init failure
	if (GLEW_OK != err)
	{
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		return false;
	}

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(errorCallback, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, true);
#endif // _DEBUG


	//	printing diagnostics
	std::cout << "OpenGl Version: " << (const char*)glGetString(GL_VERSION) << "\n";
	std::cout << "Renderer: " << (const char*)glGetString(GL_RENDERER) << "\n";
	std::cout << "Vencor: " << (const char*)glGetString(GL_VENDOR) << "\n";
	std::cout << "GLSL: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

	//	set up defaults
	glEnable(GL_BLEND);				//	enable blending
	glEnable(GL_DEPTH_TEST);		//	enable depth testing
	glEnable(GL_CULL_FACE);			//	enable backface culling

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//	set blending function
	glDepthFunc(GL_LEQUAL);								//	depth test function
	glFrontFace(GL_CCW);								//	set front face winding order
	glCullFace(GL_BACK);								//	cull backfaces

	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);			//	color to clear the screen to

    return true;
}

void aie::Context::Tick()
{
	//	swap framebuffer
	glfwSwapBuffers(window);
	//	work through event queue
	glfwPollEvents();
}

void aie::Context::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void aie::Context::Term()
{
	//	destroy the window
	glfwDestroyWindow(window);

	//	clean up glfw
	glfwTerminate();
}

bool aie::Context::ShouldClose() const
{
    return glfwWindowShouldClose(window);
}
