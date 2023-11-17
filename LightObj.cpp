#include "LightObj.h"

LightObj::LightObj(aie::Light light)
{
	this->light = &light;
	myTransform.LocalPos = light.pos;
}

void LightObj::Tick(float deltaTime)
{
	myTransform.LocalPos.y += glm::sin(deltaTime);
	light->pos = myTransform.LocalPos;
}
