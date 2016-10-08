#pragma  once

#include "cocos2d.h"
#include <iostream>
#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "DownTopLayer.h"

using namespace std;
using namespace cocos2d;
using namespace ui;

class GameOverLayer :public DownTopLayer
{
public:

	GameOverLayer();
	~GameOverLayer();
	CREATE_FUNC(GameOverLayer);
	virtual bool init();
	void onEnter() override;
	void addUI();

	void continueCbk(Ref *sender);

	void closeBtnCbk(Ref *sender);

	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);

	void SetTime(float dt);	//倒计时

	int timeCount;

private:

	Label* _time;
};