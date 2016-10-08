#include "GameOverPopLayer.h"
#include "Constant.h"
#include "CommonFunction.h"
#include "Audio.h"
#include "GameData.h"
#include "DataSave.h"

GameOverPopLayer::GameOverPopLayer()
{

}

GameOverPopLayer::~GameOverPopLayer()
{

}

bool GameOverPopLayer::init()
{
	if (!DownTopLayer::init())
	{
		return false;
	}

	addUI();

	return true;
}

void GameOverPopLayer::onEnter()
{
	DownTopLayer::onEnter();
}

void GameOverPopLayer::addUI()
{
	//背景
	auto bg_sp = Sprite::create("someUI/warningBg_1.png");
	m_popNode->addChild(bg_sp);

	//标题
	auto word_sp = Sprite::create("someUI/word1.png");
	word_sp->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.5f, bg_sp, Vec2(0, 30)));
	bg_sp->addChild(word_sp);

	//确定按钮
	auto comfirm_btn = Button::create("someUI/comfirmBtn.png");
	comfirm_btn->addClickEventListener(CC_CALLBACK_1(GameOverPopLayer::confirmCbk, this));

	comfirm_btn->setPosition(CommonFunction::getVisibleAchor(0.5, 0, bg_sp, Vec2(0, comfirm_btn->getContentSize().height)));
	bg_sp->addChild(comfirm_btn);

	//关闭按钮
	auto close_btn = Button::create("Cancel.png");
	close_btn->addClickEventListener(CC_CALLBACK_1(GameOverPopLayer::closeCbk, this));
	close_btn->setPosition(CommonFunction::getVisibleAchor(1, 1, bg_sp, Vec2(-close_btn->getContentSize().width /2, -close_btn->getContentSize().height /2)));
	bg_sp->addChild(close_btn);

}

void GameOverPopLayer::confirmCbk(Ref *sender)
{
	Audio::getInstance()->onClickBtn();
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

	JniMethodInfo info;
	bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/PayService", "pay", "(I)V");
	if (ret)
	{
		jobject jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID, 11);
	}
	log(">>>>>>>>>>>>>>>>>>LocaleventId =11");
#endif
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

	auto data = GameData::getInstance();
	data->setDiamondNum(data->getDiamondNum() + 40);

	data->setDiamondNum(data->getDiamondNum() -30);

	//去下一关
	EventCustom event(GOTONEXTLEVEL);
	_eventDispatcher->dispatchEvent(&event);
	//刷新UI层的UI
	_eventDispatcher->dispatchCustomEvent(REFRISHUILAYER);
#endif
	close();
}

void GameOverPopLayer::closeCbk(Ref *sender)
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
	//5元100钻石界面
}

void GameOverPopLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	if (event->getType() == Event::Type::KEYBOARD)
	{
		event->stopPropagation();
		{
			if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
			{
				//close();
			}
		}
	}
}
