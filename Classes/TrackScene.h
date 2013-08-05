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

};

#endif /* TRACKSCENE_H_ */

