/*
 * TrackScene.h
 *
 *  Created on: Aug 5, 2013
 *      Author: mtfk
 */

#ifndef TRACKSCENE_H_
#define TRACKSCENE_H_

#include "cocos2d.h"

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
	cocos2d::CCSprite *player;
};

#endif /* TRACKSCENE_H_ */

