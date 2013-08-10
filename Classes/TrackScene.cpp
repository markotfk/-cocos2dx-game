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

#define MAX_SPEED 0.1

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

TrackScene::TrackScene() : m_car(NULL), m_world(NULL), m_groundBody(NULL),
		m_controls(0) {
	accelerate = false;
	speed = 0.0;
	b2Vec2 gravity(0, -9.8); //normal earth gravity
	m_world = new b2World(gravity);
	m_world->SetDestructionListener(&m_destructionListener);

	//set up ground areas
	{
		b2BodyDef bodyDef;
		m_groundBody = m_world->CreateBody(&bodyDef);

		b2PolygonShape polygonShape;
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &polygonShape;
		fixtureDef.isSensor = true;

		polygonShape.SetAsBox( 9, 7, b2Vec2(-10,15), 20*DEGTORAD );
		b2Fixture* groundAreaFixture = m_groundBody->CreateFixture(&fixtureDef);
		groundAreaFixture->SetUserData( new GroundAreaFUD( 0.5f, false ) );

		polygonShape.SetAsBox( 9, 5, b2Vec2(5,20), -40*DEGTORAD );
		groundAreaFixture = m_groundBody->CreateFixture(&fixtureDef);
		groundAreaFixture->SetUserData( new GroundAreaFUD( 0.2f, false ) );
	}
}

TrackScene::~TrackScene()
{
}

/// Initialize the track scene
bool TrackScene::init()
{
	bool retVal = false;
	do {
		// super init first
		CC_BREAK_IF(!CCLayerColor::initWithColor( ccc4(210,210,210,210) ) );

		// Add a menu item image to quit program
		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			this,
			menu_selector(TrackScene::menuCloseCallback));
		CC_BREAK_IF(!pCloseItem);
		// Place the menu item
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
		pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2,
									origin.y + pCloseItem->getContentSize().height/2));
		// Create a menu
		CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
		CC_BREAK_IF(! pMenu);
		pMenu->setPosition(CCPointZero);

		setTouchEnabled(true);

		// Add the menu as a child layer.
		addChild(pMenu, 1);

		m_car = new RaceCar("car.png", m_world);
		CC_BREAK_IF(! m_car );

		m_car->getSprite()->setPosition( ccp(origin.x + visibleSize.width/2,
								 origin.y + visibleSize.height/2) );
		addChild(m_car->getSprite());

		// schedule updateGame to be called for every frame
		schedule( schedule_selector(TrackScene::updateGame) );

		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);
		retVal = true;
		} while(0);

		return retVal;
}

void TrackScene::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();
	exit(0);
}

void TrackScene::updateGame(float dt)
{
	if (accelerate)
	{
		speed += 0.01;
		if (speed > MAX_SPEED)
		{
			speed = MAX_SPEED;
		}
	}
	else {
		speed -= 0.01;
		if (speed < 0.0)
		{
			speed = 0.0;
		}
	}
	if (speed > 0.0)
	{
		float deltaX = target.x - m_car->getSprite()->getPositionX();
		float deltaY = -(target.y - m_car->getSprite()->getPositionY());
		float rads = atan2(deltaY, deltaX);
		m_car->getSprite()->setPositionX(m_car->getSprite()->getPositionX() + deltaX*speed);
		m_car->getSprite()->setPositionY(m_car->getSprite()->getPositionY() + -(deltaY*speed));
	}
}


void TrackScene::spriteMoveFinished(CCNode* sender)
{
}

void TrackScene::ccTouchesBegan(Set *touches, CCEvent *event)
{
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocation();
	accelerate = true;
	target = location;

	float deltaX = location.x - m_car->getSprite()->getPosition().x;
	float deltaY = -(location.y - m_car->getSprite()->getPosition().y);
	float rads = atan2(deltaY, deltaX);

	m_car->getSprite()->stopAllActions();
	m_car->getSprite()->runAction( CCSequence::create(
				CCRotateTo::create(0.1, CC_RADIANS_TO_DEGREES(rads)),
				CCCallFuncN::create(this,
		                            callfuncN_selector(TrackScene::spriteMoveFinished)),
		        NULL) );
}

void TrackScene::ccTouchesMoved(cocos2d::Set *touches, cocos2d::Event *event)
{
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocation();
	float deltaX = location.x - m_car->getSprite()->getPosition().x;
	float deltaY = -(location.y - m_car->getSprite()->getPosition().y);
	float rads = atan2(deltaY, deltaX);

	target = location;
	m_car->getSprite()->stopAllActions();
	m_car->getSprite()->runAction( CCSequence::create(
					CCRotateTo::create(0.1, CC_RADIANS_TO_DEGREES(rads)),
					CCCallFuncN::create(this,
			                            callfuncN_selector(TrackScene::spriteMoveFinished)),
			        NULL) );
}

void TrackScene::ccTouchesEnded(Set* touches, CCEvent* event)
{
	accelerate = false;
}

void TrackScene::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}


