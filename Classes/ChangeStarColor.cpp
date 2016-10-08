
#include "CommonFunction.h"
#include "ChangeStarColor.h"
#include "ui/UIButton.h"
#include "Constant.h"
#include "GameData.h"

using namespace ui;

ChangeStarColor::~ChangeStarColor()
{
}
ChangeStarColor::ChangeStarColor()
{

}

bool ChangeStarColor:: init()
{
	if (!Layer ::init())
	{
		return false;
	}
	visbleSize = Director::getInstance()->getVisibleSize();
	return true;
}

void  ChangeStarColor::onEnter()
{
	Layer::onEnter();
	// 屏蔽触摸
	
	auto myListener = EventListenerTouchOneByOne::create();
	myListener->setSwallowTouches(true);

	myListener->onTouchBegan = [](Touch *pTouch, Event *pEvent){
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, this);
	
}

void ChangeStarColor:: createChangeBox(Point p)
{
	Sprite* pointer = Sprite:: create("paint_arrow.png");
	pointer->setAnchorPoint(Vec2(0.5, 0));
	pointer->setPosition(p);
	addChild(pointer);

	Sprite * boxBG = Sprite::create("paint_bg.png");
	boxBG->setAnchorPoint(Vec2(0.5, 0.5));
	boxBG->setPosition(visbleSize.width / 2, p.y + 100);
	addChild(boxBG);

	Button * star;
	for (int i = 1; i <= 5; i++)
	{
		star = Button::create(StringUtils::format("star%d.png", i));
		float x = i*0.15;
		star->setPosition(CommonFunction::getVisibleAchor(x, 0.5, boxBG));
		boxBG->addChild(star);
		
		star->addClickEventListener(CC_CALLBACK_1(ChangeStarColor::StarButtonCallBack, this));
		star->setUserData(*(int**)&i);
	}

	Button * backBT = Button::create("paint_back.png");
	backBT->setPosition(CommonFunction::getVisibleAchor(0.9, 0.5, boxBG));
	boxBG->addChild(backBT);
	backBT->addClickEventListener(CC_CALLBACK_1(ChangeStarColor::returnButtonCallBack, this));

}

void ChangeStarColor::StarButtonCallBack(Ref* ref)
{
	if (!ref)
	{
		return;
	}
	Button *  star = static_cast<Button*> (ref);
	void* p = star->getUserData();
	int indext = *(int*)(&p);
	int paintNum = GameData::getInstance()->getPaintNum();
	switch (indext)
	{
	case 1:
	{
			  if (paintNum>0)
			  {
				  GameData::getInstance()->setPaintNum(paintNum - 1);
				  EventCustom event_refrishUI(REFRISHUILAYER);
				  _eventDispatcher->dispatchEvent(&event_refrishUI);

				  EventCustom event(CHANGE_1);
				  _eventDispatcher->dispatchEvent(&event);
			  }
			  else
			  {
				  cout << "没有道具了" << endl;
				  GameData::getInstance()->setPaintNum(10);
				  GameData::getInstance()->setDiamondNum(GameData::getInstance()->getDiamondNum() - 150); //使用150个购买道具

				  EventCustom event_refrishUI(REFRISHUILAYER);
				  _eventDispatcher->dispatchEvent(&event_refrishUI);
			  }
			

	}

		break;
	case 2:
	{
			  if (paintNum>0)
			  {
				  GameData::getInstance()->setPaintNum(paintNum - 1);
				  EventCustom event_refrishUI(REFRISHUILAYER);
				  _eventDispatcher->dispatchEvent(&event_refrishUI);

				  EventCustom event(CHANGE_2);
				  _eventDispatcher->dispatchEvent(&event);
			  }
			  else
			  {
				  cout << "没有道具了" << endl;
			  }
	}

		break;
	case 3:
	{
			  if (paintNum>0)
			  {
				  GameData::getInstance()->setPaintNum(paintNum - 1);
				  EventCustom event_refrishUI(REFRISHUILAYER);
				  _eventDispatcher->dispatchEvent(&event_refrishUI);

				  EventCustom event(CHANGE_3);
				  _eventDispatcher->dispatchEvent(&event);
			  }
			  else
			  {
				  cout << "没有道具了" << endl;

			  }
	}

		break;
	case 4:
	{
			  if (paintNum>0)
			  {
				  GameData::getInstance()->setPaintNum(paintNum - 1);
				  EventCustom event_refrishUI(REFRISHUILAYER);
				  _eventDispatcher->dispatchEvent(&event_refrishUI);

				  EventCustom event(CHANGE_4);
				  _eventDispatcher->dispatchEvent(&event);
			  }
			  else
			  {
				  cout << "没有道具了" << endl;
			  }
	}

		break;
	case 5:
	{
			  if (paintNum>0)
			  {
				  GameData::getInstance()->setPaintNum(paintNum - 1);
				  EventCustom event_refrishUI(REFRISHUILAYER);
				  _eventDispatcher->dispatchEvent(&event_refrishUI);

				  EventCustom event(CHANGE_5);
				  _eventDispatcher->dispatchEvent(&event);
			  }
			  else
			  {
				  cout << "没有道具了" << endl;

			  }
	}

		break;
	default:
		break;
	}

	if (getParent())
	{
		this->removeFromParent();
	}
}

void ChangeStarColor:: returnButtonCallBack(Ref* ref)
{
	//移除层
	if (!ref)
	{
		return;
	}
	if (getParent())
	{
		this->removeFromParent();
	}
}