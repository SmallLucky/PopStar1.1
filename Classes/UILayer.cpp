#include "GameData.h"
#include "PauseLayer.h"
#include "Constant.h"
#include "CommonFunction.h"
#include "Audio.h"
#include "UILayer.h"

#define GAMEDATA GameData::getInstance()

UILayer::UILayer():
m_level_label(nullptr),
m_targetScore_label(nullptr),
m_diamond_label(nullptr),
m_myscore_label(nullptr),
m_bomb_label(nullptr),
m_paint_label(nullptr),
m_rainbow_label(nullptr)
{
	auto _listener = EventListenerCustom::create(REFRISHUILAYER, [=](EventCustom*event){
		refrishUI();
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_listener, 1);
}

UILayer::~UILayer()
{
	_eventDispatcher->removeCustomEventListeners(REFRISHUILAYER);
}

void UILayer::onEnter()
{
	Layer::onEnter();

	//手机返回键事件
	auto listenerkeyPad = EventListenerKeyboard::create();
	listenerkeyPad->onKeyReleased = CC_CALLBACK_2(UILayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);

	refrishUI();
}

void UILayer::onExit()
{
	Layer::onExit();
}

bool UILayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Plist.plist");

	addUI();

	return true;
}

void UILayer::addUI()
{
	//597*51
	//历史最高分
	auto highScore_sp = Sprite::createWithSpriteFrameName("highScore.png");
	highScore_sp->setPosition(CommonFunction::getVisibleAchor(0.5f, 1.0f, this, Vec2(-70, -highScore_sp->getContentSize().height )));
	addChild(highScore_sp);


	//最高分标签

	m_highscore_label = Label::createWithTTF("00", "fonts/Square.ttf", 32);
	m_highscore_label->setPosition(Vec2(highScore_sp->getPositionX() + 140, highScore_sp->getPositionY()));
	addChild(m_highscore_label);

	//关卡
	Sprite* level_sp = Sprite::createWithSpriteFrameName("level.png");
	level_sp->setPosition(CommonFunction::getVisibleAchor(0, 1.0f, this, Vec2(level_sp->getContentSize().width /2, -100)));
	addChild(level_sp);

	//目标
	Sprite* target_sp = Sprite::createWithSpriteFrameName("targetScore.png");
	target_sp->setPosition(CommonFunction::getVisibleAchor(0.5f, 0, this, Vec2(-60, level_sp->getPositionY())));
	addChild(target_sp);

	//购买精灵背景
	Sprite* buy_sp = Sprite::create("dialog_item.png");
	buy_sp->setPosition(CommonFunction::getVisibleAchor(1.0f, 0, this, Vec2(-buy_sp->getContentSize().width / 2, level_sp->getPositionY())));
	addChild(buy_sp);

	//钻石背景
	Sprite* diamond_sp = Sprite::create("diamond.png");
	diamond_sp->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, buy_sp, Vec2(-65, 0)));
	buy_sp->addChild(diamond_sp);

	//购买按钮
	Button* buy_btn = Button::create("coin_add.png");
	buy_btn->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.5f, buy_sp,Vec2(59,0)));
	buy_btn->addClickEventListener(CC_CALLBACK_1(UILayer::buyBtnCallback, this));
	buy_sp->addChild(buy_btn);

	//关卡标签

	m_level_label = Label::createWithTTF("00", "fonts/Square.ttf", 32);
	m_level_label->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.5f, level_sp,Vec2(70,0)));
	level_sp->addChild(m_level_label);

	//目标标签
	m_targetScore_label = Label::createWithTTF("00", "fonts/Square.ttf", 32);
	m_targetScore_label->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.5f, target_sp, Vec2(80, 0)));
	target_sp->addChild(m_targetScore_label);


	//钻石标签
	m_diamond_label = Label::createWithTTF("00", "fonts/Square.ttf", 35);
	m_diamond_label->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.5f, buy_sp, Vec2(0, 0)));
	buy_sp->addChild(m_diamond_label);

	//暂停按钮

	Button* stop_btn = Button::create("Item_pause.png");
	stop_btn->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.5f, level_sp, Vec2(0, -80)));
	stop_btn->addClickEventListener(CC_CALLBACK_1(UILayer::pauseBtnCallback, this));
	level_sp->addChild(stop_btn);

	//我的分数图片
	auto myScore_sp = Sprite::createWithSpriteFrameName("totalScore.png");
	myScore_sp->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.5f, stop_btn, Vec2(105, 0)));
	stop_btn->addChild(myScore_sp);

	//我的分数
	m_myscore_label = Label::createWithTTF("0", "fonts/Square.ttf", 35);
	m_myscore_label->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.5f, myScore_sp, Vec2(90, -5)));
	myScore_sp->addChild(m_myscore_label);

	//三个道具
	Button* bomb_btn = Button::create("Props_Bomb.png");
	bomb_btn->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.5f, buy_sp, Vec2(-70, -80)));
	bomb_btn->addClickEventListener(CC_CALLBACK_1(UILayer::bombBtnCallback, this));
	buy_sp->addChild(bomb_btn);

	Button* paint_btn = Button::create("Props_Paint.png");
	paint_btn->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.5f, bomb_btn, Vec2(52, 0)));
	paint_btn->addClickEventListener(CC_CALLBACK_1(UILayer::paintBtnCallback, this));
	bomb_btn->addChild(paint_btn);

	Button* rainbow_btn = Button::create("Props_Rainbow.png");
	rainbow_btn->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.5f, paint_btn, Vec2(52, 0)));
	rainbow_btn->addClickEventListener(CC_CALLBACK_1(UILayer::rainbowBtnCallback, this));
	paint_btn->addChild(rainbow_btn);

	//三个道具的标签
	m_bomb_label = Label::createWithTTF("00", "fonts/Square.ttf", 25);
	m_bomb_label->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.5f, bomb_btn, Vec2(0, -40)));
	bomb_btn->addChild(m_bomb_label);

	m_paint_label = Label::createWithTTF("00", "fonts/Square.ttf", 25);
	m_paint_label->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.5f, paint_btn, Vec2(0, -40)));
	paint_btn->addChild(m_paint_label);


	m_rainbow_label = Label::createWithTTF("00", "fonts/Square.ttf", 25);
	m_rainbow_label->setPosition(CommonFunction::getVisibleAchor(0.5f, 0.5f, rainbow_btn, Vec2(0, -40)));
	rainbow_btn->addChild(m_rainbow_label);

	initUI();
}

