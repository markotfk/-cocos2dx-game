#include "SimpleAudioEngine.h"
#include "AppDelegate.h"
#include "TrackScene.h"

USING_NS_CC;


AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::sharedDirector();
    
    director->setOpenGLView(EGLView::sharedOpenGLView());
    
    Size screenSize = EGLView::sharedOpenGLView()->getFrameSize();
    Size designSize = CCSizeMake(1024, 768);
    
    EGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionNoBorder);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create and run a scene. it's an autorelease object
    director->runWithScene(TrackScene::scene());

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::sharedDirector()->stopAnimation();

    //CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::sharedDirector()->startAnimation();

    //CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
