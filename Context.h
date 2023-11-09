#pragma once

//	redeclare GLFWwindow from GLFW so we can avoid including it
//	NOTE: GLFW does not use namespaces

struct GLFWwindow;

namespace aie
{
	class Context
	{
		GLFWwindow* window;

	public:
		bool Init(int width, int height, const char* title);
		void Tick();
		void Clear();
		void Term();

		bool ShouldClose() const;
	};
}