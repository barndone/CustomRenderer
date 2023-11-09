#include "Render.h"

namespace aie
{
	Geometry aie::MakeGeometry(const Vertex* const verts, GLsizei vertCount, const GLuint* const indicies, GLsizei indxCount)
	{
		//	create an instance of Geometry
		Geometry newGeo = {};
		newGeo.Size = indxCount;

		//	generate buffers
		//	glGenXXX functions accept:
		//		- 1. the number of objects to generate
		//		- 2. a pointer to an array of GLuint objects 
		//			 that will be filled with the generated object names

		glGenVertexArrays(1, &newGeo.Vao);	//	make 1 VAO
		glGenBuffers(1, &newGeo.Vbo);		//	make 1 VBO
		glGenBuffers(1, &newGeo.Ibo);		//	make 1 IBO

		//	if we know how many meshes will be needed to load, we can pass that into those 
		//  functions to generate all of the necessary buffers in a single call instead of one-by-one

		//	next: bind the buffers (VAO -> VBO -> IBO)
		glBindVertexArray(newGeo.Vao);
		glBindBuffer(GL_ARRAY_BUFFER, newGeo.Vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newGeo.Ibo);
		
		//	by binding the VAO first- this allows subsequent bound buffers to be associated with that VAO!
		//	now that they are bound, we can begin buffering data into them!

		//	buffer vertex data
		//	we use glBufferData for this which needs:
		//		1.	what buffer are we buffering data into?
		//		2.	how much data is being buffered?
		//		3.	a pointer to the data being buffered
		//		4.	how should the data be interpreted?

		//	do this for both the VBO and IBO
		glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Vertex), verts, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indxCount * sizeof(GLsizei), indicies, GL_STATIC_DRAW);
		
		//	after both of these return, we can assume our buffers are fully populated with data-
		//	before we finish, we have to tell OpenGL how to interpret the data we've buffered 
		//	which allows OpenGL to decompose our data into the individual properties 
		//	that our vert shader will accept

		//	describe vertex data
		//	enable the use of a vertex attribute at a specific index
		glEnableVertexAttribArray(0);			//	attribute index to enable

		//	describes data offsets for a specific vertex attribute
		glVertexAttribPointer(0,				//	attribute index
							  4,				//	number of components
							  GL_FLOAT,			//	type of data
							  GL_FALSE,			//	should the data be normalized?
							  sizeof(Vertex),	//	stride (in bytes) between verts
							  (void*)0);		//	offset (in bytes)

		//	if we were to add new attributes to our vert structure:
		//	we'd need to enable those attribute indices and describe them appropriately
		//	to prevent accidentally overwriting this data--
		//	we will unbind the VAO, VBO, and IBO before returning our geo
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return newGeo;
	}
	
	void aie::FreeGeometry(Geometry& geo)
	{
		//	delete the buffers we've created and zero out the geometry object to 
		//	prevent us from accidentally referring to those now de-allocated handles
		//	**order doesn't matter as long as we delete all of them!!

		glDeleteBuffers(1, &geo.Vbo);
		glDeleteBuffers(1, &geo.Ibo);
		glDeleteVertexArrays(1, &geo.Vao);

		geo = {};

		//	this will free up VRAM on the GPU for other purposes like more textures or meshes to load!
	}
	Shader MakeShader(const char* vertSource, const char* fragSource)
	{
		//	overall process:
		//  1.	create a new shader program
		//	2.	load the source code for the vertex and fragment shaders
		//	3.	compile the shaders
		//	4.	attach the shaders to the shader program
		//	5.	link the shader program
		//	6.	clean up the shaders (deleting them, not the program)
		//	7.	wrute the shader program to our shader object and return it

		//	make the shader object
		Shader newShad = {};
		newShad.Program = glCreateProgram();

		//	create shaders
		GLuint vert = glCreateShader(GL_VERTEX_SHADER);
		GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);

		//	load source code for each and compile:
		glShaderSource(vert, 1, &vertSource, 0);
		glShaderSource(frag, 1, &fragSource, 0);
		glCompileShader(vert);
		glCompileShader(frag);

		//	attach and link shaders
		glAttachShader(newShad.Program, vert);
		glAttachShader(newShad.Program, frag);
		glLinkProgram(newShad.Program);

		glDeleteShader(vert);
		glDeleteShader(frag);

		return newShad;
	}
	void FreeShader(Shader& shad)
	{
		glDeleteProgram(shad.Program);
		shad = {};
	}
	void Draw(const Shader& shad, const Geometry& geo)
	{
		//	we have to: 
		//	bind the shader program
		glUseProgram(shad.Program);
		//	bind the vertex array ojbect
		glBindVertexArray(geo.Vao);
		//	draw the geometry
		glDrawElements(GL_TRIANGLES, geo.Size, GL_UNSIGNED_INT, 0);
	}
}

