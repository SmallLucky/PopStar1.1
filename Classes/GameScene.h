#pragma  once

#include "cocos2d.h"
#include "GameLayer.h"

using namespace cocos2d;

class GameScene:public Layer
{
public:
	GameScene();
	~GameScene();

	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);

public:
	void onEnter() override;
	void onExit() override;

public:

};
