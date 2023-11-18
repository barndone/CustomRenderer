#include "LightObj.h"

LightObj::LightObj(aie::Light light)
{
	this->light = &light;
	myTransform.LocalPos = light.pos_range;
}

void LightObj::Tick(float deltaTime)
{
	myTransform.LocalPos.y += glm::sin(deltaTime) / waveSpeed;
	light->pos_range.y = myTransform.LocalPos.y;
}
