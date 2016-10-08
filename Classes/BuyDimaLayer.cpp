#include "BuyDimaLayer.h"
#include "Constant.h"
#include "GameData.h"
#include "Audio.h"

BuyDimaLayer::BuyDimaLayer()
{

}

BuyDimaLayer::~BuyDimaLayer()
{

}

bool BuyDimaLayer::init()
{
	if (!TopDownLayer::init())
	{
		return false;
	}

	addUI();
	return true;
}

void BuyDimaLayer::onEnter()
{
	TopDownLayer::onEnter();
}

void BuyDimaLayer::addUI()
{
	
	auto bg_sp = Sprite::create("someUI/warningBg_1.png");
	bg_sp->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, this, Vec2(0, 0)));
	m_popNode->addChild(bg_sp);

	auto cofirm_btn = Button::create("someUI/comfirmBtn.png");
	cofirm_btn->setPosition(CommonFunction::getVisibleAchor(0.5, 0, bg_sp, Vec2(0, cofirm_btn->getContentSize().height / 2 + 15)));
	cofirm_btn->addClickEventListener(CC_CALLBACK_1(BuyDimaLayer::confirmCbk,this));
	bg_sp->addChild(cofirm_btn);

	auto closeBtn = Button::create("Cancel.png");
	closeBtn->setPosition(CommonFunction::getVisibleAchor(1, 1, bg_sp, Vec2(-closeBtn->getContentSize().width / 2, -closeBtn->getContentSize().height / 2)));
	closeBtn->addClickEventListener(CC_CALLBACK_1(BuyDimaLayer::closeCbk, this));
	bg_sp->addChild(closeBtn);

	auto youhui_sp = Sprite::create("someUI/dayouhui.png");
	youhui_sp->setPosition(CommonFunction::getVisibleAchor(0, 1, bg_sp, Vec2(100, -youhui_sp->getContentSize().height + 40)));
	bg_sp->addChild(youhui_sp);

	auto buy_sp = Sprite::create("someUI/buydiam_1.png");
	buy_sp->setScale(1.5f);
	buy_sp->setPosition(CommonFunction::getVisibleAchor(0.5, 1, bg_sp, Vec2(0, -buy_sp->getContentSize().height - 110)));
	bg_sp->addChild(buy_sp);

	auto word_sp = Sprite::create("someUI/buydiam_2.png");
	word_sp->setScale(1.5f);
	word_sp->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, bg_sp, Vec2(0, -70)));
	bg_sp->addChild(word_sp);
	
}

void BuyDimaLayer::confirmCbk(Ref *sender)
{
	Audio::getInstance()->onClickBtn();
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

	JniMethodInfo info;
	bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/PayService", "pay", "(I)V");
	if (ret)
	{
		jobject jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID, 22);
	}
	log("......eventId = 22");
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto data = GameData::getInstance();
	data->setDiamondNum(data->getDiamondNum() + 266);

	EventCustom event_1(REFRISHUILAYER);
	_eventDispatcher->dispatchEvent(&event_1);

#endif
	close();
}

void BuyDimaLayer::closeCbk(Ref *sender)
{
	Audio::getInstance()->onClickBtn();
	close();
}
