#include "FloatWord.h"
#include "CommonFunction.h"

FloatWord* FloatWord::create(const std::string& word, const int fontSize, Point begin)
{
	FloatWord* ret = new FloatWord();
	if (ret && ret->init(word, fontSize, begin))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool FloatWord::init(const std::string& word,const int fontSize, Point begin)
{
	if (!Node::init())
	{
		return false;
	}
	_begin = begin;
	Size visibleSize = Director::getInstance()->getVisibleSize();

	_label = Label::create(word, "Verdana-Bold", fontSize);
	_label->setPosition(begin);
	this->addChild(_label);

	return true;
}

void FloatWord::floatIn(const float delay, std::function<void()> callback)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	MoveTo* move = MoveTo::create(delay, Point(visibleSize.width / 2, _begin.y));

	CallFunc* call = CallFunc::create(callback);

	Sequence* action = Sequence::create(move, call, NULL);

	_label->runAction(action);
}

void FloatWord::floatOut(const float delay, std::function<void()> callback)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//MoveTo* move = MoveTo::create(delay, Point(0, _begin.y));

	auto fade = FadeOut::create(delay);

	CallFunc* call = CallFunc::create(callback);

	CallFunc* removeC = CallFunc::create([this](){
		this->removeFromParentAndCleanup(true);
	});
	Sequence* action = Sequence::create(fade, call, removeC, NULL);
	_label->runAction(action);
}

void FloatWord::floatInOut(const float speed, const float delayTime, std::function<void()> callback)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	MoveTo* moveIn = MoveTo::create(speed, Point(visibleSize.width / 2, _begin.y));

	//MoveTo* moveOut = MoveTo::create(speed, Point(0, _begin.y));
	auto moveOut = FadeOut::create(0.5f);

	CallFunc* removeC = CallFunc::create([this](){
		this->removeFromParentAndCleanup(true);
	});
	Sequence* action = Sequence::create(moveIn, DelayTime::create(delayTime), moveOut, removeC, CallFunc::create(callback), NULL);

	_label->runAction(action);
}