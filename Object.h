#pragma once
#include "Transform.h"
#include "Render.h"


class Object
{
public:
	Transform myTransform;
	aie::Geometry * Geo;
	aie::Shader * shad;

	Object();

	virtual void Tick(float deltaTime);
	virtual void Draw() const;
};