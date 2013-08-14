/*
 * RaceCar.h
 *
 *  Created on: Aug 7, 2013
 *      Author: Marko Karjalainen
 */

#ifndef RACECAR_H_
#define RACECAR_H_

#include "cocos2d.h"
#include <vector>
#include "Box2D.h"

#include "FixtureUserData.h"

class CarTire;
enum class CarControls;

class RaceCar : public FixtureUserData {
public:
	RaceCar(const char* carFileName, float x, float y, b2World* world);
	virtual ~RaceCar();
	void updateCarAngle();
	void updateCarPosition();
	void update(CarControls controlState);

	inline b2Vec2 getPosition() const { return m_body->GetPosition(); }
	inline float getAngle() const { return m_body->GetAngle(); }
	inline b2Body *getBody() { return m_body; };

private:
	b2Body* m_body;
	std::vector<CarTire*> m_tires;
	b2RevoluteJoint *m_flJoint, *m_frJoint;
};

#endif /* RACECAR_H_ */
