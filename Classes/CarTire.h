/*
 * CarTire.h
 *
 *  Created on: Aug 10, 2013
 *      Author: Marko Karjalainen
 */

#ifndef CARTIRE_H_
#define CARTIRE_H_

#include <set>
#include "Box2D.h"
#include "FixtureUserData.h"
#ifndef DEGTORAD
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#endif
enum {
    TDC_LEFT     = 0x1,
    TDC_RIGHT    = 0x2,
    TDC_UP       = 0x4,
    TDC_DOWN     = 0x8
};

class GroundAreaFUD;

class CarTire : FixtureUserData {
public:
	CarTire(b2World* world);
	virtual ~CarTire();

public: // public member functions
	b2Vec2 getLateralVelocity();
	b2Vec2 getForwardVelocity();

	void updateFriction();
	void updateDrive(int controlState);
	void updateTurn(int controlState);
	void setCharacteristics(float maxForwardSpeed, float maxBackwardSpeed, float maxDriveForce, float maxLateralImpulse);

public: // public fields
	b2Body* m_body;

private: // private fields

	float m_maxForwardSpeed;
	float m_maxBackwardSpeed;
	float m_maxDriveForce;
	float m_maxLateralImpulse;
	float m_currentTraction;
	std::set<GroundAreaFUD*> m_groundAreas;
};

#endif
