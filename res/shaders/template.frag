// fragment shader
// this is run for each fragment to the screen (basically each pixel for now)
#version 430 core

struct Light
{
	vec3 pos;
	float intensity;
	float range;
	vec4 color;
};

layout (location = 4) uniform sampler2D albedo;
layout (location = 5) uniform vec3 ambientLight;
layout (location = 6) uniform vec3 lightDir;
layout (location = 7) uniform vec4 dirLightColor;
layout (location = 8) uniform Light lights[%%n%%];

in vec2 vUV;
in vec3 vNormal;
in vec4 vertColor;
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

  vec4 base = texture(albedo, vUV) * vertColor;
    outColor.xyz = base.xyz * (ambientLight + diffuse);
    outColor.a = base.a;
};