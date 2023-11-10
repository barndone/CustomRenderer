#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 colors;

out vec4 vertColor;

void main() 
{ 
	vertColor = colors; 
	gl_Position = position; 
}