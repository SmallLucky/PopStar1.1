#include "TouchSwallowLayer.h"

bool TouchSwallowLayer::init()
{
	if (!Node::init())
	{
		return false;
	}

	auto winSize = Director::getInstance()->getVisibleSize();
	auto layercolor = LayerColor::create(Color4B::BLACK);
	layercolor->setOpacity(0);
	addChild(layercolor);

	auto myListener = EventListenerTouchOneByOne::create();
	myListener->setSwallowTouches(true);
	myListener->onTouchBegan = [](Touch*, Event*){return true;};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, this);
	//_eventDispatcher->setPriority(myListener, -128);
	return true;
}