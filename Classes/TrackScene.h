/*
 * TrackScene.h
 *
 *  Created on: Aug 5, 2013
 *      Author: Marko Karjalainen
 */

#ifndef TRACKSCENE_H_
#define TRACKSCENE_H_
#include "Box2D.h"
#include "cocos2d.h"
#include "DestructionListener.h"
#include "ContactListener.h"

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
	void ccTouchesCancelled(cocos2d::Set *touches, cocos2d::Event *event);
	void ccTouchesMoved(cocos2d::Set *touches, cocos2d::Event *event);
	void ccTouchesBegan(cocos2d::Set *touches, cocos2d::CCEvent *event);
private:
	void addSpritesFromB2World();

private:
	RaceCar *m_car;

	b2World* m_world;
	b2Body* m_groundBody;
	DestructionListener m_destructionListener;
	ContactListener *m_contactListener;
	int m_controlState;
};

#endif /* TRACKSCENE_H_ */

