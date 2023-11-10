// vertex shader
#version 430 core

// vertex inputs
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 colors;

// uniforms (shader program globals)
layout (location = 0) uniform mat4 proj;
layout (location = 1) uniform mat4 view;
layout (location = 2) uniform mat4 model;
layout (location = 3) uniform float time;

out vec4 vertColor;

void main()
{
  // transform object from
  // object space to world (model)
  // world to camera       (view)
  // camera to clip        (proj)

  vec4 temp_pos = position;
  temp_pos.y += sin(time);

  gl_Position = proj * view * model * temp_pos;
  vertColor = colors;
}