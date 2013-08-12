/*
 * ContactListener.h
 *
 *  Created on: Aug 12, 2013
 *      Author: Marko Karjalainen
 */

#include "ContactListener.h"

ContactListener::ContactListener()
{
}

ContactListener::~ContactListener()
{
}

void ContactListener::BeginContact(b2Contact* contact)
{
    RallyContact rallyContact = { contact->GetFixtureA(), contact->GetFixtureB() };
    m_contacts.push_back(rallyContact);
}

void ContactListener::EndContact(b2Contact* contact)
{
	RallyContact rallyContact = { contact->GetFixtureA(), contact->GetFixtureB() };
    std::vector<RallyContact>::iterator pos;
    pos = std::find(m_contacts.begin(), m_contacts.end(), rallyContact);
    if (pos != m_contacts.end())
    {
    	m_contacts.erase(pos);
    }
}

void ContactListener::PreSolve(b2Contact* contact,
                                 const b2Manifold* oldManifold)
{
}

void ContactListener::PostSolve(b2Contact* contact,
                                  const b2ContactImpulse* impulse)
{
}
