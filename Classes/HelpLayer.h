#pragma  once

#include "cocos2d.h"
#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "TopDownLayer.h"

using namespace cocos2d;
using namespace ui;

class HelpLayer :public TopDownLayer
{
public:

	HelpLayer();
	~HelpLayer();
	CREATE_FUNC(HelpLayer);
	virtual bool init();

	void onEnter() override;
	void addUI();
	void closeBtnCbk(Ref *sender);

private:

};
