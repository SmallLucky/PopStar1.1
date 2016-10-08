#include "GameScene.h"
#include "UILayer.h"
#include "GameData.h"
#include "GameOverLayer.h"
#include "RewardLayer.h"
#include "DataSave.h"
#include "Constant.h"
#include "PauseLayer.h"
#include "ShopLayer.h"
#include "GameLayer.h"
#include "Audio.h"
#include "BuyDimaLayer.h"
#include "TouchSwallowLayer.h"
#include "GameOverPopLayer.h"
#include "WelcomeScene.h"
#include "LoginRewordLayer.h"

GameLayer::GameLayer() :
matrix(nullptr),
linkNum(nullptr),
_uilayer(nullptr),
_levelMsg(nullptr),
_targetScore(nullptr),
_swallowLayer(nullptr),
isNextLevel(true),
_girlSp(nullptr)
{
	//重置剩下的星星
	auto _listenerReset = EventListenerCustom::create(RESETSTARPOS, [=](EventCustom*event){
		resetLeftStarPos();
	});

	auto _listenerBoom = EventListenerCustom::create(ISBOOM, [=](EventCustom*event){
		runBoomPop();
	});
	auto _listenerChange = EventListenerCustom::create(ISCHANGE, [=](EventCustom*event){
		changeStarColor();
	});
	//
	auto _listenerPauseLayer = EventListenerCustom::create(PAUSELAYER, [=](EventCustom*event){
		addPauseLayer();
	});
	//ti
	auto _listenerAdLayer = EventListenerCustom::create(SHOPLAYER, [=](EventCustom*event){
		addShopLayer();
	});
	auto _listenerDiamLayer = EventListenerCustom::create(BUYDIAMLAYER, [=](EventCustom*event){
		addBuyDiamondLayer();
	});

	auto _listenerGameOverPopLayer = EventListenerCustom::create(GAMEOVERPOPLAYER, [=](EventCustom*event){
		gotoGameOverPopLayer();
	});

	auto _listenerWelScene = EventListenerCustom::create(WELCOMESCENE, [=](EventCustom*event){
		gotoWelclmeScene();
	});

	_eventDispatcher->addEventListenerWithFixedPriority(_listenerReset, 1);
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerBoom, 1);
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerChange, 1);
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerPauseLayer, 1);
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerAdLayer, 1);
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerDiamLayer, 1);
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerGameOverPopLayer, 1);
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerWelScene, 1);

	//键盘事件
	auto listenerkeyPad = EventListenerKeyboard::create();
	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(GameLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);

}

GameLayer::~GameLayer()
{
	
	_eventDispatcher->removeCustomEventListeners(RESETSTARPOS);
	_eventDispatcher->removeCustomEventListeners(ISBOOM);
	_eventDispatcher->removeCustomEventListeners(ISCHANGE);
	_eventDispatcher->removeCustomEventListeners(PAUSELAYER);
	_eventDispatcher->removeCustomEventListeners(SHOPLAYER);
	_eventDispatcher->removeCustomEventListeners(BUYDIAMLAYER);
	_eventDispatcher->removeCustomEventListeners(GAMEOVERPOPLAYER);
	_eventDispatcher->removeCustomEventListeners(WELCOMESCENE);
	
}

void GameLayer::onEnter()
{
	Layer::onEnter();

	//单点触摸
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//多点触摸
	/*auto lis = EventListenerTouchAllAtOnce::create();
	lis->onTouchesBegan = CC_CALLBACK_2(GameLayer::onTouchesBegan,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lis,this);*/

	showStarMatrix();
	floatLevelWord();
}

void GameLayer::onExit()
{
	Layer::onExit();
}

