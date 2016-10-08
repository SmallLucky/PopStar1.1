#pragma  once

#include "cocos2d.h"
using namespace cocos2d;

class TouchSwallowLayer :public Layer
{
public:
	CREATE_FUNC(TouchSwallowLayer);
	virtual bool init();
};
