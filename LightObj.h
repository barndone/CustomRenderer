#pragma once
#include "Object.h"

class LightObj :public Object
{
public:
	aie::Light* light;

	LightObj(aie::Light light);

	virtual void Tick(float deltaTime) override;
};