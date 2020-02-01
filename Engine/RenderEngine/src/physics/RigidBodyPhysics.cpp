#include "RigidBodyPhysics.h"

RigidBodyPhysics::RigidBodyPhysics()
{

}

void RigidBodyPhysics::addRigidBody(RigidBody* r)
{
	m_rigidBodys.push_back(r);
	r->applyForce(m_gravity);
}

void RigidBodyPhysics::update(float dt)
{
	//Collision Check
	//std::vector<Collision*> collisions;
	for (int i = 0; i < m_rigidBodys.size(); i++) {
		for (int j = 0; j < m_rigidBodys.size(); j++) {
			if (i <= j) // Collision with itself
				continue;
			Collision* c = checkCollision(m_rigidBodys[i], m_rigidBodys[j]);
			if (c == nullptr) // no Collision
				continue;
			//collisions.push_back(c);
			calculateCollision(c);
		}
	}
	//Update Bodys
	/*for (Collision* c : collisions) {
		calculateCollision(c);
	}
	collisions.clear();*/
	for (int i = 0; i < m_rigidBodys.size();i++) {
		m_rigidBodys[i]->integrate(dt);
	}
}

std::vector<RigidBody*>* RigidBodyPhysics::getRigidBodyArray()
{
	return &m_rigidBodys;
}

Collision* RigidBodyPhysics::checkCollision(RigidBody* i, RigidBody* j)
{
	if (i->getShape()->getType() == SPHERE && j->getShape()->getType() == SPHERE) {
		float r1 = static_cast<Sphere*>(i->getShape())->getRadius();
		glm::vec3 pos1 = i->getShape()->getPosition();
		float r2 = static_cast<Sphere*>(j->getShape())->getRadius();
		glm::vec3 pos2 = j->getShape()->getPosition();
		if (glm::distance(pos1, pos2) <= r1 + r2) {
			Collision* c = new Collision();
			c->normal = glm::normalize(pos1 - pos2);
			c->contact = pos1 + c->normal * r1;
			c->i = i;
			c->j = j;
			float d = glm::dot(i->getLinearVelocity() - j->getLinearVelocity(), c->normal);
			if (d <= 0.0f) {
				return c;
			}
		}
		return nullptr;
	}
	else if (i->getShape()->getType() == SPHERE && j->getShape()->getType() == PLANE) {
		float r1 = static_cast<Sphere*>(i->getShape())->getRadius();
		glm::vec3 pos1 = i->getShape()->getPosition();
		glm::vec3 pos2 = j->getShape()->getPosition();
		glm::vec3 normal = glm::normalize(static_cast<Plane*>(j->getShape())->getNormal());
		glm::vec3 posOnPlane = pos1 - glm::dot(pos1 - pos2, normal) * normal;
		float distSpherePlane = glm::length(posOnPlane - pos1);
		float d = glm::dot(i->getLinearVelocity() - j->getLinearVelocity(), normal);
		if (distSpherePlane <= r1 && d <= 0.0f) {
			Collision* c = new Collision();
			c->contact = posOnPlane;
			c->normal = normal;
			c->i = i;
			c->j = j;
			return c;
		}
		return nullptr;
	}
	else if (i->getShape()->getType() == PLANE && j->getShape()->getType() == SPHERE) {
		float r1 = static_cast<Sphere*>(j->getShape())->getRadius();
		glm::vec3 pos1 = j->getShape()->getPosition();
		glm::vec3 pos2 = i->getShape()->getPosition();
		glm::vec3 normal = glm::normalize(static_cast<Plane*>(i->getShape())->getNormal());
		glm::vec3 posOnPlane = pos1 - glm::dot(pos1 - pos2, normal) * normal;
		float distSpherePlane = glm::length(posOnPlane - pos1);
		float d = glm::dot(j->getLinearVelocity() - i->getLinearVelocity(), normal);
		if (distSpherePlane <= r1 && d <= 0.0f) {
			Collision* c = new Collision();
			c->contact = posOnPlane;
			c->normal = normal;
			c->i = i;
			c->j = j;
			return c;
		}
		return nullptr;
	}
	else {
		return nullptr;
	}
}

void RigidBodyPhysics::calculateCollision(Collision* c)
{
	glm::vec3 rA = c->contact - c->i->getShape()->getPosition();
	glm::vec3 rB = c->contact - c->j->getShape()->getPosition();
	glm::vec3 ra_cross_n = glm::cross(rA, c->normal);
	glm::vec3 rb_cross_n = glm::cross(rB, c->normal);

	float s = -(1.0 + c->i->getBounciness()) * (glm::dot(c->normal, c->i->getLinearVelocity() - c->j->getLinearVelocity()) + (glm::dot(c->i->getLinearVelocity(), ra_cross_n) - glm::dot(c->j->getAngularVelocity(), rb_cross_n)));
	float t = c->i->getMassInverse() + c->j->getMassInverse() + glm::dot(ra_cross_n * c->i->getLocalInertiaTensorInverse(), ra_cross_n) + glm::dot(rb_cross_n * c->j->getLocalInertiaTensorInverse(), rb_cross_n);
	float p = s / t;
	
	glm::vec3 rA_cross_pn = glm::cross(c->j->getLinearVelocity(), rA);
	if(rA_cross_pn!= glm::vec3(0.0))
		rA_cross_pn = glm::normalize(rA_cross_pn) * p;

	glm::vec3 rB_cross_pn = glm::cross(c->i->getLinearVelocity(),rB);
	if (rB_cross_pn != glm::vec3(0.0))
		rB_cross_pn = glm::normalize(rB_cross_pn) * p;

	c->i->setAngularVelocity(c->i->getAngularVelocity() + c->i->getLocalInertiaTensorInverse() * rA_cross_pn);
	c->j->setAngularVelocity(c->j->getAngularVelocity() - c->j->getLocalInertiaTensorInverse() * rB_cross_pn);
	
	c->i->setLinearVelocity(c->i->getLinearVelocity() + (p * c->i->getMassInverse()) * c->normal);
	c->j->setLinearVelocity(c->j->getLinearVelocity() - (p * c->j->getMassInverse()) * c->normal);
	//Friction Testing
	//glm::vec3 bla = glm::cross(c->i->getForce()*c->i->getFrictionCoefficient(), c->i->getLinearVelocity());
	//c->i->setAngularVelocity(c->i->getAngularVelocity() + c->i->getLocalInertiaTensorInverse() * bla);

	//glm::vec3 bla2 = glm::cross(c->j->getForce() * c->j->getFrictionCoefficient(), c->j->getLinearVelocity());
	//c->j->setAngularVelocity(c->j->getAngularVelocity() + c->j->getLocalInertiaTensorInverse() * bla2);
}
