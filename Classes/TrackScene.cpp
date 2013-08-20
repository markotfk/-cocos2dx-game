/*
 * TrackScene.cpp
 *
 *  Created on: Aug 5, 2013
 *      Author: Marko Karjalainen
 */
#include <math.h>
#include "SimpleAudioEngine.h"
#include "DebugDraw.h"
#include "RaceCar.h"
#include "ContactListener.h"
#include "Constants.h"
#include "GroundAreaFUD.h"
#include "TrackScene.h"

using namespace cocos2d;

cocos2d::Scene* TrackScene::scene()
{
	Scene * scene = nullptr;
	do
	{
		scene = Scene::create();
		CC_BREAK_IF(!scene);
		TrackScene *layer = TrackScene::create();
		CC_BREAK_IF(!layer);
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

TrackScene::TrackScene() : m_car(nullptr), m_world(nullptr),
		m_groundBody(nullptr), m_contactListener(nullptr), m_controlState(CarControls::NONE)
{
	const b2Vec2 gravity(0.0f, 0.0f);
	m_world = new b2World(gravity);
	m_world->SetContinuousPhysics(true);
	m_world->SetAutoClearForces(true);
	m_world->SetDestructionListener(&m_destructionListener);
	m_contactListener = new ContactListener();
	m_world->SetContactListener(m_contactListener);

	// Debug draw class (implement also draw())
	m_debugDraw = new DebugDraw(1);
	m_debugDraw->AppendFlags(b2Draw::e_shapeBit|b2Draw::e_jointBit|
			b2Draw::e_aabbBit|b2Draw::e_pairBit|b2Draw::e_centerOfMassBit);
	//m_world->SetDebugDraw(m_debugDraw);
}

TrackScene::~TrackScene()
{
	delete m_world;
	delete m_contactListener;
	delete m_debugDraw;
}

/// Initialize the track scene
bool TrackScene::init()
{
	bool retVal = false;
	do {
		// super init first
		CC_BREAK_IF(!LayerColor::initWithColor( ccc4(225,225,225,225), 1024, 768 ) );

		Size visibleSize = Director::sharedDirector()->getVisibleSize();
		Point origin = Director::sharedDirector()->getVisibleOrigin();

		// Initialize the race car
		float carPosX = origin.x + visibleSize.width/2;
		float carPosY = origin.y + visibleSize.height/2;

		m_car = new RaceCar("car.png", carPosX,	carPosY, m_world);

		// enable keyboard
		setKeyboardEnabled(true);

		// Box2D stuff

		// Add edges so you cannot drive over screen
		addEdges(visibleSize);

		/*//set up ground areas
		b2BodyDef groundDef;
		groundDef.position.Set(carPosX/PTM, carPosY/PTM);
		m_groundBody = m_world->CreateBody(&groundDef);

		b2PolygonShape polygonShape;
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &polygonShape;
		fixtureDef.isSensor = true;

		polygonShape.SetAsBox( 10/PTM, 10/PTM, b2Vec2(carPosX/PTM,carPosY/PTM), 20*DEGTORAD );
		b2Fixture* groundAreaFixture = m_groundBody->CreateFixture(&fixtureDef);
		groundAreaFixture->SetUserData( new GroundAreaFUD( 0.5f, 0.1f, carPosX/PTM, carPosY/PTM ) );

		polygonShape.SetAsBox( 10/PTM, 10/PTM, b2Vec2(carPosX/PTM, carPosY/PTM), -40*DEGTORAD );
		groundAreaFixture = m_groundBody->CreateFixture(&fixtureDef);
		groundAreaFixture->SetUserData( new GroundAreaFUD( 0.2f, 0.1f, carPosX, carPosY ) );*/

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

void TrackScene::addEdges(const Size& size)
{
	// Left edge
	{
		b2BodyDef edgeDef;
		edgeDef.fixedRotation = true;
		edgeDef.type = b2_dynamicBody;
		auto body = m_world->CreateBody(&edgeDef);
		b2PolygonShape edgePolygon;
		edgePolygon.SetAsBox(1.0f, size.height/PTM, b2Vec2(0,0), 0);
		b2FixtureDef edgeFixture;
		edgeFixture.shape = &edgePolygon;
		edgeFixture.restitution = 1.0f;
		edgeFixture.density = 1.0f;
		body->CreateFixture(&edgeFixture);
	}


	// Right edge
	{
		b2BodyDef edgeDef;
		edgeDef.fixedRotation = true;
		edgeDef.type = b2_dynamicBody;
		auto body = m_world->CreateBody(&edgeDef);
		b2PolygonShape edgePolygon;
		edgePolygon.SetAsBox(1.0f/PTM, size.height/PTM, b2Vec2(size.width/PTM, 0), 0);
		b2FixtureDef edgeFixture;
		edgeFixture.shape = &edgePolygon;
		edgeFixture.restitution = 1.0f;
		edgeFixture.density = 1.0f;
		body->CreateFixture(&edgeFixture);
	}

	// Top edge
	{
		b2BodyDef edgeDef;
		edgeDef.fixedRotation = true;
		edgeDef.type = b2_dynamicBody;
		auto body = m_world->CreateBody(&edgeDef);
		b2PolygonShape edgePolygon;
		edgePolygon.SetAsBox(size.width/PTM, 1.0f/PTM, b2Vec2(0, size.height/PTM), 0);
		b2FixtureDef edgeFixture;
		edgeFixture.shape = &edgePolygon;
		edgeFixture.restitution = 1.0f;
		edgeFixture.density = 1.0f;
		body->CreateFixture(&edgeFixture);
	}


	// Bottom edge
	{
		b2BodyDef edgeDef;
		edgeDef.fixedRotation = true;
		edgeDef.type = b2_dynamicBody;
		auto body = m_world->CreateBody(&edgeDef);
		b2PolygonShape edgePolygon;
		edgePolygon.SetAsBox(size.width/PTM, 1.0f/PTM, b2Vec2(0, 0), 0);
		b2FixtureDef edgeFixture;
		edgeFixture.shape = &edgePolygon;
		edgeFixture.restitution = 1.0f;
		edgeFixture.density = 1.0f;
		body->CreateFixture(&edgeFixture);
	}
}

void TrackScene::addSpritesFromB2World()
{
	for (auto body = m_world->GetBodyList(); body; body = body->GetNext())
	{
		if (body->GetUserData() != nullptr)
		{
			auto user = static_cast<FixtureUserData*>(body->GetUserData());
			auto sprite = user->getSprite();
			if (sprite != nullptr)
			{
				addChild(sprite);
			}

		}
		for (auto fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
		{
			FixtureUserData *user = static_cast<FixtureUserData*>(fixture->GetUserData());
			if (user != nullptr)
			{
				auto sprite = user->getSprite();
				if (sprite != nullptr)
				{
					addChild(sprite);
				}
			}
		}
	}
}

void TrackScene::exitApp()
{
	Director::sharedDirector()->end();
	exit(0);
}

/*void TrackScene::draw()
{
	CCLayer::draw();

	ccGLEnableVertexAttribs(cocos2d::kCCVertexAttribFlag_Position);
	kmGLPushMatrix();
	m_world->DrawDebugData();
	kmGLPopMatrix();
	CHECK_GL_ERROR_DEBUG();
}*/

void TrackScene::updateGame(float dt)
{
	if (m_paused)
	{
		dt = 0.0f; // paused
	}
	else
	{
		m_car->update(m_controlState);
	}

	// Instruct the world to perform a single step of simulation.
	m_world->Step(dt, 6, 2);

	// Go through the bodies in world and update sprites
	for (auto body = m_world->GetBodyList(); body && !m_paused; body = body->GetNext())
	{
		//printf("Body : angle %f x:%f y:%f\n", body->GetAngle(), body->GetPosition().x, body->GetPosition().y);
		if (body->GetUserData() != nullptr)
		{
			// Get user data from body
			auto userData = static_cast<FixtureUserData*>(body->GetUserData());
			auto sprite = userData->getSprite();
			if (sprite != nullptr)
			{
				const float x = body->GetPosition().x*PTM;
				const float y = body->GetPosition().y*PTM;
				sprite->setPosition(Point(x, y));
				sprite->setRotation(body->GetAngle()*RADTODEG);
			}
			// Go through body fixtures
			for (auto fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
			{
				// Get user data from fixture
				auto userFixture = static_cast<FixtureUserData*>(fixture->GetUserData());
				if (userFixture != nullptr)
				{
					auto spriteFixture = userFixture->getSprite();
					if (spriteFixture != nullptr)
					{
						const float x = body->GetPosition().x*PTM;
						const float y = body->GetPosition().y*PTM;
						spriteFixture->setPosition(Point(x, y));
						spriteFixture->setRotation(body->GetAngle()*RADTODEG);
					}

				}
			}
		}
	}
	/* Go through contacts
	if (m_contactListener->m_contacts.size() > 0)
	{
		printf("Amount of contacts: %u\n", (unsigned int)m_contactListener->m_contacts.size());
	}

	for(auto pos = m_contactListener->m_contacts.begin();pos != m_contactListener->m_contacts.end(); ++pos) {
		RallyContact contact = *pos;

		b2Body *bodyA = contact.fixtureA->GetBody();
		b2Body *bodyB = contact.fixtureB->GetBody();
		if (bodyA->GetUserData() != nullptr && bodyB->GetUserData() != nullptr) {
			FixtureUserData *dataA = static_cast<FixtureUserData*>(bodyA->GetUserData());
			FixtureUserData *dataB = static_cast<FixtureUserData*>(bodyB->GetUserData());

		}
	}*/


}

void TrackScene::keyPressed(int keyCode)
{
	switch (keyCode)
	{
		case KEYCODE_UP:
			m_controlState |= CarControls::UP;
			break;
		case KEYCODE_DOWN:
			m_controlState |= CarControls::DOWN;
			break;
		case KEYCODE_LEFT:
			m_controlState |= CarControls::LEFT;
			break;
		case KEYCODE_RIGHT:
			m_controlState |= CarControls::RIGHT;
			break;
		default:
			break;
	}
};

void TrackScene::keyReleased(int keyCode)
{
	switch (keyCode)
		{
		case KEYCODE_UP:
			m_controlState &= ~CarControls::UP;
			break;
		case KEYCODE_DOWN:
			m_controlState &= ~CarControls::DOWN;
			break;
		case KEYCODE_LEFT:
			m_controlState &= ~CarControls::LEFT;
			break;
		case KEYCODE_RIGHT:
			m_controlState &= ~CarControls::RIGHT;
			break;
		case 'P':
			m_paused = !m_paused;
			break;
		case 'Q':
			exitApp();
			break;
		default:
			break;
		}
};

void TrackScene::registerWithTouchDispatcher()
{
	Director::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}


