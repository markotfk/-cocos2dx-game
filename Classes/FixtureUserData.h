/*
 * FixtureUserData.h
 *
 *  Created on: Aug 10, 2013
 *      Author: mtfk
 */

#ifndef FIXTUREUSERDATA_H_
#define FIXTUREUSERDATA_H_
//types of fixture user data
enum fixtureUserDataType {
    FUD_CAR_TIRE,
    FUD_GROUND_AREA
};

class FixtureUserData {
public:
	fixtureUserDataType m_type;
	FixtureUserData(fixtureUserDataType type) : m_type(type) {}

	virtual ~FixtureUserData() {}

	virtual fixtureUserDataType getType() { return m_type; }
};

//class to allow marking a fixture as a car tire
class CarTireFUD : public FixtureUserData {
public:
    CarTireFUD() : FixtureUserData(FUD_CAR_TIRE) {}
};

//class to allow marking a fixture as a ground area
class GroundAreaFUD : public FixtureUserData {
public:
    float frictionModifier;
    bool outOfCourse;

    GroundAreaFUD(float fm, bool ooc) : FixtureUserData(FUD_GROUND_AREA) {
        frictionModifier = fm;
        outOfCourse = ooc;
    }
};

#endif /* FIXTUREUSERDATA_H_ */
