/*
 * FixtureUserData.h
 *
 *  Created on: Aug 10, 2013
 *      Author: Marko Karjalainen
 */

#ifndef FIXTUREUSERDATA_H_
#define FIXTUREUSERDATA_H_
#include "cocos2d.h"

//types of fixture user data
enum fixtureUserDataType {
    FUD_CAR_TIRE,
    FUD_CAR,
    FUD_GROUND_AREA
};

class FixtureUserData {
public:
	FixtureUserData(fixtureUserDataType type) : m_type(type), m_sprite(nullptr) {}

	virtual ~FixtureUserData() {}

	inline const fixtureUserDataType getType() const { return m_type; }
	inline cocos2d::Sprite* getSprite() const { return m_sprite; }
protected:
	fixtureUserDataType m_type;

	cocos2d::Sprite *m_sprite;
};


//class to allow marking a fixture as a ground area
class GroundAreaFUD : public FixtureUserData {
public:
    float frictionModifier;
    bool outOfCourse;

    GroundAreaFUD(float friction, bool outOfCourse, float posX, float posY) : FixtureUserData(FUD_GROUND_AREA) {
        frictionModifier = friction;
        this->outOfCourse = outOfCourse;
        m_sprite = cocos2d::Sprite::create("grass20.png");
        m_sprite->setPosition(cocos2d::Point(posX, posY));
    }
};

#endif /* FIXTUREUSERDATA_H_ */
