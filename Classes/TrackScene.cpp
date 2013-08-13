/*
 * TrackScene.cpp
 *
 *  Created on: Aug 5, 2013
 *      Author: Marko Karjalainen
 */
#include <math.h>
#include "SimpleAudioEngine.h"
#include "RaceCar.h"
#include "ContactListener.h"
#include "Constants.h"
#include "TrackScene.h"

using namespace cocos2d;

cocos2d::CCScene* TrackScene::scene()
{
	CCScene * scene = NULL;
	do
	{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);
		TrackScene *layer = TrackScene::create();
		CC_BREAK_IF(!layer);
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

TrackScene::TrackScene() : m_car(NULL), m_world(NULL),
		m_groundBody(NULL), m_controlState(0)
{
	const b2Vec2 gravity(0.0f, 0.0f);
	m_world = new b2World(gravity);
	m_world->SetDestructionListener(&m_destructionListener);
	m_contactListener = new ContactListener();
	m_world->SetContactListener(m_contactListener);
}

TrackScene::~TrackScene()
{
	delete m_world;
	delete m_contactListener;
}

/// Initialize the track scene
bool TrackScene::init()
{
	bool retVal = false;
	do {
		// super init first
		CC_BREAK_IF(!CCLayerColor::initWithColor( ccc4(225,225,225,225), 1024, 768 ) );

		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		// Initialize the race car
		float carPosX = origin.x + visibleSize.width/2;
		float carPosY = origin.y + visibleSize.height/2;

		m_car = new RaceCar("car.png", carPosX,	carPosY, m_world);

		// enable touch
		setTouchEnabled(true);

		/*// Box2D stuff
		//set up ground areas
		b2BodyDef groundDef;
		groundDef.position.Set(carPosX/PTM, carPosY/PTM);
		m_groundBody = m_world->CreateBody(&groundDef);

		b2PolygonShape polygonShape;
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &polygonShape;
		fixtureDef.isSensor = true;

		polygonShape.SetAsBox( 10, 10, b2Vec2(carPosX/PTM,carPosY/PTM), 20*DEGTORAD );
		b2Fixture* groundAreaFixture = m_groundBody->CreateFixture(&fixtureDef);
		groundAreaFixture->SetUserData( new GroundAreaFUD( 0.5f, false, carPosX/PTM, carPosY/PTM ) );

		polygonShape.SetAsBox( 190, 255, b2Vec2(carPosX, carPosY), -40*DEGTORAD );
		groundAreaFixture = m_groundBody->CreateFixture(&fixtureDef);
		groundAreaFixture->SetUserData( new GroundAreaFUD( 0.2f, false, carPosX, carPosY ) );*/

		// schedule updateGame to be called for every frame
		schedule( schedule_selector(TrackScene::updateGame) );

		// Add sprites to scene
		addSpritesFromB2World();
		// music disabled for now
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);
		retVal = true;
		} while(0);

		return retVal;
}

void TrackScene::addSpritesFromB2World()
{
	for (b2Body* body = m_world->GetBodyList(); body; body = body->GetNext())
	{
		if (body->GetUserData() != NULL)
		{
			FixtureUserData *user = static_cast<FixtureUserData*>(body->GetUserData());
			CCSprite *sprite = user->getSprite();
			if (sprite != NULL)
			{
				addChild(sprite);
			}

		}
		for (b2Fixture *fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
		{
			FixtureUserData *user = static_cast<FixtureUserData*>(fixture->GetUserData());
			if (user != NULL)
			{
				CCSprite *sprite = user->getSprite();
				if (sprite != NULL)
				{
					addChild(sprite);
				}
			}
		}
	}
}

void TrackScene::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();
	exit(0);
}

void TrackScene::updateGame(float dt)
{
	// TODO: keyboard event handling
	m_car->update(m_controlState);
	// Instruct the world to perform a single step of simulation.
	m_world->Step(dt, 6, 2);

	// Go through the bodies in world and update sprites
	for (b2Body* body = m_world->GetBodyList(); body; body = body->GetNext())
	{
		if (body->GetUserData() != NULL)
		{
			// Get user data from body
			FixtureUserData *user = static_cast<FixtureUserData*>(body->GetUserData());
			CCSprite *sprite = user->getSprite();
			if (sprite != NULL && user->getType() == FUD_CAR)
			{
				RaceCar* car = static_cast<RaceCar*>(user);
				car->updateCarAngle();
				car->updateCarPosition();
			}
			// Go through body fixtures
			for (b2Fixture *fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
			{
				// Get user data from fixture
				FixtureUserData *user = static_cast<FixtureUserData*>(fixture->GetUserData());
				if (user != NULL && user->getType() == FUD_CAR)
				{
					RaceCar* car = static_cast<RaceCar*>(user);
					car->updateCarAngle();
					car->updateCarPosition();
				}
			}
		}
	}
	/*b2Vec2 position = m_car->getPosition();
	float angle = m_car->getAngle();
	printf("car position: x: %f, y:%f\n", position.x, position.y);
	printf("car angle:%f\n", angle);*/


	// Go through contacts
	/*if (m_contactListener->m_contacts.size() > 0)
	{
		printf("Amount of contacts: %u\n", (unsigned int)m_contactListener->m_contacts.size());
	}

	for(auto pos = m_contactListener->m_contacts.begin();pos != m_contactListener->m_contacts.end(); ++pos) {
		RallyContact contact = *pos;

		b2Body *bodyA = contact.fixtureA->GetBody();
		b2Body *bodyB = contact.fixtureB->GetBody();
		if (bodyA->GetUserData() != NULL && bodyB->GetUserData() != NULL) {
			FixtureUserData *dataA = static_cast<FixtureUserData*>(bodyA->GetUserData());
			FixtureUserData *dataB = static_cast<FixtureUserData*>(bodyB->GetUserData());

		}
	}*/


}

void TrackScene::ccTouchesBegan(Set *touches, CCEvent *event)
{
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());

	float deltaX = location.x - m_car->getPosition().x;
	float deltaY = -(location.y - m_car->getPosition().y);
	float rads = atan2(deltaY, deltaX);
	m_controlState = TDC_UP;
}

void TrackScene::ccTouchesMoved(cocos2d::Set *touches, cocos2d::Event *event)
{
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());

	float deltaX = location.x - m_car->getPosition().x;
	float deltaY = -(location.y - m_car->getPosition().y);
	float rads = atan2(deltaY, deltaX);
}

void TrackScene::ccTouchesCancelled(Set* touches, CCEvent* event)
{
	m_controlState = 0;
}

void TrackScene::ccTouchesEnded(Set* touches, CCEvent* event)
{
	m_controlState = TDC_DOWN;
}

void TrackScene::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}


