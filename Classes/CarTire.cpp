/*
 * CarTire.cpp
 *
 *  Created on: Aug 10, 2013
 *      Author: Marko Karjalainen
 */

#include "cocos2d.h"
#include "FixtureUserData.h"
#include "Constants.h"
#include "CarTire.h"


CarTire::CarTire(b2World* world) : m_maxForwardSpeed(0), m_maxBackwardSpeed(0),
	m_maxDriveForce(0), m_maxLateralImpulse(0), m_currentTraction(0),
	m_lastDriveImpulse(0), m_lastLateralFrictionImpulse(0),
	FixtureUserData(FixtureUserDataType::CAR_TIRE)
{
	m_sprite = cocos2d::Sprite::create("road10.png"); // this is for debugging
	m_currentTraction = 1/PTM;
	m_currentDrag = 1/PTM;

	b2BodyDef tireBodyDef;
	tireBodyDef.type = b2_dynamicBody;
	m_body = world->CreateBody(&tireBodyDef);

	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(6.0f/PTM, 3.0f/PTM);
	m_body->CreateFixture(&polygonShape, 1.0f);//shape, density

	m_body->SetUserData(this);
}

CarTire::~CarTire()
{
	if (m_body != nullptr)
	{
		m_body->GetWorld()->DestroyBody(m_body);
	}
	for (auto i = m_groundAreas.begin(); i != m_groundAreas.end(); ++i)
	{
		delete *i;
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
	//angular velocity
	m_body->ApplyAngularImpulse( m_currentTraction * 0.1f * m_body->GetInertia() * -m_body->GetAngularVelocity() );

	//forward linear velocity
	b2Vec2 currentForwardNormal = getForwardVelocity();
	float currentForwardSpeed = currentForwardNormal.Normalize();
	float dragForceMagnitude = -0.25 * currentForwardSpeed;
	dragForceMagnitude *= m_currentDrag;
	m_body->ApplyForce( m_currentTraction * dragForceMagnitude * currentForwardNormal, m_body->GetWorldCenter() );
}

void CarTire::setCharacteristics(float maxForwardSpeed, float maxBackwardSpeed, float maxDriveForce, float maxLateralImpulse)
{
	m_maxForwardSpeed = maxForwardSpeed;
	m_maxBackwardSpeed = maxBackwardSpeed;
	m_maxDriveForce = maxDriveForce;
	m_maxLateralImpulse = maxLateralImpulse;
}

void CarTire::addGroundArea(GroundAreaFUD* ga)
{
	m_groundAreas.insert(ga);
	updateTractionAndDrag();
}
void CarTire::removeGroundArea(GroundAreaFUD* ga)
{
	m_groundAreas.erase(ga);
	updateTractionAndDrag();
}

void CarTire::updateTractionAndDrag()
    {
	if ( m_groundAreas.empty() ) {
		m_currentTraction = 1/PTM;
		m_currentDrag = 1/PTM;
	}
	else {
		//find area with highest traction, same for drag
		m_currentTraction = 0.1f/PTM;
		m_currentDrag = 1/PTM;//not zero!
		auto it = m_groundAreas.begin();
		while (it != m_groundAreas.end()) {
			GroundAreaFUD* ga = *it;
			if ( ga->frictionModifier > m_currentTraction )
				m_currentTraction = ga->frictionModifier;
			if ( ga->dragModifier > m_currentDrag )
				m_currentDrag = ga->dragModifier;
			++it;
		}
	}
}

void CarTire::updateDrive(int controlState)
{
	//find desired speed
	float desiredSpeed = 0;
	switch (controlState & (CarControls::UP|CarControls::DOWN))
	{
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
	b2Vec2 currentForwardNormal = m_body->GetWorldVector( b2Vec2(0,1) );
	float currentSpeed = b2Dot( getForwardVelocity(), currentForwardNormal );

	//apply necessary force
	float force = 0;
	if (controlState & (CarControls::UP|CarControls::DOWN))
	{
		if ( desiredSpeed > currentSpeed )
			force = m_maxDriveForce;
		else if ( desiredSpeed < currentSpeed )
			force = -m_maxDriveForce * 0.5f;
	}

	float speedFactor = currentSpeed / 120;

	b2Vec2 driveImpulse = (force / 60.0f) * currentForwardNormal;
	if ( driveImpulse.Length() > m_maxLateralImpulse )
		driveImpulse *= m_maxLateralImpulse / driveImpulse.Length();

	b2Vec2 lateralFrictionImpulse = m_body->GetMass() * -getLateralVelocity();
	float lateralImpulseAvailable = m_maxLateralImpulse;
	lateralImpulseAvailable *= 2.0f * speedFactor;
	if ( lateralImpulseAvailable < 0.5f * m_maxLateralImpulse )
		lateralImpulseAvailable = 0.5f * m_maxLateralImpulse;
	/*
	else if ( lateralImpulseAvailable > m_maxLateralImpulse )
		lateralImpulseAvailable = m_maxLateralImpulse;*/

	if ( lateralFrictionImpulse.Length() > lateralImpulseAvailable )
		lateralFrictionImpulse *= lateralImpulseAvailable / lateralFrictionImpulse.Length();

	m_lastDriveImpulse = driveImpulse.Length();
	m_lastLateralFrictionImpulse = lateralFrictionImpulse.Length();

	b2Vec2 impulse = driveImpulse + lateralFrictionImpulse;
	if ( impulse.Length() > m_maxLateralImpulse )
		impulse *= m_maxLateralImpulse / impulse.Length();
	m_body->ApplyLinearImpulse( m_currentTraction * impulse, m_body->GetWorldCenter() );
}

void CarTire::updateTurn(int controlState)
{
	float desiredTorque = 0;
	switch (controlState & (CarControls::LEFT|CarControls::RIGHT))
	{
		case CarControls::LEFT:
			desiredTorque = 15/PTM;
			break;
		case CarControls::RIGHT:
			desiredTorque = -15/PTM;
			break;
		default: ;//nothing
	}
	m_body->ApplyTorque(desiredTorque);
}
