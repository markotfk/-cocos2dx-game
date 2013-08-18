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
#include "Constants.h"
#include "ContactListener.h"

class RaceCar;
class DebugDraw;

class TrackScene: public cocos2d::LayerColor {
public:
	static cocos2d::Scene* scene();

	TrackScene();
	virtual ~TrackScene();

	// implement the "static node()" method manually
	CREATE_FUNC(TrackScene);
	virtual bool init();
	virtual void menuCloseCallback(cocos2d::Object* pSender);

	void updateGame(float dt);

	void addTarget();
	void spriteMoveFinished(cocos2d::Node* sender);
	void registerWithTouchDispatcher();
	void keyPressed(int keyCode);
	void keyReleased(int keyCode);
	void draw();
private:
	void addSpritesFromB2World();

private:
	RaceCar *m_car;
	DebugDraw *m_debugDraw;
	b2World* m_world;
	b2Body* m_groundBody;
	DestructionListener m_destructionListener;
	ContactListener *m_contactListener;
	int m_controlState;
};

#endif /* TRACKSCENE_H_ */

