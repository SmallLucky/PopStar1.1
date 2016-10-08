#pragma  once

#include "cocos2d.h"
#include <iostream>
#include "ui/UIText.h"
#include "ui/UIButton.h"

USING_NS_CC;

class ChangeStarColor : public Layer
{
public:

	virtual bool init();

	Size visbleSize;

	void createChangeBox(Point p);
	void StarButtonCallBack(Ref* ref);
	void returnButtonCallBack(Ref* ref);

	virtual void onEnter();
	~ChangeStarColor();
	ChangeStarColor();

	CREATE_FUNC(ChangeStarColor);

};