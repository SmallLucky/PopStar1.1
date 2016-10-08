#include "ShopItem.h"
#include "CommonFunction.h"
#include "ShopLayer.h"
#include "GameData.h"
#include "Constant.h"
#include "Audio.h"

ShopLayer::ShopLayer():
diaNum_text(nullptr),
_userId(nullptr)
{
	auto _listener = EventListenerCustom::create(REFRIEHSHOPUI, [=](EventCustom*event){
		refrishUI();
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_listener, 1);
}

ShopLayer::~ShopLayer()
{
	_eventDispatcher->removeCustomEventListeners(REFRIEHSHOPUI);
}

void  ShopLayer::onEnter()
{
	TopDownLayer::onEnter();
}

bool ShopLayer::init()
{
	if (!TopDownLayer::init())
	{
		return false;
	}

	addUI();
	return true;
}

void ShopLayer::addUI()
{
	//背景
	auto bg_sp = Sprite::create("Diog_bg.png");
	m_popNode->addChild(bg_sp);

	auto title_bg_sp = Sprite::create("Dialog_Title.png");
	title_bg_sp->setPosition(CommonFunction::getVisibleAchor(0.5, 1.0, bg_sp, Vec2(0, 0)));
	bg_sp->addChild(title_bg_sp);

	auto title_sp = Sprite::create("Title_shop.png");
	title_sp->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, title_bg_sp, Vec2(0, 35)));
	title_bg_sp->addChild(title_sp);

	auto close_Btn = Button::create("Cancel.png");
	close_Btn->addClickEventListener(CC_CALLBACK_1(ShopLayer::closeCbk,this));
	close_Btn->setPosition(CommonFunction::getVisibleAchor(0.8, 0.8, title_bg_sp, Vec2(0, 10)));
	title_bg_sp->addChild(close_Btn);

	auto hava_text = Text::create();
	hava_text->setFontSize(20);
	hava_text->setColor(Color3B::BLACK);
	hava_text->setString(CommonFunction::WStrToUTF8(L"拥有钻石:"));
	hava_text->setPosition(CommonFunction::getVisibleAchor(0.5, 0, bg_sp, Vec2(-50, 60)));
	bg_sp->addChild(hava_text);

	diaNum_text = Label::createWithTTF("", "fonts/arial.ttf", 20);
	int diaNum = GameData::getInstance()->getDiamondNum();
	diaNum_text->setString(Value(diaNum).asString());
	diaNum_text->setColor(Color3B::BLACK);
	diaNum_text->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, hava_text, Vec2(80, 0)));
	hava_text->addChild(diaNum_text);

	/*
	auto userid_text = Text::create();
	userid_text->setString(CommonFunction::WStrToUTF8(L"用户ID:"));
	userid_text->setColor(Color3B::BLACK);
	userid_text->setFontSize(20);
	userid_text->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, hava_text, Vec2(12, -30)));
	hava_text->addChild(userid_text);

	_userId = Label::createWithTTF("", "fonts/arial.ttf", 20);
	_userId->setString("61462852");
	_userId->setColor(Color3B::BLACK);
	_userId->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, userid_text, Vec2(90, 0)));
	userid_text->addChild(_userId);
	*/

	auto phone_text = Text::create();
	phone_text->setString(CommonFunction::WStrToUTF8(L"客服热线: 0571-86956190"));
	phone_text->setColor(Color3B::BLACK);
	phone_text->setFontSize(20);
	phone_text->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, hava_text, Vec2(30,-30)));
	hava_text->addChild(phone_text);

	ScrollView*	scrollView = ScrollView::create();
	scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	scrollView->setAnchorPoint(Vec2(0.5, 0.5));
	scrollView->setContentSize(Size(450, 450));
	scrollView->setInertiaScrollEnabled(false);
	scrollView->setBounceEnabled(true);
	scrollView->setInnerContainerSize(Size(450, 7*60));////////////////
	scrollView->setBackGroundColor(Color3B::GRAY);
	//scrollView->setBackGroundColorType(Layout::BackGroundColorType::SOLID);

	scrollView->setPosition(CommonFunction::getVisibleAchor(Center, bg_sp, Vec2(0,10)));
	bg_sp->addChild(scrollView);

	//auto visibleSize = Director::getInstance()->getVisibleSize();

	auto scrv_width = scrollView->getContentSize().width;

	for (int i = 0; i <6;i++)
	{
		auto item = ShopItem::createShopItem(GAMEDATA->getDiamBuy(i+3), GAMEDATA->getDiamRewd(i+3), GAMEDATA->getNeedMoney(i+3));
		item->setTag(i);
		int item_height = item->getContentSize().height -10;
		scrollView->addChild(item);
		//item->setPosition(CommonFunction::getVisibleAchor(0.5, 0.2, bg_sp, Vec2(0, i * item_height -25)));
		//item->setPosition(Vec2(visibleSize.width /2 - 15, i * (item_height+10) + 75));

		item->setPosition(CommonFunction::getVisibleAchor(0.5,0.5, scrollView, Vec2(0, i * (item_height + 10) - 150)));
	}
}

void ShopLayer::closeCbk(Ref *sender)
{
	close();
	Audio::getInstance()->onClickBtn();
}

void ShopLayer::refrishUI()
{
	int diaNum = GameData::getInstance()->getDiamondNum();
	if (diaNum_text)
	{
		diaNum_text->setString( Value(diaNum).asString());
	}
}

void ShopLayer::setUserId(int userid)
{
	if (_userId)
	{
		_userId->setString(Value(userid).asString());
	}
}