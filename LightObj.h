#pragma once
#include "Object.h"

class LightObj :public Object
{
public:
	aie::Light* light;

	LightObj(aie::Light light);

	float waveSpeed = 5.0f;

	virtual void Tick(float deltaTime) override;
};