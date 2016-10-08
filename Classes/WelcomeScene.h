#pragma  once

#include "cocos2d.h"
#include <iostream>
#include "TopDownLayer.h"

using namespace std;
using namespace cocos2d;

class WelcomeScene : public Layer
{
public:

	WelcomeScene();
	~WelcomeScene();

	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(WelcomeScene);
	void onEnter() override;

	virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);

public:
	void addUI();

public:

	void newGameCbk(cocos2d::Ref* pSender);
	void continueGameCbk(cocos2d::Ref* pSender);
	void shopCbk(cocos2d::Ref* pSender);

	void musicCbk(cocos2d::Ref* pSender);
	void helpCbk(cocos2d::Ref* pSender);

private:

};

