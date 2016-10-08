#pragma  once

#include "cocos2d.h"
#include <iostream>
#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "TopDownLayer.h"

using namespace std;
using namespace cocos2d;
using namespace ui;

class ShopLayer :public TopDownLayer
{
public:

	ShopLayer();
	~ShopLayer();
	CREATE_FUNC(ShopLayer);
	virtual bool init();
	virtual void onEnter();

public:

	void addUI();
	void closeCbk(Ref *sender);
	
	void refrishUI();
	void setUserId(int userid);
private:
	Label* diaNum_text;
	Label* _userId;
};
