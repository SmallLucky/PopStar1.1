#pragma  once

#include "cocos2d.h"

#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "CommonFunction.h"
#include "TopDownLayer.h"

using namespace cocos2d;
using namespace ui;

class BuyDimaLayer: public TopDownLayer 
{
public:

	BuyDimaLayer();
	~BuyDimaLayer();

	CREATE_FUNC(BuyDimaLayer);
	virtual bool init();
	virtual void onEnter();

public:

	void addUI();
	void confirmCbk(Ref *sender);
	void closeCbk(Ref *sender);

private:

};