bool GameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//snow effect
	ParticleSystem* m_emitter1 = ParticleSystemQuad::create("particle/snow.plist");
	m_emitter1->retain();
	ParticleBatchNode *batch = ParticleBatchNode::createWithTexture(m_emitter1->getTexture());
	batch->addChild(m_emitter1);

	m_emitter1->setPosition(CommonFunction::getVisibleAchor(0.5, 1, this, Vec2(0, 0)));
	m_emitter1->setStartSize(5);
	addChild(batch, 12);
	m_emitter1->release();

	Audio::getInstance()->playReadyGo();
	//背景
	auto bg_Sp = Sprite::create("bg_mainscene_1.png");

	if (bg_Sp)
	{
		bg_Sp->setPosition(Vec2(VISIBLESIZE.width / 2, VISIBLESIZE.height / 2));
		addChild(bg_Sp);

		auto bg_width = bg_Sp->getContentSize().width;
		auto bg_height = bg_Sp->getContentSize().height;
		bg_Sp->setScale(VISIBLESIZE.width / bg_width, VISIBLESIZE.height / bg_height);
	}

	_girlSp = Sprite::create();
	if (_girlSp)
	{
		bg_Sp->addChild(_girlSp);
		_girlSp->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, bg_Sp, Vec2(0, 0)));
	}
	
	this->scheduleUpdate();

	linkNum = Text::create();
	linkNum->setFontSize(32.0f);
	linkNum->setPosition(Vec2(VISIBLESIZE.width / 2, VISIBLESIZE.height - 250));
	addChild(linkNum);
	linkNum->setVisible(false);
	//UI 层
	_uilayer = UILayer::create();
	this->addChild(_uilayer,UILAYER_ZORDER);

	_swallowLayer = TouchSwallowLayer::create();
	addChild(_swallowLayer, TOUCHSWALL_ZORDER);

	return true;
}

void GameLayer::floatLeftStarMsg(int leftNum)
{
	if (_swallowLayer)
	{
		_swallowLayer->setLocalZOrder(TOUCHSWALL_ZORDER);
	}

	FloatWord* leftStarMsg1 = FloatWord::create(CommonFunction::WStrToUTF8(L"剩余:") + String::createWithFormat("%d", leftNum)->_string + CommonFunction::WStrToUTF8(L"个"),
		32, Point(VISIBLESIZE.width, VISIBLESIZE.height /  3*2 - 30));
	this->addChild(leftStarMsg1);

	int rewordScore = GameData::getInstance()->getRewordScore(leftNum);		//奖励分数

	FloatWord* leftStarMsg2 = FloatWord::create(CommonFunction::WStrToUTF8(L"奖励:") + String::createWithFormat("%d", rewordScore)->_string + CommonFunction::WStrToUTF8(L"分"),
		32, Point(VISIBLESIZE.width, VISIBLESIZE.height / 2 + 50));
	this->addChild(leftStarMsg2);

	//可以开始消除剩余的星星
	leftStarMsg1->floatInOut(0.5f, 1.5f,
		[=](){
		matrix->setNeedClear(true);
		hideLinkNum();

		GameData* data = GameData::getInstance();

		data->setCurScore(data->getCurSocre() + rewordScore);
		if (data->getCurSocre() > data->getHistoryScore())
		{
			data->setHistoryScore(data->getCurSocre());
		}
		refrishUI();

		if (_swallowLayer)
		{
			_swallowLayer->setLocalZOrder(TOUCHSWALL_ZORDER_1);
		}
	});
	leftStarMsg2->floatInOut(0.5f, 1.5f, nullptr);

}

