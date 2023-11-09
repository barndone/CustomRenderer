#pragma once

#include <GLEW/GL/glew.h>
#include "GLM/glm.hpp"

namespace aie
{
	struct Shader
	{
		GLuint Program;			//	ogl program names

		//	shader programs are objects containing one or more shaders
		//	requiring only the vertex shader at minimum (some shaders don't need to emit fragments)
		//	it can even contain other shaders like the fragment, geometry, or tesselation shaders but are not required to!
	};

	struct Vertex
	{
		glm::vec4 Pos;
	};

	//	in order to upload verts to the GPU, we need to tell OpenGL
	//	some info about the data we're uploading and how to interpret it

	//	+ how much data does the GPU need to reserve?
	//	+ how is the data formatted?
	//	here's where buffers come in

	struct Geometry
	{
		//	VAO - Vertex Array Object (collection of related buffer objects)
		//	VBO	- Vertex Buffer Object (buffer containing vertex info)
		//	IBO - Index Bufer Object (buffer containing indices that form mesh primitive)

		GLuint Vao, Vbo, Ibo;	//	OGL bugger names
		GLuint Size;			//	OGL index count
	};

	Geometry MakeGeometry(const Vertex* const verts, GLsizei vertCount,
							const GLuint* const indicies, GLsizei indxCount);
	void FreeGeometry(Geometry& geo);

	Shader MakeShader(const char* vertSource, const char* fragSource);
	void FreeShader(Shader& shad);

	void Draw(const Shader& shad, const Geometry& geo);
}