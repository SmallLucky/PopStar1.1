#include "ShopItem.h"
#include "CommonFunction.h"
#include "GameData.h"
#include "Constant.h"
#include "Audio.h"

ShopItem::ShopItem()
{

}

ShopItem::~ShopItem()
{

}

ShopItem* ShopItem::createShopItem(int diamNum, int giveNum, int moneyNum)
{
	ShopItem* item = new ShopItem();
	if (item && item->init(diamNum, giveNum, moneyNum))
	{
		item->autorelease();
		return item;
	}
	CC_SAFE_DELETE(item);
	return nullptr;
}

bool ShopItem::init(int diamNum, int giveNum, int moneyNum)
{
	if (!Node::init())
	{
		return false;
	}
	auto bg_sp = Sprite::create("Dialog_Item2.png");
	bg_sp->setScale(0.9f);
	//bg_sp->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,Vec2::ZERO));
	addChild(bg_sp);
	this->setContentSize(bg_sp->getContentSize());
	//钻石精灵
	auto diamond_sp = Sprite::create("diamond.png");
	diamond_sp->setScale(0.7f);
	int dia_width = diamond_sp->getContentSize().width;
	diamond_sp->setPosition(CommonFunction::getVisibleAchor(0, 0.5, bg_sp, Vec2(dia_width /2 +87, 0)));
	bg_sp->addChild(diamond_sp);
	//赠送精灵
	auto give_sp = Sprite::create("zengsong.png");
	give_sp->setScale(0.4f);
	int give_width = give_sp->getContentSize().width;
	give_sp->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, bg_sp, Vec2(- give_width / 3 +60, 0)));
	bg_sp->addChild(give_sp);

	//购买按钮
	auto buy_btn = Button::create("menu_money.png");
	buy_btn->setSwallowTouches(false);
	buy_btn->setScale(0.8f);
	int btn_width = buy_btn->getContentSize().width;
	buy_btn->setPosition(CommonFunction::getVisibleAchor(1.0, 0.5, bg_sp, Vec2(-btn_width / 2+15, 0)));
	buy_btn->addClickEventListener(CC_CALLBACK_1(ShopItem::btnClickEvent,this));
	bg_sp->addChild(buy_btn);

	//钻石个数文本
	auto diam_text = Text::create();
	diam_text->setString(Value(diamNum).asString()+ CommonFunction::WStrToUTF8(L"个"));
	diam_text->setFontSize(32);
	diam_text->setPosition(CommonFunction::getVisibleAchor(1, 0.5, diamond_sp, Vec2(40, 0)));
	diamond_sp->addChild(diam_text);
	//赠送个数文本
	auto give_text = Text::create();
	give_text->setString(Value(giveNum).asString() + CommonFunction::WStrToUTF8(L"个"));
	give_text->setFontSize(24);
	give_text->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, bg_sp, Vec2(70, 0)));
	bg_sp->addChild(give_text);

	//按钮上的文本
	auto buy_text = Text::create();
	buy_text->setString(CommonFunction::WStrToUTF8(L"获取"));
	buy_text->setFontSize(32);
	buy_text->setColor(Color3B::BLACK);
	buy_text->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, buy_btn, Vec2(0, 0)));
	buy_btn->addChild(buy_text);

	auto money_text = Text::create();
	money_text->setString(CommonFunction::WStrToUTF8(L"￥") + Value(moneyNum).asString());
	money_text->setFontSize(32);
	money_text->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, diamond_sp, Vec2(-60, 0)));
	diamond_sp->addChild(money_text);
	//打折
	auto count_text = Text::create();
	//count_text->setString(Value(moneyNum).asString() + CommonFunction::WStrToUTF8(L"折"));
	count_text->setColor(Color3B::RED);
	count_text->setFontSize(24);
	count_text->setRotation(-26);
	count_text->setPosition(CommonFunction::getVisibleAchor(0, 0.5, bg_sp, Vec2(count_text->getContentSize().width/2 + 25 , 0)));
	bg_sp->addChild(count_text);

	if (moneyNum<= 8)
	{
		count_text->setString(CommonFunction::WStrToUTF8(L"5折"));
	}
	else if (moneyNum<= 14)
	{
		count_text->setString(CommonFunction::WStrToUTF8(L"4折"));
	}
	else if (moneyNum <= 18)
	{
		count_text->setString(CommonFunction::WStrToUTF8(L"3折"));
	}

	return true;
}

void ShopItem::btnClickEvent(Ref* pSender)
{
	Audio::getInstance()->onClickBtn();
	int totalDiam = 0;

	for (int i = 0; i < 6;i++) //0-6
	{
		if (getTag() == i)
		{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

			JniMethodInfo info;
			bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/PayService", "pay", "(I)V");
			if (ret)
			{
				jobject jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID, 8 + i); //8->13
			}
			log(">>>>>>eventId = %d",i+8);

#else
			totalDiam = GAMEDATA->getDiamBuy(i+3) + GAMEDATA->getDiamRewd(i+3) + GAMEDATA->getDiamondNum();
			GameData::getInstance()->setDiamondNum(totalDiam);
#endif

		}
	}

	//发送消息到shop层
	EventCustom event(REFRIEHSHOPUI);
	_eventDispatcher->dispatchEvent(&event);

	//发送消息到UI层
	EventCustom event_1(REFRISHUILAYER);
	_eventDispatcher->dispatchEvent(&event_1);
}