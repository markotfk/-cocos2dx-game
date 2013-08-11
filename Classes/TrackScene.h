/*
 * TrackScene.h
 *
 *  Created on: Aug 5, 2013
 *      Author: mtfk
 */

#ifndef TRACKSCENE_H_
#define TRACKSCENE_H_
#include "Box2D.h"
#include "cocos2d.h"
#include "Box2DDestructionListener.h"

#ifndef DEGTORAD
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#endif

enum {
    TDC_LEFT     = 0x1,
    TDC_RIGHT    = 0x2,
    TDC_UP       = 0x4,
    TDC_DOWN     = 0x8
};

class RaceCar;

class TrackScene: public cocos2d::CCLayerColor {
public:
	static cocos2d::CCScene* scene();

	TrackScene();
	virtual ~TrackScene();

	// implement the "static node()" method manually
	CREATE_FUNC(TrackScene);
	virtual bool init();
	virtual void menuCloseCallback(cocos2d::CCObject* pSender);

	void updateGame(float dt);

	void addTarget();
	void spriteMoveFinished(cocos2d::CCNode* sender);
	void registerWithTouchDispatcher();
	void ccTouchesEnded(cocos2d::Set* touches, cocos2d::CCEvent* event);
	void ccTouchesMoved(cocos2d::Set *touches, cocos2d::Event *event);
	void ccTouchesBegan(cocos2d::Set *touches, cocos2d::CCEvent *event);
private:
	void addSpritesFromB2World();

private:
	RaceCar *m_car;

	bool accelerate;
	b2World* m_world;
	b2Body* m_groundBody;
	Box2DDestructionListener m_destructionListener;
};

#endif /* TRACKSCENE_H_ */

