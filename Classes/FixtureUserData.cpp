/*
 * FixtureUserData.cpp
 *
 *  Created on: Aug 18, 2013
 *      Author: Marko Karjalainen
 */

#include "cocos2d.h"
#include "FixtureUserData.h"

FixtureUserData::FixtureUserData(const char *fileName, FixtureUserDataType type)
	: m_type(type), m_sprite(nullptr)
{
	if (fileName != nullptr)
	{
		m_sprite = cocos2d::Sprite::create(fileName);
	}
}

FixtureUserData::~FixtureUserData()
{
}
