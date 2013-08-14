/*
 * Constants.h
 *
 *  Created on: Aug 13, 2013
 *      Author: Marko Karjalainen
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_


#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

enum class CarControls {
	NONE = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

const float PTM = 32.0f;

#endif /* CONSTANTS_H_ */
