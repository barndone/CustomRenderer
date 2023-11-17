// vertex shader
#version 430 core

// vertex inputs
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normals;
layout (location = 3) in vec4 colors;

// uniforms (shader program globals)
layout (location = 0) uniform mat4 proj;
layout (location = 1) uniform mat4 view;
layout (location = 2) uniform mat4 model;
layout (location = 3) uniform float time;

out vec2 vUV;
out vec3 vNormal;
out vec4 vertColor;
out vec4 pos;

void main()
{
  // transform object from
  // object space to world (model)
  // world to camera       (view)
  // camera to clip        (proj)

  vec4 temp_pos = position;
  gl_Position = proj * view * model * temp_pos;
  temp_pos.y += sin(time);
  
  pos = temp_pos;

  vUV = uv;
  vertColor = colors;
  //  calculate how much of our surface is facing our direction light
  //  normals are in object space, whereas light will be in world space
  //  normally- we would convert by multiplying against the model matrix, but we don't want translation to apply!
  //  isolate the rotation and scale of the object using some shader trickery-
  vNormal = mat3(transpose(inverse(model))) * normals;
}