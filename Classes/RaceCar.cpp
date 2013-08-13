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

RaceCar::RaceCar(const char* carFileName, float x, float y, b2World* world) : FixtureUserData(FUD_CAR)
{
	m_sprite = cocos2d::CCSprite::create(carFileName);
	m_sprite->setPosition(cocos2d::Point(x, y));
	//create car body
	b2BodyDef carBodyDef;
	carBodyDef.type = b2_dynamicBody;
	carBodyDef.position = b2Vec2(x/PTM, y/PTM);
	carBodyDef.userData = this;
	m_body = world->CreateBody(&carBodyDef);
	m_body->SetAngularDamping(3);

	b2Vec2 vertices[6];
	vertices[0].Set(0.0f, -15.0f/PTM);
	vertices[1].Set(37.0f/PTM, -12.0f/PTM);
	vertices[2].Set(37.0f/PTM, 12.0f/PTM);
	vertices[3].Set(0.0f, 15.0f/PTM);
	vertices[4].Set(-37.0f/PTM, 14.0f/PTM);
	vertices[5].Set(-37.0f/PTM, -14.0f/PTM);
	b2PolygonShape polygonShape;
	polygonShape.Set(vertices, 6);
	m_body->CreateFixture(&polygonShape, 0.2f);//shape, density

	//prepare common joint parameters
	b2RevoluteJointDef jointDef;
	jointDef.bodyA = m_body;
	jointDef.enableLimit = true;
	jointDef.lowerAngle = 0;
	jointDef.upperAngle = 0;
	jointDef.localAnchorB.SetZero();//center of tire

	float maxForwardSpeed = 250;
	float maxBackwardSpeed = -40;
	float backTireMaxDriveForce = 300;
	float frontTireMaxDriveForce = 500;
	float backTireMaxLateralImpulse = 8.5;
	float frontTireMaxLateralImpulse = 7.5;

	//back left tire
	CarTire* tire = new CarTire(world);
	tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, backTireMaxDriveForce, backTireMaxLateralImpulse);
	jointDef.bodyB = tire->m_body;
	jointDef.localAnchorA.Set(-30.0f/PTM, 13.0f/PTM);
	world->CreateJoint(&jointDef);
	m_tires.push_back(tire);

	//back right tire
	tire = new CarTire(world);
	tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, backTireMaxDriveForce, backTireMaxLateralImpulse);
	jointDef.bodyB = tire->m_body;
	jointDef.localAnchorA.Set( -30.0f/PTM, -13.0f/PTM );
	world->CreateJoint(&jointDef);
	m_tires.push_back(tire);

	//front left tire
	tire = new CarTire(world);
	tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, frontTireMaxDriveForce, frontTireMaxLateralImpulse);
	jointDef.bodyB = tire->m_body;
	jointDef.localAnchorA.Set( 30.0f/PTM, 13.0f/PTM );
	m_flJoint = (b2RevoluteJoint*)world->CreateJoint( &jointDef );
	m_tires.push_back(tire);

	//front right tire
	tire = new CarTire(world);
	tire->setCharacteristics(maxForwardSpeed, maxBackwardSpeed, frontTireMaxDriveForce, frontTireMaxLateralImpulse);
	jointDef.bodyB = tire->m_body;
	jointDef.localAnchorA.Set( -30.0f/PTM, -13.0f/PTM );
	m_frJoint = (b2RevoluteJoint*)world->CreateJoint( &jointDef );
	m_tires.push_back(tire);
}

RaceCar::~RaceCar() {
	for (int i = 0; i < m_tires.size(); ++i)
        delete m_tires[i];
}

void RaceCar::updateCarAngle()
{
	m_sprite->setRotation(m_body->GetAngle()*RADTODEG);
}

void RaceCar::updateCarPosition()
{
	const b2Vec2 pos = m_body->GetPosition();
	m_sprite->setPosition(cocos2d::CCPoint(pos.x*PTM, pos.y*PTM));
}

b2Vec2 RaceCar::getPosition()
{
	return m_body->GetPosition();
}

float RaceCar::getAngle()
{
	return m_body->GetAngle();
}

void RaceCar::update(int controlState)
{
	for (int i = 0; i < m_tires.size(); ++i)
		m_tires[i]->updateFriction();
	for (int i = 0; i < m_tires.size(); ++i)
		m_tires[i]->updateDrive(controlState);

	//control steering
	float lockAngle = 35 * DEGTORAD;
	float turnSpeedPerSec = 160 * DEGTORAD;//from lock to lock in 0.5 sec
	float turnPerTimeStep = turnSpeedPerSec / 60.0f;
	float desiredAngle = 0;
	switch ( controlState & (TDC_LEFT|TDC_RIGHT))
	{
		case TDC_LEFT:
			desiredAngle = lockAngle;
			break;
		case TDC_RIGHT:
			desiredAngle = -lockAngle;
			break;
		default:
			break;
	}
	float angleNow = m_flJoint->GetJointAngle();
	float angleToTurn = desiredAngle - angleNow;
	angleToTurn = b2Clamp( angleToTurn, -turnPerTimeStep, turnPerTimeStep );
	float newAngle = angleNow + angleToTurn;
	m_flJoint->SetLimits( newAngle, newAngle );
	m_frJoint->SetLimits( newAngle, newAngle );
}
