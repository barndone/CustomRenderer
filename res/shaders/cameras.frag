// fragment shader
// this is run for each fragment to the screen (basically each pixel for now)
#version 430 core

in vec4 vertColor;
out vec4 outColor;

void main()
{
  outColor = vertColor;
};