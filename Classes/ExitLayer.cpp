#include "ExitLayer.h"
#include "CommonFunction.h"
#include "Audio.h"
#include "BuyDimaLayer.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include <android/log.h>
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

#endif

ExitLayer::ExitLayer()
{

}

ExitLayer::~ExitLayer()
{

}

bool ExitLayer::init()
{
	if (!PopLayer::init())
	{
		return false;
	}
	addUI();
	return true;
}

void ExitLayer::addUI()
{
	auto bg_sp = Sprite::create("exitbg.png");
	m_popNode->addChild(bg_sp);

	Button* exit_btn = Button::create("exit.png");
	exit_btn->setPosition(CommonFunction::getVisibleAchor(0, 0, bg_sp, Vec2(exit_btn->getContentSize().width, exit_btn->getContentSize().height /2)));
	exit_btn->addClickEventListener(CC_CALLBACK_1(ExitLayer::exitBtnCbk, this));
	bg_sp->addChild(exit_btn);

	Button* buy_btn = Button::create("exit_lq.png");
	buy_btn->setPosition(CommonFunction::getVisibleAchor(1, 0, bg_sp, Vec2(-buy_btn->getContentSize().width, buy_btn->getContentSize().height / 2)));
	buy_btn->addClickEventListener(CC_CALLBACK_1(ExitLayer::buyBtnCbk, this));
	bg_sp->addChild(buy_btn);

	int bg_height = bg_sp->getContentSize().height;
	Button* close_btn = Button::create("Cancel.png");
	close_btn->setPosition(CommonFunction::getVisibleAchor(1, 0, bg_sp, Vec2(-close_btn->getContentSize().width / 2, bg_height / 2)));
	close_btn->addClickEventListener(CC_CALLBACK_1(ExitLayer::closeBtnCbk, this));
	bg_sp->addChild(close_btn);
}

void ExitLayer::exitBtnCbk(Ref *sender)
{
	Audio::getInstance()->onClickBtn();
	//退出游戏
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
	Director::getInstance()->end();
}

void ExitLayer::buyBtnCbk(Ref *sender)
{
	Audio::getInstance()->onClickBtn();
	//购买
	auto buydiamLayer = BuyDimaLayer::create();
	buydiamLayer->ignoreAnchorPointForPosition(true);
	buydiamLayer->setAnchorPoint(Vec2(0.5,0.5));
	buydiamLayer->setPosition(Vec2(0,0));
	addChild(buydiamLayer,20);
}

void ExitLayer::closeBtnCbk(Ref *sender)
{
	Audio::getInstance()->onClickBtn();
	//关闭
	close();
}

void ExitLayer::onEnter()
{
	PopLayer::onEnter();
}
