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
	FixtureUserData(const char *fileName, FixtureUserDataType type);

	virtual ~FixtureUserData();

	inline const FixtureUserDataType getType() const { return m_type; }
	inline cocos2d::Sprite* getSprite() const { return m_sprite; }
protected:
	FixtureUserDataType m_type;
	cocos2d::Sprite *m_sprite;
};


class TrackWallFUD : public FixtureUserData
{
	public: TrackWallFUD() :   FixtureUserData(nullptr, FixtureUserDataType::TRACK_WALL) {}
};

#endif /* FIXTUREUSERDATA_H_ */
