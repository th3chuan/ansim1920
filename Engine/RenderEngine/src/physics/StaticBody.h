#pragma once
#include "RigidBody.h"
class StaticBody : public RigidBody {
public:
	StaticBody(Object* object, int shape);
	void integrate(float dt) override;
	void applyForce(glm::vec3 force) override;
	void setInitialVelocity(glm::vec3 v);
	void setLinearVelocity(glm::vec3 v);
	void setLinearMomentum(glm::vec3 p);
	void setAngularVelocity(glm::vec3 w);
};