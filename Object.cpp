#include "Object.h"

Object::Object()
{
	myTransform = Transform();
}

void Object::Tick(float deltaTime)
{
	myTransform.LocalRot *= glm::quat(glm::vec3(0, 0.15f, 0));
}

void Object::Draw() const
{
	aie::SetUniform(*shad, 2, myTransform.LocalMat());
	aie::Draw(*shad, *Geo);
}
