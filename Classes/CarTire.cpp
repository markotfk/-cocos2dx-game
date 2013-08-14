/*
 * CarTire.cpp
 *
 *  Created on: Aug 10, 2013
 *      Author: Marko Karjalainen
 */

#include "FixtureUserData.h"
#include "Constants.h"
#include "CarTire.h"


CarTire::CarTire(b2World* world) : m_maxForwardSpeed(0), m_maxBackwardSpeed(0),
	m_maxDriveForce(0), m_maxLateralImpulse(0), m_currentTraction(0), FixtureUserData(FUD_CAR_TIRE)
{
	m_sprite = nullptr; // no sprite for tire
	m_currentTraction = 1.0/PTM;

	b2BodyDef tireBodyDef;
	tireBodyDef.type = b2_dynamicBody;
	m_body = world->CreateBody(&tireBodyDef);

	b2PolygonShape polygonShape;
	//polygonShape.SetAsBox(5.0f/PTM, 3.0f/PTM);
	polygonShape.SetAsBox(5.0f/PTM, 3.0f/PTM, b2Vec2(0, 0), 0.0f);
	m_body->CreateFixture(&polygonShape, 1.0f);//shape, density

	m_body->SetUserData(this);
}

CarTire::~CarTire()
{
	if (m_body != nullptr)
	{
		m_body->GetWorld()->DestroyBody(m_body);
	}
}

b2Vec2 CarTire::getLateralVelocity()
{
      b2Vec2 currentRightNormal = m_body->GetWorldVector(b2Vec2(1,0));
      return b2Dot( currentRightNormal, m_body->GetLinearVelocity()) * currentRightNormal;
}
b2Vec2 CarTire::getForwardVelocity()
{
    b2Vec2 currentRightNormal = m_body->GetWorldVector(b2Vec2(0,1));
    return b2Dot( currentRightNormal, m_body->GetLinearVelocity()) * currentRightNormal;
}

void CarTire::updateFriction()
{
    b2Vec2 impulse = m_body->GetMass() * -getLateralVelocity();
    m_body->ApplyLinearImpulse( impulse, m_body->GetWorldCenter() );
    m_body->ApplyAngularImpulse( 0.1f * m_body->GetInertia() * -m_body->GetAngularVelocity() );

	b2Vec2 currentForwardNormal = getForwardVelocity();
	float currentForwardSpeed = currentForwardNormal.Normalize();
	float dragForceMagnitude = -2 * currentForwardSpeed;
	m_body->ApplyForce( dragForceMagnitude * currentForwardNormal, m_body->GetWorldCenter() );
}

void CarTire::setCharacteristics(float maxForwardSpeed, float maxBackwardSpeed, float maxDriveForce, float maxLateralImpulse)
{
	m_maxForwardSpeed = maxForwardSpeed;
	m_maxBackwardSpeed = maxBackwardSpeed;
	m_maxDriveForce = maxDriveForce;
	m_maxLateralImpulse = maxLateralImpulse;
}

void CarTire::updateDrive(CarControls controlState)
{
	//find desired speed
	float desiredSpeed = 0;
	switch (controlState) {
		case CarControls::UP:
			desiredSpeed = m_maxForwardSpeed/PTM;
			break;
		case CarControls::DOWN:
			desiredSpeed = m_maxBackwardSpeed/PTM;
			break;
		default:
			break;
	}

	//find current speed in forward direction
	b2Vec2 currentForwardNormal = m_body->GetWorldVector(b2Vec2(0,1));
	float currentSpeed = b2Dot( getForwardVelocity(), currentForwardNormal );

	//apply necessary force
	float force = 0;
	if ( desiredSpeed > currentSpeed )
		force = m_maxDriveForce;
	else if ( desiredSpeed < currentSpeed )
		force = -m_maxDriveForce;
	else
		return;
	m_body->ApplyForce( m_currentTraction * force * currentForwardNormal, m_body->GetWorldCenter());
}

void CarTire::updateTurn(CarControls controlState)
{
	float desiredTorque = 0;
	switch (controlState)
	{
		case CarControls::LEFT:
			desiredTorque = 15;
			break;
		case CarControls::RIGHT:
			desiredTorque = -15;
			break;
		default:
			break;
	}
	m_body->ApplyTorque(desiredTorque);
}
