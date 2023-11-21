// fragment shader
// this is run for each fragment to the screen (basically each pixel for now)
#version 430 core

#define NR_LIGHTS 3

struct Light
{
	vec4 pos_range;   // 4 4 4 4
	vec4 color;       // 4 4 4 4
};

layout (location = 4) uniform sampler2D albedo;
layout (location = 5) uniform vec3 ambientLight;
layout (location = 6) uniform vec3 lightDir;
layout (location = 7) uniform vec4 dirLightColor;

layout (binding = 0) uniform LightBlock
{
  Light lights[3];
}; 


in vec2 vUV;
in vec3 vNormal;
in vec4 vertColor;
in vec3 pos;
out vec4 outColor;

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos);

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

  vec3 pointDiffuse = vec3(0,0,0);

  for (int i = 0; i < NR_LIGHTS; ++i)
  pointDiffuse += CalcPointLight(lights[i], vNormal, pos);

  outColor.xyz = base.xyz * (ambientLight + diffuse + pointDiffuse);
  //outColor.xyz = pointDiffuse;
  outColor.a = base.a;
};

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos)
{
  vec3 direction = normalize(light.pos_range.xyz - fragPos);
  float diffuse = max(0, dot(normal, direction));
  float dist = sqrt(length(light.pos_range.xyz - fragPos));
  float attenuation = abs(1.0 - (dist / light.pos_range.w));
  vec3 d = light.color.xyz * diffuse;
  //vec3 d = vec3(1,1,1) * diffuse;

  return d * vec3(1,1,1);
}
