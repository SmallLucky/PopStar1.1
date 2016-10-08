#include "ui/UIButton.h"
#include "GameScene.h"
#include "GameData.h"
#include "ShopLayer.h"
#include "CommonFunction.h"
#include "ExitLayer.h"
#include "Audio.h"
#include "HelpLayer.h"
#include "DataSave.h"
#include "SimpleAudioEngine.h"
#include "LoginRewordLayer.h"
#include "WelcomeScene.h"
#include "Constant.h"

#define VISIBLESIZE Director::getInstance()->getVisibleSize()

using namespace ui;

WelcomeScene::WelcomeScene()
{

}

WelcomeScene::~WelcomeScene()
{

}

cocos2d::Scene* WelcomeScene::createScene()
{
	auto scene = Scene::create();
	auto layer = WelcomeScene::create();
	scene->addChild(layer);
	return scene;
}

bool WelcomeScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	GameData::getInstance()->praseJsonData();
	DataSava::getInstance()->praseJsonData();

	addUI();
	//Audio::getInstance()->playBGM();
	
	int eventId = UserDefault::getInstance()->getIntegerForKey(LOGINREWARD,1);

	int curDay = CommonFunction::getCurDay();

	int saveDay = UserDefault::getInstance()->getIntegerForKey(MYDAY);

	if (curDay != saveDay)
	{
		if (eventId <= 5)
		{
			auto delay = DelayTime::create(3.0);

			auto callfunc = CallFunc::create([=](){

				auto rewordLayer = LoginRewordLayer::create(eventId);
				addChild(rewordLayer,100);

				UserDefault::getInstance()->setBoolForKey(ISREWARDCOME, true);	//是否弹出了登录奖励

				log("set isrewardcome =%d", UserDefault::getInstance()->getBoolForKey(ISREWARDCOME));
			});

			auto seq = Sequence::create(delay, callfunc, nullptr);
			this->runAction(seq);
		}

	}

	Audio::getInstance()->playMainBGM();

	log("curday =%d.....", CommonFunction::getCurDay());

	return true;
}

void WelcomeScene::addUI()
{
	//背景
	auto bg_Sp = Sprite::create("background.png");
	bg_Sp->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,this,Vec2(0,0)));
	addChild(bg_Sp);

	auto bg_width = bg_Sp->getContentSize().width;
	auto bg_height = bg_Sp->getContentSize().height;

	bg_Sp->setScale(VISIBLESIZE.width / bg_width, VISIBLESIZE.height / bg_height);

	//继续游戏
	auto continue_btn = Button::create("menu_continue.png");
	continue_btn->addClickEventListener(CC_CALLBACK_1(WelcomeScene::continueGameCbk, this));
	continue_btn->setPosition(CommonFunction::getVisibleAchor(0.5,0.5,this,Vec2(0,-30)));
	addChild(continue_btn);

	//新游戏
	auto newGame_btn = Button::create("menu_start.png");
	newGame_btn->addClickEventListener(CC_CALLBACK_1(WelcomeScene::newGameCbk,this));
	newGame_btn->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, continue_btn, Vec2(0, newGame_btn->getContentSize().height)));

	continue_btn->addChild(newGame_btn);

	//商城
	auto shop_btn = Button::create("menu_shop.png");
	shop_btn->addClickEventListener(CC_CALLBACK_1(WelcomeScene::shopCbk, this));
	shop_btn->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, continue_btn, Vec2(0, -shop_btn->getContentSize().height)));
	continue_btn->addChild(shop_btn);

	//帮助按钮
	auto help_btn = Button::create("menu_help.png");
	help_btn->addClickEventListener(CC_CALLBACK_1(WelcomeScene::helpCbk, this));
	//help_btn->setPosition(Vec2(help_btn->getContentSize().width,help_btn->getContentSize().height));
	help_btn->setPosition(CommonFunction::getVisibleAchor(0, 0, this, Vec2(help_btn->getContentSize().width, help_btn->getContentSize().height)));

	addChild(help_btn);

	auto toggleItem1 = MenuItemImage::create("menu_sound_on.png", "menu_sound_on.png");
	auto toggleItem2 = MenuItemImage::create("menu_sound_off.png", "menu_sound_off.png");

	auto toggle = MenuItemToggle::create();
	toggle->setCallback(CC_CALLBACK_1(WelcomeScene::musicCbk, this));
	toggle->addSubItem(toggleItem1);
	toggle->addSubItem(toggleItem2);
	toggle->setSelectedIndex(0);//设置默认初始值
	//toggle->setPosition(Vec2(help_btn->getPositionX(), help_btn->getPositionY() + 80));
	toggle->setPosition(CommonFunction::getVisibleAchor(0, 0, this, Vec2(help_btn->getPositionX(), help_btn->getPositionY() + 80)));

	if (!UserDefault::getInstance()->getBoolForKey("ISMUSICON"))
	{
		toggle->setSelectedIndex(1);
	}

	auto music_menu = Menu::create(toggle,nullptr);
	music_menu->setPosition(CommonFunction::getVisibleAchor(0,0,this,Vec2(0,0)));
	this->addChild(music_menu);
}

void WelcomeScene::newGameCbk(cocos2d::Ref* pSender)
{
	auto s = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, s));

	GameData::getInstance()->setCurScore(0);
	GameData::getInstance()->setCurLevel(1);

	//清除数据
	DataSava::getInstance()->clearJsonData();

	UserDefault::getInstance()->setBoolForKey("ISCONTINUE", false);
	UserDefault::getInstance()->flush();

	Audio::getInstance()->onClickBtn();
}

void WelcomeScene::continueGameCbk(cocos2d::Ref* pSender)
{
	DataSava::getInstance()->praseJsonData();

	auto data = DataSava::getInstance();

	int starNum = data->getStarColorList().size();

	if (starNum>0)
	{
		UserDefault::getInstance()->setBoolForKey(ISCONTINUE, true);
		UserDefault::getInstance()->flush();

		auto s = GameScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, s));
	}
	else
	{
		auto s = GameScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, s));
	}
	Audio::getInstance()->onClickBtn();
}

void WelcomeScene::shopCbk(cocos2d::Ref* pSender)
{
	auto shop = ShopLayer::create();
	addChild(shop);
	Audio::getInstance()->onClickBtn();
}

void WelcomeScene::musicCbk(cocos2d::Ref* pSender)
{
	//cout << "音乐" << endl;
	auto toggle = (MenuItemToggle*)pSender;
	int index = toggle->getSelectedIndex();
	if (index == 0)
	{
		UserDefault::getInstance()->setBoolForKey("ISMUSICON",true);
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}
	else
	{
		UserDefault::getInstance()->setBoolForKey("ISMUSICON", false);
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		
	}
	Audio::getInstance()->onClickBtn();
}

void WelcomeScene::helpCbk(cocos2d::Ref* pSender)
{
	auto helpLayer = HelpLayer::create();
	addChild(helpLayer);
	Audio::getInstance()->onClickBtn();
}

void WelcomeScene::onEnter()
{
	Layer::onEnter();

	auto listenerkeyPad = EventListenerKeyboard::create();
	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(WelcomeScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
}

void WelcomeScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	if (event->getType() == Event::Type::KEYBOARD)
	{
		event->stopPropagation();
		{
			if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
			{
				ExitLayer* l = ExitLayer::create();
				addChild(l);
			}
		}
	}
}
