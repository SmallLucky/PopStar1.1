#pragma  once

#include "cocos2d.h"
#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "DownTopLayer.h"

using namespace cocos2d;
using namespace ui;

/*
通关奖励层
*/

class RewardLayer :public DownTopLayer
{
public:

	RewardLayer();
	~RewardLayer();
	CREATE_FUNC(RewardLayer);
	virtual bool init();
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);

	void onEnter() override;
	void addUI();
	void closeBtnCbk(Ref *sender);
	void buyBtnCbk(Ref *sender);

};
