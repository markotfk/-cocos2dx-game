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
enum class FixtureUserDataType {
    CAR_TIRE,
    CAR,
    GROUND_AREA,
    TRACK_WALL
};

class FixtureUserData {
public:
	FixtureUserData(FixtureUserDataType type) : m_type(type), m_sprite(nullptr) {}

	virtual ~FixtureUserData() { CC_SAFE_RELEASE_NULL(m_sprite); }

	inline const FixtureUserDataType getType() const { return m_type; }
	inline cocos2d::Sprite* getSprite() const { return m_sprite; }
protected:
	FixtureUserDataType m_type;

	cocos2d::Sprite *m_sprite;
};


//class to allow marking a fixture as a ground area
class GroundAreaFUD : public FixtureUserData {
public:
    float frictionModifier;
    float dragModifier;

    GroundAreaFUD(float friction, float drag, float posX, float posY) : FixtureUserData(FixtureUserDataType::GROUND_AREA) {
        frictionModifier = friction;
        dragModifier = drag;
        m_sprite = cocos2d::Sprite::create("grass20.png");
        m_sprite->setPosition(cocos2d::Point(posX, posY));
    }
};

class TrackWallFUD : public FixtureUserData
{
	public: TrackWallFUD() :   FixtureUserData(FixtureUserDataType::TRACK_WALL) {}
};

#endif /* FIXTUREUSERDATA_H_ */