void UILayer::initUI()
{
	int curlevel = GAMEDATA->getCurLevel();

	m_highscore_label->setString(Value(GAMEDATA->getHistoryScore()).asString());
	m_level_label->setString(Value(GAMEDATA->getCurLevel()).asString());
	m_targetScore_label->setString(Value(GAMEDATA->getTargetScore(curlevel)).asString());
	m_diamond_label->setString(Value(GAMEDATA->getDiamondNum()).asString());
	m_myscore_label->setString(Value(GAMEDATA->getCurSocre()).asString());

	m_bomb_label->setString(Value(GAMEDATA->getBombNum()).asString());
	m_paint_label->setString(Value(GAMEDATA->getPaintNum()).asString());
	m_rainbow_label->setString(Value(GAMEDATA->getRanbowNum()).asString());
}

void UILayer::refrishUI()
{
	int curlevel = GAMEDATA->getCurLevel();

	m_highscore_label->setString(Value(GAMEDATA->getHistoryScore()).asString());
	m_level_label->setString(Value(GAMEDATA->getCurLevel()).asString());
	m_targetScore_label->setString(Value(GAMEDATA->getTargetScore(curlevel)).asString());
	m_diamond_label->setString(Value(GAMEDATA->getDiamondNum()).asString());
	m_myscore_label->setString(Value(GAMEDATA->getCurSocre()).asString());

	m_bomb_label->setString(Value(GAMEDATA->getBombNum()).asString());
	m_paint_label->setString(Value(GAMEDATA->getPaintNum()).asString());
	m_rainbow_label->setString(Value(GAMEDATA->getRanbowNum()).asString());

}

void UILayer::buyBtnCallback(Ref* sender)
{
	Audio::getInstance()->onClickBtn();
	EventCustom event(SHOPLAYER);
	_eventDispatcher->dispatchEvent(&event);
}

void UILayer::pauseBtnCallback(Ref* sender)
{
	Audio::getInstance()->onClickBtn();
	EventCustom event(PAUSELAYER);
	_eventDispatcher->dispatchEvent(&event);

}

void UILayer::bombBtnCallback(Ref* sender)
{
	Audio::getInstance()->onClickBtn();
	if (GAMEDATA->getBombNum() > 0)
	{
		EventCustom event(ISBOOM);
		_eventDispatcher->dispatchEvent(&event);
	}
	else
	{
		if (GAMEDATA->getDiamondNum()>=150)
		{
			GAMEDATA->setBombNum(10);
			GAMEDATA->setDiamondNum(GAMEDATA->getDiamondNum() - 150); //使用150个购买道具
			refrishUI();
			std::cout << "刷新成功" << std::endl;
		}
		else
		{
			//create shop 
			Audio::getInstance()->onClickBtn();
			EventCustom event(BUYDIAMLAYER);
			_eventDispatcher->dispatchEvent(&event);

		}
	}

}

void UILayer::paintBtnCallback(Ref* sender)
{
	Audio::getInstance()->onClickBtn();
	//染色
	log("paint_num");
	if (GAMEDATA->getPaintNum()>0)
	{
		EventCustom event(ISCHANGE);
		_eventDispatcher->dispatchEvent(&event);
	}
	else
	{
		cout << "没有道具了" << endl;
		if (GAMEDATA->getDiamondNum()>=150)
		{
			GAMEDATA->setPaintNum(10);
			GAMEDATA->setDiamondNum(GAMEDATA->getDiamondNum() - 150); //使用150个购买道具
			refrishUI();
			std::cout << "刷新成功" << std::endl;
		}
		else
		{
			//create shop
			Audio::getInstance()->onClickBtn();
			EventCustom event(BUYDIAMLAYER);
			_eventDispatcher->dispatchEvent(&event);
		}
	}
}

void UILayer::rainbowBtnCallback(Ref* sender)
{
	Audio::getInstance()->onClickBtn();
	//刷新数据
	int rainbow_num = GAMEDATA->getRanbowNum();

	if (rainbow_num>0)
	{
		EventCustom event(RESETSTARPOS);
		_eventDispatcher->dispatchEvent(&event);

		GAMEDATA->setRanbowNum(rainbow_num - 1);
		refrishUI();
		std::cout << "刷新成功" << std::endl;
	}
	else
	{
		std::cout << "道具不足" << std::endl;
		//检测钻石，并且自动够买10个
		if (GAMEDATA->getDiamondNum()>=150)
		{
			GAMEDATA->setRanbowNum(10); //增加道具10个
			GAMEDATA->setDiamondNum(GAMEDATA->getDiamondNum() - 150); //使用150个购买道具
			refrishUI(); //刷新
		}
		else
		{
			//create shop
			Audio::getInstance()->onClickBtn();
			EventCustom event(BUYDIAMLAYER);
			_eventDispatcher->dispatchEvent(&event);
		}
	}

}