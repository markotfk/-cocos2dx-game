/*
 * Constants.h
 *
 *  Created on: Aug 13, 2013
 *      Author: Marko Karjalainen
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_


const float DEGTORAD = 0.0174532925199432957f;
const float RADTODEG = 57.295779513082320876f;

enum CarControls {
	NONE = 	0x0,
    LEFT = 	0x1,
    RIGHT = 0x2,
    UP = 	0x4,
    DOWN = 	0x8,
};

enum
{
   kFilterCategoryLevel = 0x01,
   kFilterCategorySolidObject = 0x02,
   kFilterCategoryNonSolidObject = 0x04
};

const float LOCK_ANGLE = 45 * DEGTORAD;
const float TURNSPEED_PER_SEC = 320 * DEGTORAD;//from lock to lock in 0.25 sec
const float TURN_PER_TIME_STEP = TURNSPEED_PER_SEC / 60.0f;
const float PTM = 32.0f;

#endif /* CONSTANTS_H_ */
