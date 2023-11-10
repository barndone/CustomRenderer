#pragma once

#include "GLM/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL 1
#include "GLM/gtx/quaternion.hpp"
#undef GLM_ENABLE_EXPERIMENTAL

class Transform
{
public:
	glm::vec3 LocalPos;
	glm::quat LocalRot;
	glm::vec3 LocalScl;

	Transform();

	glm::mat4 LocalMat() const;
};