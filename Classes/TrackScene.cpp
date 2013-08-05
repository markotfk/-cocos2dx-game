/*
 * TrackScene.cpp
 *
 *  Created on: Aug 5, 2013
 *      Author: mtfk
 */

#include "SimpleAudioEngine.h"

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

TrackScene::TrackScene() {


}

TrackScene::~TrackScene() {

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

		// Add the menu as a child layer.
		addChild(pMenu, 1);

		CCSprite *player = CCSprite::create("car.png", CCRectMake(0, 0, 75, 30) );

		player->setPosition( ccp(origin.x + visibleSize.width/2,
								 origin.y + visibleSize.height/2) );
		addChild(player);

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
}

void TrackScene::updateGame(float dt)
{

}