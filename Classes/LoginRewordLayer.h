#pragma  once

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "DownTopLayer.h"

using namespace cocos2d;
using namespace ui;

/*
登录奖励层
*/

class LoginRewordLayer :public DownTopLayer
{
public:

	LoginRewordLayer();
	~LoginRewordLayer();
	static LoginRewordLayer* create(int eventId);
	virtual bool init(int eventId);

	void onEnter() override;
	void addUI(int eventId);
	void closeBtnCbk(Ref *sender);
	void buyBtnCbk(Ref *sender);
};
