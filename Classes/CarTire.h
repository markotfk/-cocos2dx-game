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
#include "Constants.h"
#include "FixtureUserData.h"

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
	void addGroundArea(GroundAreaFUD* ga);
	void removeGroundArea(GroundAreaFUD* ga);
	void updateTractionAndDrag();

public: // public fields
	b2Body* m_body;

private: // private fields

	float m_maxForwardSpeed;
	float m_maxBackwardSpeed;
	float m_maxDriveForce;
	float m_maxLateralImpulse;
	float m_currentTraction;
	std::set<GroundAreaFUD*> m_groundAreas;

	float m_currentDrag;
	float m_lastDriveImpulse;
	float m_lastLateralFrictionImpulse;
};

#endif
