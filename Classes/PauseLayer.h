#pragma  once

#include "cocos2d.h"
#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "TopDownLayer.h"

using namespace cocos2d;
using namespace ui;

class PauseLayer :public TopDownLayer
{
public:

	PauseLayer();
	~PauseLayer();
	CREATE_FUNC(PauseLayer);
	virtual bool init();

	void onEnter() override;
	void addUI();

	void resumeGameCbk(Ref *sender);
	void backGameCbk(Ref *sender);

	void shopCbk(cocos2d::Ref* pSender);

	void newGameCbk(cocos2d::Ref* pSender);

private:

	RepeatForever* MyPathFun(float controlX, float controlY, float w);
	ParticleSystem* particleInit();

};
