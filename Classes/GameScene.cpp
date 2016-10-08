#include "GameScene.h"
#include "GameData.h"
#include "DataSave.h"
#include "Constant.h"
#include "CommonFunction.h"
#include "LoginRewordLayer.h"
#include "Audio.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}

cocos2d::Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();

	scene->addChild(layer);

	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	GameData::getInstance()->praseJsonData();
	DataSava::getInstance()->praseJsonData();


	//int curDay = CommonFunction::getCurDay();
	//int saveDay = UserDefault::getInstance()->getIntegerForKey(MYDAY);

	//登录礼物
	int eventId = UserDefault::getInstance()->getIntegerForKey(LOGINREWARD, 1);

		if (eventId <=1)
		{
			auto delay = DelayTime::create(0);

			auto callfunc = CallFunc::create([=](){

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

				JniMethodInfo info;
				bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/PayService", "pay", "(I)V");
				if (ret)
				{
					jobject jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID, eventId);
					LOGD(">>>>>>eventId=%d", eventId);
				}
#endif
			});

			auto seq = Sequence::create(delay, callfunc, nullptr);
			this->runAction(seq);
		}

	addChild(GameLayer::create());

	Audio::getInstance()->playMainBGM();
	Audio::getInstance()->playBGM();

	return true;
}

void GameScene::onEnter()
{
	Layer::onEnter();
}

void GameScene::onExit()
{
	Layer::onExit();
}


