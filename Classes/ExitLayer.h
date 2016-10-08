#pragma  once

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "PopLayer.h"

using namespace cocos2d;
using namespace ui;

class ExitLayer :public PopLayer
{
public:

	ExitLayer();
	~ExitLayer();
	CREATE_FUNC(ExitLayer);
	virtual bool init();

	void onEnter() override;
	void addUI();

	void exitBtnCbk(Ref *sender);
	void buyBtnCbk(Ref *sender);
	void closeBtnCbk(Ref *sender);

private:

};
