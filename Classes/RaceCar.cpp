/*
 * RaceCar.cpp
 *
 *  Created on: Aug 7, 2013
 *      Author: Marko Karjalainen
 */

#include "cocos2d.h"
#include "CarTire.h"
#include "DestructionListener.h"
#include "Constants.h"
#include "RaceCar.h"

RaceCar::RaceCar(const char* carFileName, float x, float y, b2World* world) :
m_world(world), FixtureUserData(carFileName,
		FixtureUserDataType::CAR)
{
	//create car body
	b2BodyDef carBodyDef;
	carBodyDef.type = b2_dynamicBody;
	carBodyDef.position = b2Vec2(x/PTM, y/PTM);
	carBodyDef.angle = 0;
	carBodyDef.userData = this;
	m_body = m_world->CreateBody(&carBodyDef);
	m_body->SetAngularDamping(0.1f);

	b2Vec2 vertices[6];
	vertices[0].Set(0.0f, -9.0f/PTM);
	vertices[1].Set(22.0f/PTM, -8.0f/PTM);
	vertices[2].Set(22.0f/PTM, 8.0f/PTM);
	vertices[3].Set(0.0f, 9.0f/PTM);
	vertices[4].Set(-22.0f/PTM, 8.0f/PTM);
	vertices[5].Set(-22.0f/PTM, -8.0f/PTM);
	b2PolygonShape polygonShape;
	polygonShape.Set(vertices, 6);
	m_body->CreateFixture(&polygonShape, 1.0f);//shape, density

	//prepare common joint parameters
	b2RevoluteJointDef jointDef;
	jointDef.bodyA = m_body;
	jointDef.enableLimit = true;
	jointDef.lowerAngle = 0;
	jointDef.upperAngle = 0;
	jointDef.localAnchorB.SetZero();//center of tire

	float maxForwardSpeed = 100;
	float maxBackwardSpeed = -40;
	float backTireMaxDriveForce = 40;
	float frontTireMaxDriveForce = 15;
	float backTireMaxLateralImpulse = 4;
	float frontTireMaxLateralImpulse = 4;

	//back left tire
	CarTire* tire = new CarTire(world);
	tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, backTireMaxDriveForce, backTireMaxLateralImpulse);
	jointDef.bodyB = tire->m_body;
	jointDef.localAnchorA.Set(-16.0f/PTM, 8.0f/PTM);
	m_world->CreateJoint(&jointDef);
	m_tires.push_back(tire);

	//back right tire
	tire = new CarTire(world);
	tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, backTireMaxDriveForce, backTireMaxLateralImpulse);
	jointDef.bodyB = tire->m_body;
	jointDef.localAnchorA.Set( -16.0f/PTM, -8.0f/PTM );
	m_world->CreateJoint(&jointDef);
	m_tires.push_back(tire);

	//front left tire
	tire = new CarTire(world);
	tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, frontTireMaxDriveForce, frontTireMaxLateralImpulse);
	jointDef.bodyB = tire->m_body;
	jointDef.localAnchorA.Set( 16.0f/PTM, 8.0f/PTM );
	m_flJoint = (b2RevoluteJoint*)m_world->CreateJoint(&jointDef);
	m_tires.push_back(tire);

	//front right tire
	tire = new CarTire(world);
	tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, frontTireMaxDriveForce, frontTireMaxLateralImpulse);
	jointDef.bodyB = tire->m_body;
	jointDef.localAnchorA.Set( 16.0f/PTM, -8.0f/PTM );
	m_frJoint = (b2RevoluteJoint*)m_world->CreateJoint(&jointDef);
	m_tires.push_back(tire);
}

RaceCar::~RaceCar() {
	for (int i = 0; i < m_tires.size(); ++i)
        delete m_tires[i];

}
void RaceCar::update(int controlState)
{
	for (int i = 0; i < m_tires.size(); ++i)
		m_tires[i]->updateFriction();
	for (int i = 0; i < m_tires.size(); ++i)
		m_tires[i]->updateDrive(controlState);
	for (int i = 0; i < m_tires.size(); ++i)
		m_tires[i]->updateTurn(controlState);
	//control steering

	float desiredAngle = 0;
	switch (controlState & (CarControls::LEFT|CarControls::RIGHT))
	{
		case CarControls::LEFT:
			desiredAngle = LOCK_ANGLE;
			break;
		case CarControls::RIGHT:
			desiredAngle = -LOCK_ANGLE;
			break;
		default:
			break;
	}
	float angleNow = m_flJoint->GetJointAngle();
	float angleToTurn = desiredAngle - angleNow;
	angleToTurn = b2Clamp(angleToTurn, -TURN_PER_TIME_STEP, TURN_PER_TIME_STEP);
	float newAngle = angleNow + angleToTurn;
	m_flJoint->SetLimits(newAngle, newAngle);
	m_frJoint->SetLimits(newAngle, newAngle);
}
