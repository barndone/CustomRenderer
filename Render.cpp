#include "Render.h"

#include "GLM/gtc/type_ptr.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION 1
#include "STBI/stb_image.h"

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
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);



		//	describes data offsets for a specific vertex attribute
		//	pos
		glVertexAttribPointer(0,				//	attribute index
							  4,				//	number of components
							  GL_FLOAT,			//	type of data
							  GL_FALSE,			//	should the data be normalized?
							  sizeof(Vertex),	//	stride (in bytes) between verts
							  (void*)0);		//	offset (in bytes)

		//	(void*) mimics memory address (local pointer)
		//	UV
		glVertexAttribPointer(1,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			(void*)16);

		//	normals
		glVertexAttribPointer(2,
							  3,
							  GL_FLOAT,
							  GL_FALSE,
			                  sizeof(Vertex),
							  (void*)24);
		//	color
		glVertexAttribPointer(3,
			4,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			(void*)36);

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

	Shader LoadShader(const char* vertPath, const char* fragPath)
	{

		const char* *paths[] = { &vertPath, &fragPath };
		std::string src[2];

		for (int i = 0; i < 2; ++i)
		{
			std::ifstream file;
			file.open(*paths[i]);
			assert(file.is_open());
			
			std::string buf;
			//	using .good() after the getline will cause the file reading to break (cut out last curly bracket at end of file)
			while (std::getline(file, buf))
			{
				src[i] += buf + '\n';
			}

			file.close();
		}

		return MakeShader(src[0].c_str(), src[1].c_str());
	}

	Shader LoadShader(const std::string& vertPath, const std::string& fragPath)
	{
		const std::string* paths[] = { &vertPath, &fragPath };
		std::string src[2];

		for (int i = 0; i < 2; ++i)
		{
			std::ifstream file;
			file.open(*paths[i]);
			assert(file.is_open());

			std::string buf;
			//	using .good() after the getline will cause the file reading to break (cut out last curly bracket at end of file)
			while (std::getline(file, buf))
			{
				src[i] += buf + '\n';
			}

			file.close();
		}

		return MakeShader(src[0].c_str(), src[1].c_str());
	}

	Texture MakeTexture(unsigned width, unsigned height, unsigned channels, const unsigned char* pixels)
	{
		//	init texture object
		Texture retVal = { 0, width, height, channels };

		//	set up OGL channels based off of passed channel value
		GLenum oglFormat = GL_RED;
		switch (channels)
		{
		case 1:
			oglFormat = GL_RED;
			break;
		case 2:
			oglFormat = GL_RG;
			break;
		case 3:
			oglFormat = GL_RGB;
			break;
		case 4:
			oglFormat = GL_RGBA;
			break;
		}

		//	begin texture creation w/ ogl
		glGenTextures(1, &retVal.Handle);
		glBindTexture(GL_TEXTURE_2D, retVal.Handle);

		//	send the texture to the GPU
		glTexImage2D(GL_TEXTURE_2D,     // texture type
			0,
			oglFormat,         // color format
			width,             // width
			height,            // height
			0,
			oglFormat,         // color format
			GL_UNSIGNED_BYTE,  // type of data
			pixels);           // pointer to data

		//	OpenGL performs texture scaling itself so that we don't lose a 1:1 mapping between the texture's pixels and the framebuffer's pixels
		//	for simplicity, applying linear filtering when scaling textures
		//	for pixel art, we could apply GL_NEAREST instead

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//	unbind texture so we don't accidentally overwrite it
		glBindTexture(GL_TEXTURE_2D, 0);

		return retVal;
	}
	void FreeTexture(Texture& tex)
	{
		glDeleteTextures(1, &tex.Handle);
		tex = {};
	}

	Texture LoadTexture(const char* imagePath)
	{
		int imageWidth, imageHeight, imageFormat;
		unsigned char* rawPixelData = nullptr;

		Texture newTex = {};

		//	STBI loads images w/ origin at top-left but opengl expects bottom left
		//	to combat this, set vert flip on load to true
		stbi_set_flip_vertically_on_load(true);

		//	load the data and properties
		rawPixelData = stbi_load(imagePath,
			&imageWidth,
			&imageHeight,
			&imageFormat,
			STBI_default);

		assert(rawPixelData != nullptr && "Image failed to load.");
		newTex = MakeTexture(imageWidth, imageHeight, imageFormat, rawPixelData);

		// free data AFTER buffering texture to GPU
		stbi_image_free(rawPixelData);

		return newTex;
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

	void SetUniform(const Shader& shad, GLuint location, float value)
	{
		glProgramUniform1fv(shad.Program, location, 1, &value);
	}
	void SetUniform(const Shader& shad, GLuint location, const glm::vec3& value)
	{
		glProgramUniform3fv(shad.Program, location, 1, glm::value_ptr(value));
	}
	void SetUniform(const Shader& shad, GLuint location, const glm::vec4& value)
	{
		glProgramUniform4fv(shad.Program, location, 1, glm::value_ptr(value));
	}
	void SetUniform(const Shader& shad, GLuint location, const glm::mat4& value)
	{
		glProgramUniformMatrix4fv(shad.Program, location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void SetUniform(const Shader& shad, GLuint location, const Texture& value, int textureSlot)
	{
		// specify the texture slot we are working with
		glActiveTexture(GL_TEXTURE0 + textureSlot);
		// bind the texture to that slot
		glBindTexture(GL_TEXTURE_2D, value.Handle);

		// assign that texture (slot) to the shader
		glProgramUniform1i(shad.Program, location, textureSlot);
	}

	//	wrapper for loading/generating geometry using tiny_obj_loader lib
	Geometry LoadObj(const char* filename)
	{
		std::string inputfile = filename;

		std::vector<Vertex> verts;
		std::vector<GLuint> indices;

		tinyobj::ObjReader reader;
		tinyobj::ObjReaderConfig reader_config;

		if (!reader.ParseFromFile(inputfile, reader_config))
		{
			//	do some error checking
		}

		auto& attrib = reader.GetAttrib();
		auto& shapes = reader.GetShapes();
		auto& mats = reader.GetMaterials();

		//	loop over shapes
		for (size_t s = 0; s < shapes.size(); ++s)
		{
			//	loop over faces (polygon)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); ++f)
			{
				size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

				//	loop over verts in the face
				for (size_t v = 0; v < fv; ++v)
				{
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
					tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
					tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

					// Check if `normal_index` is zero or positive. negative = no normal data
					if (idx.normal_index >= 0) {
						tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
						tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
						tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
					}
					
					// Check if `texcoord_index` is zero or positive. negative = no texcoord data
					if (idx.texcoord_index >= 0) {
						tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
						tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
					}
					Vertex vert = { glm::vec4(vx, vy, vz, 1), 
						{ attrib.texcoords[2 * size_t(idx.texcoord_index) + 0], attrib.texcoords[2 * size_t(idx.texcoord_index) + 1] },
						{ attrib.normals[3 * size_t(idx.normal_index) + 0], attrib.normals[3 * size_t(idx.normal_index) + 1], attrib.normals[3 * size_t(idx.normal_index) + 2]},
						{ 1, 1, 1, 1 }
				};

					//	works by taking which face we are on multiplied by the number of verts on that face PLUS the vert num, 
					indices.push_back(fv * f + v);
					verts.push_back(vert);

				}

				index_offset += fv;
			}
		}

		return MakeGeometry(verts.data(), verts.size(), indices.data(), indices.size());
	}
}

