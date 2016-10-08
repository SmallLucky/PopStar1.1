#include "GameOverLayer.h"
#include "WelcomeScene.h"
#include "CommonFunction.h"
#include "GameData.h"
#include "Constant.h"
#include "Audio.h"
#include "DataSave.h"

GameOverLayer::GameOverLayer():
timeCount(9),
_time(nullptr)
{

}

GameOverLayer::~GameOverLayer()
{

}

bool GameOverLayer::init()
{
	if (!DownTopLayer::init())
	{
		return false;
	}

	addUI();

	schedule(schedule_selector(GameOverLayer::SetTime), 1);

	return true;
}

void GameOverLayer::onEnter()
{
	DownTopLayer::onEnter();
}

void GameOverLayer::addUI()
{
	//背景
	auto bg_sp = Sprite::create("Diog_bg.png");
	bg_sp->setPosition(CommonFunction::getVisibleAchor(0.5f,0.5f,this,Vec2(0,-80)));
	bg_sp->setScaleX(0.8f);
	bg_sp->setScaleY(0.6f);
	m_popNode->addChild(bg_sp);
	//人
	auto hero_sp = Sprite::create("GameOver.png");
	hero_sp->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.5f, this, Vec2(0, 50)));
	m_popNode->addChild(hero_sp);
	//倒计时
	_time = Label::createWithTTF("10", "fonts/arial.ttf", 30);
	_time->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.5f, hero_sp, Vec2(0, -135)));
	hero_sp->addChild(_time, 2);

	//关闭按钮
	auto closeBtn = Button::create("Cancel.png");
	closeBtn->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.5f, hero_sp, Vec2(175, 45)));
	closeBtn->addClickEventListener(CC_CALLBACK_1(GameOverLayer::closeBtnCbk,this));
	hero_sp->addChild(closeBtn);

	//继续通关按钮
	auto continue_Btn = Button::create("menu_continue2.png");
	continue_Btn->addClickEventListener(CC_CALLBACK_1(GameOverLayer::continueCbk, this));
	continue_Btn->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.5f, hero_sp,Vec2(0,-200)));
	hero_sp->addChild(continue_Btn);

	Text* info_text = Text::create();
	info_text->setFontSize(20);
	info_text->setColor(Color3B::RED);
	info_text->setString(CommonFunction::WStrToUTF8(L"挑战一次需要30个钻石"));
	info_text->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.5f, continue_Btn,Vec2(0,-70)));
	continue_Btn->addChild(info_text);
}

//倒计时
void GameOverLayer::SetTime(float dt)
{
	_time->setString(Value(timeCount).asString());
	if (timeCount == 0)
	{
		this->unschedule(schedule_selector(GameOverLayer::SetTime));
		//介入计费点界面 5元100钻石
		_eventDispatcher->dispatchCustomEvent(GAMEOVERPOPLAYER);

		close();
	}
	else
	{
		timeCount--;
	}

}

void GameOverLayer::continueCbk(Ref *sender)
{
	Audio::getInstance()->onClickBtn();

	this->unschedule(schedule_selector(GameOverLayer::SetTime));

	int diam = GameData::getInstance()->getDiamondNum();

	if (diam >= 30)
	{
		//刷新UI层的UI
		GameData::getInstance()->setDiamondNum(diam - 30);
		_eventDispatcher->dispatchCustomEvent(REFRISHUILAYER);

		//去下一关
		EventCustom event(GOTONEXTLEVEL);
		_eventDispatcher->dispatchEvent(&event);

		//移除自己
		close();
	}
	else
	{
		//介入计费点界面
		close();
		_eventDispatcher->dispatchCustomEvent(GAMEOVERPOPLAYER);
	}
}

void GameOverLayer::closeBtnCbk(Ref *sender)
{
	Audio::getInstance()->onClickBtn();

	GameData::getInstance()->setCurScore(0);
	GameData::getInstance()->setCurLevel(1);
	GameData::getInstance()->setOldScore(0);
	GameData::getInstance()->praseJsonData();

	//清除数据
	DataSava::getInstance()->clearJsonData();

	_eventDispatcher->dispatchCustomEvent(REFRISHUILAYER);
	_eventDispatcher->dispatchCustomEvent(CLEANSTAR);
	_eventDispatcher->dispatchCustomEvent(GOTONEXTLEVEL);

	close();

}

void GameOverLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	if (event->getType() == Event::Type::KEYBOARD)
	{
		event->stopPropagation();
		{
			if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
			{
			}
		}
	}
}