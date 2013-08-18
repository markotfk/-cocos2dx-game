/*
 * GroundAreaFUD.h
 *
 *  Created on: Aug 18, 2013
 *      Author: Marko Karjalainen
 */

#ifndef GROUNDAREAFUD_H_
#define GROUNDAREAFUD_H_

#include "FixtureUserData.h"
#include "cocos2d.h"

class GroundAreaFUD : public FixtureUserData
{
	public:
		GroundAreaFUD(float friction, float drag, float x, float y);
		virtual ~GroundAreaFUD();

		float frictionModifier;
		float dragModifier;
};

#endif /* GROUNDAREAFUD_H_ */
