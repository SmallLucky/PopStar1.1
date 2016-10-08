#include "PauseLayer.h"
#include "WelcomeScene.h"
#include "CommonFunction.h"
#include "Constant.h"
#include"Audio.h"
#include "GameScene.h"
#include "GameData.h"
#include "DataSave.h"

PauseLayer::PauseLayer()
{

}

PauseLayer::~PauseLayer()
{

}

bool PauseLayer::init()
{
	if (!TopDownLayer::init())
	{
		return false;
	}
	addUI();
	return true;
}

void PauseLayer::addUI()
{
	//继续游戏
	Button* resume_btn = Button::create("menu_continue.png");
	resume_btn->setPosition(CommonFunction::getVisibleAchor(0.5,0.5,this,Vec2(0,resume_btn->getContentSize().height - 60)));
	resume_btn->addClickEventListener(CC_CALLBACK_1(PauseLayer::resumeGameCbk, this));
	m_popNode->addChild(resume_btn);

	//保存退出
	Button* back_btn = Button::create("save.png");
	back_btn->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, resume_btn,Vec2(0,-back_btn->getContentSize().height * 2)));
	back_btn->addClickEventListener(CC_CALLBACK_1(PauseLayer::backGameCbk, this));
	resume_btn->addChild(back_btn);

	//新游戏
	auto newGame_btn = Button::create("menu_start.png");
	newGame_btn->addClickEventListener(CC_CALLBACK_1(PauseLayer::newGameCbk, this));
	newGame_btn->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, resume_btn, Vec2(0, newGame_btn->getContentSize().height)));

	resume_btn->addChild(newGame_btn);

	//商城
	auto shop_btn = Button::create("menu_shop.png");
	shop_btn->addClickEventListener(CC_CALLBACK_1(PauseLayer::shopCbk, this));
	shop_btn->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, resume_btn, Vec2(0, -shop_btn->getContentSize().height)));
	resume_btn->addChild(shop_btn);

	auto path = MyPathFun(0, shop_btn->getContentSize().height - 20, shop_btn->getContentSize().width - 15);
	auto _emitter2 = particleInit();

	_emitter2->setPosition(Point(shop_btn->getPosition().x - 265, shop_btn->getPosition().y -42));

	_emitter2->runAction(path);

}

void PauseLayer::resumeGameCbk(Ref *sender)
{
	Audio::getInstance()->onClickBtn();
	close();
}

void PauseLayer::backGameCbk(Ref *sender)
{
	Audio::getInstance()->onClickBtn();
	//auto s = WelcomeScene::createScene();
	//Director::getInstance()->replaceScene(TransitionFade::create(0.5, s));

	UserDefault::getInstance()->setBoolForKey(ISREWARDCOME, false);

	//发个消息，StarMatrix层接受消息保存数据
	EventCustom event(SAVEDATA);
	_eventDispatcher->dispatchEvent(&event);
	close();

	//退出游戏
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	Director::getInstance()->end();
}

void PauseLayer::onEnter()
{
	TopDownLayer::onEnter();

}

void PauseLayer::newGameCbk(cocos2d::Ref* pSender)
{
	Audio::getInstance()->onClickBtn();

	GameData::getInstance()->setCurScore(0);
	GameData::getInstance()->setCurLevel(1);
	GameData::getInstance()->setOldScore(0);
	GameData::getInstance()->praseJsonData();

	//清除数据
	DataSava::getInstance()->clearJsonData();

	_eventDispatcher->dispatchCustomEvent(REFRISHUILAYER);
	_eventDispatcher->dispatchCustomEvent(CLEANSTAR);
	_eventDispatcher->dispatchCustomEvent(GOTONEXTLEVEL);

	close();
}

void PauseLayer::shopCbk(cocos2d::Ref* pSender)
{
	EventCustom event(SHOPLAYER);
	_eventDispatcher->dispatchEvent(&event);
	Audio::getInstance()->onClickBtn();
}

RepeatForever* PauseLayer::MyPathFun(float controlX, float controlY, float w)
{
	ccBezierConfig bezier1;
	bezier1.controlPoint_1 = Point(-controlX, 0);
	bezier1.controlPoint_2 = Point(-controlX, controlY);
	bezier1.endPosition = Point(0, controlY);
	auto bezierBy1 = BezierBy::create(0.8f, bezier1);

	auto move1 = MoveBy::create(0.8f, Point(w, 0));

	ccBezierConfig bezier2;
	bezier2.controlPoint_1 = Point(controlX, 0);
	bezier2.controlPoint_2 = Point(controlX, -controlY);
	bezier2.endPosition = Point(0, -controlY);
	auto bezierBy2 = BezierBy::create(0.8f, bezier2);
	auto move2 = MoveBy::create(0.8f, Point(-w, 0));
	auto path = RepeatForever::create(Sequence::create(bezierBy1, move1, bezierBy2, move2, NULL));
	return path;
}

ParticleSystem* PauseLayer::particleInit()
{
	auto _emitter = new ParticleSystemQuad();
	_emitter->initWithTotalParticles(50);
	addChild(_emitter, 10);
	_emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("particle/point.png"));
	_emitter->setAnchorPoint(Point(0, 0));
	// duration
	_emitter->setDuration(ParticleSystem::DURATION_INFINITY);

	// radius mode
	_emitter->setEmitterMode(ParticleSystem::Mode::RADIUS);

	// radius mode: start and end radius in pixels
	_emitter->setStartRadius(4);
	_emitter->setStartRadiusVar(1);
	_emitter->setEndRadius(ParticleSystem::START_RADIUS_EQUAL_TO_END_RADIUS);
	_emitter->setEndRadiusVar(0);

	// radius mode: degrees per second
	_emitter->setRotatePerSecond(100);
	_emitter->setRotatePerSecondVar(0);

	// angle
	_emitter->setAngle(90);
	_emitter->setAngleVar(0);

	// emitter position
	auto size = Director::getInstance()->getWinSize();
	_emitter->setPosVar(Point::ZERO);

	// life of particles
	_emitter->setLife(0.5);
	_emitter->setLifeVar(0);

	// spin of particles
	_emitter->setStartSpin(0);
	_emitter->setStartSpinVar(0);
	_emitter->setEndSpin(0);
	_emitter->setEndSpinVar(0);

	// color of particles
	Color4F startColor(1.0f, 1.0f, 1.0f, 1.0f);
	_emitter->setStartColor(startColor);

	Color4F startColorVar(0, 0, 0, 1.0f);
	_emitter->setStartColorVar(startColorVar);

	Color4F endColor(1.0f, 1.0f, 1.0f, 1.0f);
	_emitter->setEndColor(endColor);

	Color4F endColorVar(0, 0, 0, 0.1f);
	_emitter->setEndColorVar(endColorVar);
	Color4F setStartColor(Color4F(Color4B(50, 50, 50, 50)));
	Color4F setEndColor(Color4F(Color4B(0, 0, 0, 0)));
	// size, in pixels
	_emitter->setStartSize(20);
	_emitter->setStartSizeVar(1);
	_emitter->setEndSize(0);

	// emits per second
	_emitter->setEmissionRate(_emitter->getTotalParticles() / _emitter->getLife());

	// additive
	_emitter->setBlendAdditive(false);

	return _emitter;
}