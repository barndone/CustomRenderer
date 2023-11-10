#include "Transform.h"

Transform::Transform()
{
	LocalPos = { 0, 0, 0 };
	LocalRot = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
	LocalScl = { 1, 1, 1 };
}

glm::mat4 Transform::LocalMat() const
{
	return
		glm::translate(glm::mat4(1.0f), LocalPos) *
		glm::toMat4(LocalRot) * 
		glm::scale(glm::mat4(1.0f), LocalScl);
}
