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
#include <Box2D.h>

class CarTire;

class RaceCar {
public:
	RaceCar(const char* carFileName, b2World* world);
	virtual ~RaceCar();

	cocos2d::CCSprite *getSprite();
	void update(int controlState);

private:
	cocos2d::CCSprite *m_sprite;
	b2Body* m_body;
	std::vector<CarTire*> m_tires;
	b2RevoluteJoint *flJoint, *frJoint;
};

#endif /* RACECAR_H_ */
