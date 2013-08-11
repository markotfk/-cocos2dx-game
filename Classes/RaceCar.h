/*
 * RaceCar.h
 *
 *  Created on: Aug 7, 2013
 *      Author: mtfk
 */

#ifndef RACECAR_H_
#define RACECAR_H_

#include "cocos2d.h"
#include <vector>
#include "Box2D.h"
#include "FixtureUserData.h"

class CarTire;

class RaceCar : FixtureUserData {
public:
	RaceCar(const char* carFileName, float x, float y, b2World* world);
	virtual ~RaceCar();

	void setPosition(float x, float y);
	b2Vec2 getPosition();
	float getAngle();
	void updateCarAngle();
	void updateCarPosition();
	void update(int controlState);

private:
	b2Body* m_body;
	std::vector<CarTire*> m_tires;
	b2RevoluteJoint *m_flJoint, *m_frJoint;
};

#endif /* RACECAR_H_ */