void GameLayer::floatLevelWord()
{
	if (_swallowLayer)
	{
		_swallowLayer->setLocalZOrder(TOUCHSWALL_ZORDER);
	}

	int curlev = GameData::getInstance()->getCurLevel();
	_levelMsg = FloatWord::create(
		
		CommonFunction::WStrToUTF8(L"关卡:") + cocos2d::String::createWithFormat("%d", curlev)->_string,
		32, Point(VISIBLESIZE.width, VISIBLESIZE.height / 3 * 2)
		);
	this->addChild(_levelMsg, 1);

	_levelMsg->floatInOut(0.5f,1.5f,nullptr);

	//-----------------------------------------------------//
	_targetScore = FloatWord::create(
		CommonFunction::WStrToUTF8(L"目标:") + cocos2d::String::createWithFormat("%d", GameData::getInstance()->getTargetScore(curlev))->_string + CommonFunction::WStrToUTF8(L"分"),
		32, Point(VISIBLESIZE.width, VISIBLESIZE.height / 2 + 50)
		);
	this->addChild(_targetScore, 1);
	_targetScore->floatInOut(0.5f, 1.5f, [=](){
		
		//继续游戏进来的
		if (UserDefault::getInstance()->getBoolForKey(ISCONTINUE))
		{
			/*
			//调用Matrix的创建方法
			matrix->createStarFromData();
			//删掉json数据，设置ISCONTINUE为false
			DataSava::getInstance()->clearJsonData();
			UserDefault::getInstance()->setBoolForKey(ISCONTINUE, false);
			UserDefault::getInstance()->flush();

			// 继续游戏的时候要判断是否没有的消除了
			if (matrix->isEnded())
			{
				auto callFunc = CallFunc::create([=](){
					matrix->setNeedClear(true);
					hideLinkNum();
				});
				auto delay = DelayTime::create(1.0f);
				auto seq = Sequence::create(delay, callFunc,nullptr);
				this->runAction(seq);
			}

			if (_swallowLayer)
			{
				_swallowLayer->setLocalZOrder(TOUCHSWALL_ZORDER_1);
			}
			*/
			//cout << "之前的一关................." << endl;
		}
		else
		{
			matrix->initMatrix();

			if (_swallowLayer)
			{
				_swallowLayer->setLocalZOrder(TOUCHSWALL_ZORDER_1);
			}

			if (GameData::getInstance()->getCurLevel() == 1)
			{
				GameData::getInstance()->setCurScore(0);
			}
			else
			{
				GameData::getInstance()->setCurScore(GameData::getInstance()->getOldScore());

			}

			refrishUI();

			int curLev = GameData::getInstance()->getCurLevel();

			if (GameData::getInstance()->getCurLevel() <= 11)
			{
				if (curLev == 1 || curLev == 7)
				{
					if (_girlSp)
						_girlSp->initWithTexture(Director::getInstance()->getTextureCache()->addImage("girl_1.png"));

				}
				else if (curLev == 2 || curLev == 8)
				{
					if (_girlSp)
						_girlSp->initWithTexture(Director::getInstance()->getTextureCache()->addImage("girl_2.png"));
				}
				else if (curLev == 3 || curLev == 9)
				{
					if (_girlSp)
					_girlSp->initWithTexture(Director::getInstance()->getTextureCache()->addImage("girl_3.png"));
				}
				else if (curLev == 4 || curLev == 10)
				{
					if (_girlSp)
						_girlSp->initWithTexture(Director::getInstance()->getTextureCache()->addImage("girl_4.png"));
				}
				else if (curLev == 5 || curLev == 11)
				{
					if (_girlSp)
						_girlSp->initWithTexture(Director::getInstance()->getTextureCache()->addImage("girl_5.png"));
				}
			}

			if (curLev <= 11 )
			{
				if (curLev == 6)
				{
					if (_girlSp)
					{
						_girlSp->setVisible(false);
					}
				}
				else
				{
					if (_girlSp)
					{
						_girlSp->setVisible(true);
					}
				}
	
			}
			else
			{
				if (_girlSp)
				{
					_girlSp->setVisible(false);
				}
			}
		}
		
	});
}

void GameLayer::showStarMatrix()
{
	matrix = StarMatrix::create(this);

	matrix->setPosition(CommonFunction::getVisibleAchor(0,0,this,Vec2(0,0)));
	this->addChild(matrix,STARMATRIX_ZORDER);

	if (matrix)
	{
		//matrix->setScale();
		float width = matrix->getContentSize().width;
		float height = matrix->getContentSize().height;
		log("....width=%d,heig=%d", width,height);
	}
}

void GameLayer::update(float delta)
{
	if (matrix)
	{
		matrix->updateStar(delta);
	}
}

bool GameLayer::onTouchBegan(Touch* touch, Event* event)
{
	Point p = touch->getLocationInView();

	p = Director::getInstance()->convertToGL(p);

	if (matrix)
	{
		matrix->onTouch(p);
		
	}
	return true;
}

void GameLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
	for (auto &item : touches)
	{
		Point p = item->getLocationInView();
		p = Director::getInstance()->convertToGL(p);
		if (matrix)
		{
			matrix->onTouch(p);
		}
	}
}

