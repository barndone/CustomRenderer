// fragment shader
// this is run for each fragment to the screen (basically each pixel for now)
#version 430 core

struct Light
{
	vec3 pos;         // 4 4 4
	float intensity;  // 4
	float range;      // 4
	vec4 color;       // 4 4 4 4
};

layout (location = 4) uniform sampler2D albedo;
layout (location = 5) uniform vec3 ambientLight;
layout (location = 6) uniform vec3 lightDir;
layout (location = 7) uniform vec4 dirLightColor;

layout (std430, binding = 0) uniform LightBlock
{
  Light lights[1];
}; 


in vec2 vUV;
in vec3 vNormal;
in vec4 vertColor;
in vec4 pos;
out vec4 outColor;

void main()
{
    //  calculate diffuse-
    //    lambertian reflection---
    //      --  perceived brightness of a lambertian surface is the same regardless
    //           of the viewing angle
    //      --  perceived brightness obeys lambert's cosine law

    //    lambert's cosine law - brightness is proportional to the cosine of the angle 
    //    between the direction of light being reflected and surface direction

    //  presumes that the incident light is pointing towards the light, whereas ours is from
  float d = max(0, dot(vNormal, -lightDir));
  vec3 diffuse = vec3(d) * dirLightColor.xyz; //  equivalent to vec3(d, d, d)

  vec3 newLightDir = pos.xyz - lights[0].pos;
  float newD = max (0, dot(vNormal, newLightDir));  
  //float distance = pow(pos.x - lights[0].pos.x, 2) + pow(pos.y - lights[0].pos.y, 2) + pow(pos.z - lights[0].pos.z, 2));
  float dist = sqrt(length(pos.xyz - lights[0].pos));
  //diffuse +=  lights[0].color.xyz * (lights[0].range / pow(dist, 2.0));
  diffuse = vec3(lights[0].range,lights[0].range,lights[0].range);
  


  vec4 base = texture(albedo, vUV) * vertColor;
    outColor.xyz = base.xyz * (ambientLight + diffuse);
    outColor.a = base.a;
};
