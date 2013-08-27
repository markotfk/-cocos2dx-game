/*
 * GroundAreaFUD.cpp
 *
 *  Created on: Aug 18, 2013
 *      Author: Marko Karjalainen
 */
 
#include "GroundAreaFUD.h"

GroundAreaFUD::GroundAreaFUD(float friction, float drag, float x, float y) :
	FixtureUserData(nullptr, FixtureUserDataType::GROUND_AREA)
	{
	    frictionModifier = friction;
	    dragModifier = drag;
	    if (m_sprite != nullptr)
	    {
	    	m_sprite->setPosition(cocos2d::Point(x, y));
	    }
    }

GroundAreaFUD::~GroundAreaFUD()
{
}
