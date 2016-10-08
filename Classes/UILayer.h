#pragma  once

#include "cocos2d.h"
#include <iostream>
#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "CommonFunction.h"

using namespace std;
using namespace cocos2d;
using namespace ui;

#define VISIBLESIZE Director::getInstance()->getVisibleSize()

class UILayer :public Layer
{
public:

	UILayer();
	~UILayer();
	virtual bool init();
	CREATE_FUNC(UILayer);

	void onEnter() override;
	void onExit() override;

public:

	void addUI();
	void initUI();
	void refrishUI();

public:
	//分数刷新动画
	void refrishScore(int size);

	void showFontScore(float dt);
	int score;

public:

	void buyBtnCallback(Ref* sender);
	void pauseBtnCallback(Ref* sender);

	void bombBtnCallback(Ref* sender);
	void paintBtnCallback(Ref* sender);
	void rainbowBtnCallback(Ref* sender);

private:

	Label* m_level_label;		//等级
	Label* m_targetScore_label;	//目标分数
	Label* m_diamond_label;		//钻石
	Label* m_myscore_label;		//我的分数
	Label* m_highscore_label;	//最高分

	//三个道具标签
	Label* m_bomb_label;		//炸弹
	Label* m_paint_label;		//染色
	Label* m_rainbow_label;		//重置
};

