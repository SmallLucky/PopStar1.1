#include "AppDelegate.h"
#include "WelcomeScene.h"
#include "Audio.h"
#include "Constant.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 800);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 800);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(768, 1024);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1536, 2048);

static cocos2d::Size mydesignResolutionSize = cocos2d::Size(480, 800);

float DESIGN_RATIO = 0.59999;  //  800/480 = 0.59999

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{

}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

	GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
	return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif
	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
		glview = GLViewImpl::create("My Game");
		director->setOpenGLView(glview);
	}

	Size size = Director::getInstance()->getWinSize();

	float ratio = size.width / size.height;

	if (ratio >= DESIGN_RATIO)
	{
		float height = mydesignResolutionSize.height;
		float width = height * ratio;
		glview->setDesignResolutionSize(width, height, ResolutionPolicy::EXACT_FIT);
	}
	else
	{
		float width = mydesignResolutionSize.width;
		float height = width / ratio;
		glview->setDesignResolutionSize(width, height, ResolutionPolicy::EXACT_FIT);
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	glview->setFrameSize(480, 800);	
	//glview->setFrameSize(640, 960);
	//glview->setFrameSize(768, 1024);

#endif

	//director->setDisplayStats(true);

	director->setAnimationInterval(1.0 / 60);

	Size frameSize = glview->getFrameSize();
	
	int id = UserDefault::getInstance()->getIntegerForKey(LOGINREWARD, 1);	//登录奖励
	if (id == 1)
	{
		UserDefault::getInstance()->setIntegerForKey(LOGINREWARD, 1);
		UserDefault::getInstance()->flush();
	}
	/*
	int gameRewardId = UserDefault::getInstance()->getIntegerForKey(GAMEREWARD, 6);	//过关奖励
	if (gameRewardId == 6)
	{
		UserDefault::getInstance()->setIntegerForKey(GAMEREWARD, 6);
		UserDefault::getInstance()->flush();
	}
	*/

	int reward_one = UserDefault::getInstance()->getIntegerForKey(REWARD_LEVEL_ONE, 1);	//第一关过关奖励
	if (reward_one == 1)
	{
		UserDefault::getInstance()->setIntegerForKey(REWARD_LEVEL_ONE, 1);
		UserDefault::getInstance()->flush();
	}

	int reward_two = UserDefault::getInstance()->getIntegerForKey(REWARD_LEVEL_TWO, 1);	//第二关过关奖励
	if (reward_two == 1)
	{
		UserDefault::getInstance()->setIntegerForKey(REWARD_LEVEL_TWO, 1);
		UserDefault::getInstance()->flush();
	}

	int reward_two_fail = UserDefault::getInstance()->getIntegerForKey(REWARD_LEVEL_TWO_FAIL, 1);	//第二关失败奖励
	if (reward_two_fail == 1)
	{
		UserDefault::getInstance()->setIntegerForKey(REWARD_LEVEL_TWO_FAIL, 1);	
		UserDefault::getInstance()->flush();
	}

	if (UserDefault::getInstance()->getIntegerForKey(MYDAY, 1) == 1)	//时间
	{
		UserDefault::getInstance()->setIntegerForKey(MYDAY, 1);
	}

	UserDefault::getInstance()->setBoolForKey("ISMUSICON", true);

	//auto scene = WelcomeScene::createScene();

	SpriteFrameCache * cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("Flower.plist");

	auto scene = GameScene::createScene();
	Audio::getInstance()->prepare();
	// run
	director->runWithScene(scene);

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

	// if you use SimpleAudioEngine, it must be pause
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

	// if you use SimpleAudioEngine, it must resume here
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
