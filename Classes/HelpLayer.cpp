#include "HelpLayer.h"
#include "CommonFunction.h"
#include"Audio.h"

HelpLayer::HelpLayer()
{

}

HelpLayer::~HelpLayer()
{

}

bool HelpLayer::init()
{
	if (!TopDownLayer::init())
	{
		return false;
	}
	addUI();
	return true;
}

void HelpLayer::onEnter()
{
	TopDownLayer::onEnter();
}

void HelpLayer::addUI()
{
	//背景
	auto bg_sp = Sprite::create("Diog_bg_1.png");
	m_popNode->addChild(bg_sp);

	auto title_bg_sp = Sprite::create("Dialog_Title.png");
	title_bg_sp->setPosition(CommonFunction::getVisibleAchor(0.5, 1.0, bg_sp, Vec2(0, 0)));
	bg_sp->addChild(title_bg_sp);

	auto title_sp = Sprite::create("Title_Help.png");
	title_sp->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, title_bg_sp, Vec2(0, 35)));
	title_bg_sp->addChild(title_sp);
	//关闭按钮
	auto close_Btn = Button::create("Cancel.png");
	close_Btn->addClickEventListener(CC_CALLBACK_1(HelpLayer::closeBtnCbk, this));
	close_Btn->setPosition(CommonFunction::getVisibleAchor(0.8, 0.8, title_bg_sp, Vec2(0, 10)));
	title_bg_sp->addChild(close_Btn);


	auto userID_text = Label::createWithTTF("61462952","fonts/arial.ttf",18);
	userID_text->setColor(Color3B::BLACK);
	userID_text->setPosition(CommonFunction::getVisibleAchor(0.2, 0, bg_sp, Vec2(55, 40)));
	bg_sp->addChild(userID_text);
	
}

void HelpLayer::closeBtnCbk(Ref *sender)
{
	close();
	Audio::getInstance()->onClickBtn();
}
