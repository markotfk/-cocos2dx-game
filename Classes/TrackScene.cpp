/*
 * TrackScene.cpp
 *
 *  Created on: Aug 5, 2013
 *      Author: mtfk
 */
#include <math.h>
#include "SimpleAudioEngine.h"
#include "RaceCar.h"
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

TrackScene::TrackScene() : m_car(NULL), accelerate(false),
		m_world(NULL), m_groundBody(NULL)
{
	b2Vec2 gravity(0.0f, 0.0f);
	m_world = new b2World(gravity);
	m_world->SetDestructionListener(&m_destructionListener);
}

TrackScene::~TrackScene()
{
	delete m_world;
}

/// Initialize the track scene
bool TrackScene::init()
{
	bool retVal = false;
	do {
		// super init first
		CC_BREAK_IF(!CCLayerColor::initWithColor( ccc4(210,210,210,210), 1024, 768 ) );

		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		// Initialize the race car
		float carPosX = origin.x + visibleSize.width/2;
		float carPosY = origin.y + visibleSize.height/2;

		m_car = new RaceCar("car.png", carPosX,	carPosY, m_world);

		// enable touch
		setTouchEnabled(true);

		// Box2D stuff
		//set up ground areas
		b2BodyDef groundDef;
		groundDef.position.Set(0.0f, 0.0f);
		m_groundBody = m_world->CreateBody(&groundDef);

		b2PolygonShape polygonShape;
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &polygonShape;
		fixtureDef.isSensor = true;

		polygonShape.SetAsBox( 10, 10, b2Vec2(-10,15), 20*DEGTORAD );
		b2Fixture* groundAreaFixture = m_groundBody->CreateFixture(&fixtureDef);
		groundAreaFixture->SetUserData( new GroundAreaFUD( 0.5f, false, carPosX, carPosY ) );

		polygonShape.SetAsBox( 9, 5, b2Vec2(5,20), -40*DEGTORAD );
		groundAreaFixture = m_groundBody->CreateFixture(&fixtureDef);
		groundAreaFixture->SetUserData( new GroundAreaFUD( 0.2f, false, carPosX, carPosY ) );

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
	if (accelerate)
	{
		m_car->update(TDC_UP);
	}
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
			if (sprite != NULL)
			{
				b2Vec2 point = body->GetPosition();
				sprite->setPosition(Point(point.x, point.y));
				sprite->setRotation(body->GetAngle()*RADTODEG);
			}
			// Go through body fixtures
			for (b2Fixture *fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
			{
				// Get user data from fixture
				FixtureUserData *user = static_cast<FixtureUserData*>(fixture->GetUserData());
				if (user != NULL)
				{
					CCSprite *sprite = user->getSprite();
					if (sprite != NULL)
					{
						const b2Vec2 point = body->GetPosition();
						sprite->setPosition(Point(point.x, point.y));
						sprite->setRotation(body->GetAngle()*RADTODEG);
					}
				}
			}
		}
	}
	b2Vec2 position = m_car->getPosition();
	float angle = m_car->getAngle();
	//printf("position: x: %f, y:%f\n", position.x, position.y);
	//printf("angle:%f\n", angle);
}

void TrackScene::ccTouchesBegan(Set *touches, CCEvent *event)
{
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocation();

	accelerate = true;
}

void TrackScene::ccTouchesMoved(cocos2d::Set *touches, cocos2d::Event *event)
{
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocation();
}

void TrackScene::ccTouchesEnded(Set* touches, CCEvent* event)
{
	accelerate = false;
}

void TrackScene::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}


