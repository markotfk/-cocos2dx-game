/*
 * Box2DDestructionListener.h
 *
 *  Created on: Aug 10, 2013
 *      Author: Marko Karjalainen
 */

#ifndef BOX2DDESTRUCTIONLISTENER_H_
#define BOX2DDESTRUCTIONLISTENER_H_

#include <Box2D.h>
#include "FixtureUserData.h"

class DestructionListener :  public b2DestructionListener{
	void SayGoodbye(b2Fixture* fixture)
	    {
	        if ( FixtureUserData* fud = (FixtureUserData*)fixture->GetUserData() )
	            delete fud;
	    }

	//(unused but must implement all pure virtual functions)
	void SayGoodbye(b2Joint* joint) {}
};

#endif /* BOX2DDESTRUCTIONLISTENER_H_ */
