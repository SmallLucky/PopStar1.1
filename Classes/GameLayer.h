#pragma  once

#include "cocos2d.h"
#include "FloatWord.h"
#include "StarMatrix.h"
#include "ui/UIText.h"

using namespace cocos2d;

class UILayer;
class StarMatrix;
class TouchSwallowLayer;

class GameLayer : public Layer
{
public:

	virtual bool init();
	CREATE_FUNC(GameLayer);
	GameLayer();
	~GameLayer();

	virtual void onEnter() override;
	virtual void onExit() override;

public:

	void floatLevelWord();

	void showStarMatrix();

	virtual void update(float delta);

	void showLinkNum(int size);

	void hideLinkNum();

	void floatLeftStarMsg(int leftNum);

	void refrishUI();

	void resetLeftStarPos();

	void changeStarColor();

	void runBoomPop();

	void addPauseLayer();

	void addShopLayer();

	void addBuyDiamondLayer();

	virtual bool onTouchBegan(Touch* touch, Event* event);

	virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);

	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);	//返回按钮事件

public:

	void gotoNextLevel();

	void gotoGameOver();

	void gotoGameOverPopLayer();

	void gotoRewardLayer();

	void gotoWelclmeScene();

public:

	void showScore(int size,Point p);//飘字

	UILayer* _uilayer;	//UI层

	TouchSwallowLayer* _swallowLayer;  //触摸吞噬层

private:

	FloatWord* _levelMsg;
	FloatWord* _targetScore;

	StarMatrix* matrix;

	ui::Text* linkNum;

	bool isNextLevel;

	Sprite* _girlSp;
};