void GameLayer::showLinkNum(int size){

	string s = String::createWithFormat("%d", size)->_string + CommonFunction::WStrToUTF8(L"连消") +
		String::createWithFormat("%d", size*size * 5)->_string + CommonFunction::WStrToUTF8(L"分");
	linkNum->setString(s);
	linkNum->setVisible(true);

		auto fadeOut = FadeOut::create(2.5f);
		auto fadein = FadeIn::create(0.01f);
		auto seq = Sequence::create(fadein, fadeOut, nullptr);
		linkNum->runAction(seq);
}
//飘字
void GameLayer::showScore(int size, Point p)
{
		Label * score1 = Label::createWithTTF(String::createWithFormat("%d", size*size * 5)->_string, "fonts/arial.ttf", 25);

		score1->setPosition(p);

		addChild(score1,100);

		DelayTime* delayTime = DelayTime::create(0.1);

		MoveTo * move = MoveTo::create(0.6,Vec2(250,620));
		auto easeMove = EaseBackIn::create(move);

		auto action = Sequence::create(delayTime, easeMove, CallFunc::create([=]
		{
			if (score1)
			{
				score1->removeFromParent();
			}
		}), nullptr);

		score1->runAction(action);
}

void GameLayer::hideLinkNum(){
	linkNum->setVisible(false);
}

void GameLayer::gotoNextLevel()
{
	GameData::getInstance()->setOldScore(GameData::getInstance()->getCurSocre());

	log("oldscore=%d", GameData::getInstance()->getOldScore());

	Audio::getInstance()->playNextLevel();

	floatLevelWord();

	refrishUI();
}

void GameLayer::gotoGameOver()
{
	Audio::getInstance()->playLose();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	//去游戏失败界面  第二关失败是特殊的  其他都是正常的
	int curLevel = GameData::getInstance()->getCurLevel();

	LOGD(">>>>>>curLecel:%d", curLevel);

	if (curLevel == 2)	//是第二关失败的奖励界面
	{
		LOGD(">>>>>>level->2");
		int reward_two_fail = UserDefault::getInstance()->getIntegerForKey(REWARD_LEVEL_TWO_FAIL, 1);	//第二关失败奖励
		if (reward_two_fail <= 1)
		{
			JniMethodInfo info;
			bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/PayService", "pay", "(I)V");
			if (ret)
			{
				jobject jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID, 4);
			}
			LOGD(">>>>>>>localId=4");
		}
		else
		{
			int dimaNum = GameData::getInstance()->getDiamondNum();

			if (dimaNum >= 30)
			{
				//不扣钱,失败界面，扣除30钻石，确定下一关，取消到第一关
				JniMethodInfo info;
				bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/PayService", "pay", "(I)V");
				if (ret)
				{
					jobject jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID, 14);
				}
				LOGD(">>>>>>>localId=14");
			}else
			{
				//扣钱,失败界面 10元 220钻石
				JniMethodInfo info;
				bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/PayService", "pay", "(I)V");
				if (ret)
				{
					jobject jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID, 5);
				}
				LOGD(">>>>>>>localId=5");
			}
		}
	}
	else
	{
		int dimaNum = GameData::getInstance()->getDiamondNum();

		if (dimaNum >= 30)
		{
			//不扣钱失败界面，扣除30钻石，确定下一关，取消到第一关
			JniMethodInfo info;
			bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/PayService", "pay", "(I)V");
			if (ret)
			{
				jobject jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID, 14);
			}
			LOGD(">>>>>>>localId=14");

		}else
		{
			//扣钱失败界面 10元 220钻石
			JniMethodInfo info;
			bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/PayService", "pay", "(I)V");
			if (ret)
			{
				jobject jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID, 5);
			}
			LOGD(">>>>>>>localId=5");
		}
	}

#else
	auto overlayer = GameOverLayer::create();
	addChild(overlayer, GAMEOVERLAYER_ZORDER);

#endif
}

void GameLayer::gotoGameOverPopLayer()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	auto layer = GameOverPopLayer::create();
	addChild(layer, GAMEOVERPOPLLYER_ZORDER);

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	JniMethodInfo info;
	bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/PayService", "pay", "(I)V");
	if (ret)
	{
		jobject jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID, 11);
	}
	log("<<<<<<eventId = 11");

