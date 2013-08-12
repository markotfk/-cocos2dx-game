/*
 * ContactListener.h
 *
 *  Created on: Aug 12, 2013
 *      Author: Marko Karjalainen
 */

#ifndef CONTACTLISTENER_H_
#define CONTACTLISTENER_H_

#include <Box2D.h>
#include <vector>

struct RallyContact {
    b2Fixture *fixtureA;
    b2Fixture *fixtureB;
    bool operator==(const RallyContact& other) const
    {
        return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
    }
};
class ContactListener :  public b2ContactListener {

public:
    ContactListener();
    ~ContactListener();
    
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    
    std::vector<RallyContact> m_contacts;
};

#endif /* CONTACTLISTENER_H_ */
