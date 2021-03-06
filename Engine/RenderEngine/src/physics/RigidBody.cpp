#include "RigidBody.h"

RigidBody::RigidBody(Object* object, int shape)
{
	m_object = object;
	if (shape == SPHERE) {
		m_shape = new Sphere(m_object->getPosition(), m_object->getRotationMatrix(), 1.0);
	}
	else if(shape == PLANE){
		m_shape = new Plane(m_object->getPosition(), m_object->getRotationMatrix(), glm::mat3(m_object->getNormalMatrix()) * glm::vec3(0.0, 1.0, 0.0));
	}
	else {
		std::cout << "Shape "<< shape <<" not Supported" << std::endl;
	}

	m_coefficientOfRestitution = 0.95f;
	m_staticFriction = 0.04;
	m_dynamicFriction = 0.04;
	m_linearVelocity = glm::vec3(0.0);
	m_linearAcceleration = glm::vec3(0.0);
	m_linearMomentum = glm::vec3(0.0);
	m_force = glm::vec3(0.0);

	m_angularVelocity = glm::vec3(0.0);
	m_angularAcceleration = glm::vec3(0.0);
	m_angularMomentum = glm::vec3(0.0);
	m_torque = glm::vec3(0.0);
}

void RigidBody::setLinearVelocity(glm::vec3 v)
{
	m_linearVelocity = v;
}

void RigidBody::setLinearMomentum(glm::vec3 p)
{
	m_linearMomentum = p;
}

void RigidBody::setAngularVelocity(glm::vec3 w)
{
	m_angularVelocity = w;
}

void RigidBody::clearForce()
{
	m_force = glm::vec3(0.0);
}

void RigidBody::clearTorque()
{
	m_torque = glm::vec3(0.0);
}

Object* RigidBody::getObject()
{
	return m_object;
}

Shape* RigidBody::getShape()
{
	return m_shape;
}

float RigidBody::getBounciness()
{
	return m_coefficientOfRestitution;
}

float RigidBody::getStaticFriction()
{
	return m_staticFriction;
}

float RigidBody::getDynamicFriction()
{
	return m_dynamicFriction;
}

glm::vec3 RigidBody::getLinearVelocity()
{
	return m_linearVelocity;
}

glm::vec3 RigidBody::getAngularVelocity()
{
	return m_angularVelocity;
}

glm::vec3 RigidBody::getLinearMomentum()
{
	return m_linearMomentum;
}

glm::mat3 RigidBody::getLocalInertiaTensorInverse()
{
	return m_localInertiaTensorInverse;
}

int RigidBody::getType()
{
	return m_type;
}

glm::vec3 RigidBody::getForce()
{
	return m_force;
}

float RigidBody::getMass()
{
	return m_mass;
}

float RigidBody::getMassInverse()
{
	return m_massInverse;
}