#endif
}

void GameLayer::gotoRewardLayer()
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	int curLevel = GameData::getInstance()->getCurLevel();

	LOGD(">>>>>>curLecel:%d",curLevel);

	if (curLevel == 2)	//总是会提前加1，所以当前是第一关的奖励
	{
		LOGD("cometo level->2");
		int reward_one = UserDefault::getInstance()->getIntegerForKey(REWARD_LEVEL_ONE, 1);	//第一关过关奖励
		if (reward_one <= 1)
		{
			JniMethodInfo info;
			bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/PayService", "pay", "(I)V");
			if (ret)
			{
				jobject jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID,2);
			}
			LOGD(">>>>>>>localId=2");
		}else
		{
			//正常的通关奖励
			/*
			JniMethodInfo info;
			bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/PayService", "pay", "(I)V");
			if (ret)
			{
				jobject jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID, 6);
			}
				LOGD(">>>>>>>localId=6");
			*/

			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(GOTONEXTLEVEL);
		}
	}
	else if (curLevel == 3)	//第二关的奖励
	{
		LOGD("cometo level->3");
		int reward_two = UserDefault::getInstance()->getIntegerForKey(REWARD_LEVEL_TWO, 1);	//第二关过关奖励
		if (reward_two <= 1)
		{
			JniMethodInfo info;
			bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/PayService", "pay", "(I)V");
			if (ret)
			{
				jobject jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID,3);
			}
			LOGD(">>>>>>>localId=3");
		}
		else
		{
			JniMethodInfo info;
			bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/PayService", "pay", "(I)V");
			if (ret)
			{
				jobject jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID, 6);
			}
			LOGD(">>>>>>>localId=6");
		}
	}
	else
	{
		JniMethodInfo info;
		bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/PayService", "pay", "(I)V");
		if (ret)
		{
			jobject jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID, 6);
		}
		LOGD(">>>>>>>localId=6");
	}

#else

	//auto rewardLayer = RewardLayer::create();
	//addChild(rewardLayer, REWAEDLAYER_ZORDER);

	gotoNextLevel();
	
#endif

}

void GameLayer::gotoWelclmeScene()
{
	auto wellayer = WelcomeScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, wellayer));
}

void GameLayer::addPauseLayer()
{
	auto pauseLayer = PauseLayer::create();
	addChild(pauseLayer,POUSELAYER_ZORDER);
}

void GameLayer::addShopLayer()
{
	auto shopLayer = ShopLayer::create();
	addChild(shopLayer, SHOPLAYER_ZORDER);
}

void GameLayer::addBuyDiamondLayer()
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	JniMethodInfo info;
	bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/PayService", "pay", "(I)V");
	if (ret)
	{
		jobject jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID, 7);
	}
	log("<<<<<<eventId = 7");

#else
	//auto buydiamLayer = BuyDimaLayer::create();
	//addChild(buydiamLayer, BUYDIAMLAYER_ZORDER);
	log("payLayer..");
#endif
}

void GameLayer::refrishUI()
{
	_uilayer->refrishUI();
	
}

void GameLayer::resetLeftStarPos()
{
	if (matrix)
	{
		matrix->resetLeftStarPos();
	}
}

void GameLayer::changeStarColor()
{
	if (matrix)
	{
		matrix->setBoom(false);
		if (matrix->getChangeColor())
		{
			matrix->setChangeColor(false);
		}
		else
		{
			matrix->setChangeColor(true);
		}
	}
}

void GameLayer::runBoomPop()
{
	if (matrix)
	{
		if (matrix->getBoom())
		{
			matrix->setBoom(false);
		}
		else
		{
			matrix->setBoom(true);
		}
		matrix->setChangeColor(false);
	}
}

void GameLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	if (event->getType() == Event::Type::KEYBOARD)
	{
		event->stopPropagation();
		{
			if (keycode == EventKeyboard::KeyCode::KEY_BACK)
			{
				auto pauseLayer = PauseLayer::create();
				addChild(pauseLayer, POUSELAYER_ZORDER);
			}
		}
	}
}
