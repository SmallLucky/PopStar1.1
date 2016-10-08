#pragma  once

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "DownTopLayer.h"

using namespace cocos2d;
using namespace ui;

class GameOverPopLayer :public DownTopLayer
{
public:

	GameOverPopLayer();
	~GameOverPopLayer();

	CREATE_FUNC(GameOverPopLayer);

	virtual bool init();

	void onEnter() override;

	void addUI();

	void confirmCbk(Ref *sender);

	void closeCbk(Ref *sender);

public:
	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);

};