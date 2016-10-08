#include "RewardLayer.h"
#include "CommonFunction.h"
#include "Constant.h"
#include "Audio.h"
#include "GameData.h"

RewardLayer::RewardLayer()
{

}

RewardLayer::~RewardLayer()
{

}

bool RewardLayer::init()
{
	if (!DownTopLayer::init())
	{
		return false;
	}

	addUI();

	return true;
}

void RewardLayer::onEnter()
{
	DownTopLayer::onEnter();
}

void RewardLayer::addUI()
{
	//背景
	auto bg_sp = Sprite::create("someUI/warningBg_1.png");
	m_popNode->addChild(bg_sp);

	//确定按钮
	auto comfirm_btn = Button::create("someUI/comfirmBtn.png");
	comfirm_btn->addClickEventListener(CC_CALLBACK_1(RewardLayer::buyBtnCbk, this));
	comfirm_btn->setPosition(CommonFunction::getVisibleAchor(0.5, 0, bg_sp, Vec2(0, comfirm_btn->getContentSize().height)));
	bg_sp->addChild(comfirm_btn);

	Sprite* bomb_sp = Sprite::create("Props_Bomb.png");
	bomb_sp->setPosition(CommonFunction::getVisibleAchor(0, 0.5, bg_sp, Vec2(45, -15)));
	bg_sp->addChild(bomb_sp);

	Sprite* paint_sp = Sprite::create("Props_Paint.png");
	paint_sp->setPosition(CommonFunction::getVisibleAchor(0, 0.5, bomb_sp, Vec2(135, 0)));
	bomb_sp->addChild(paint_sp);

	Sprite* rain_sp = Sprite::create("Props_Rainbow.png");
	rain_sp->setPosition(CommonFunction::getVisibleAchor(0, 0.5, paint_sp, Vec2(135, 0)));
	paint_sp->addChild(rain_sp);

	Sprite* word_sp = nullptr;

	int gameRewardId = UserDefault::getInstance()->getIntegerForKey(GAMEREWARD, 6);
	//6 7 8 9 10
	if (gameRewardId == 6)
	{
		word_sp = Sprite::create("someUI/word2.png" );
		paint_sp->setVisible(false);
		rain_sp->setVisible(false);

		bomb_sp->setPosition(CommonFunction::getVisibleAchor(0, 0.5, bg_sp, Vec2(170, -15)));

		auto sp_1 = Sprite::create("someUI/X1.png");
		sp_1->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, bomb_sp, Vec2(60, 0)));
		bomb_sp->addChild(sp_1);
	}
	else if (gameRewardId == 7)
	{
		
		/*word_sp = Sprite::create("someUI/word2.png");
		rain_sp->setVisible(false);

		bomb_sp->setPosition(CommonFunction::getVisibleAchor(0, 0.5, bg_sp, Vec2(105, -15)));

		auto sp_1 = Sprite::create("someUI/X1.png");
		sp_1->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, bomb_sp, Vec2(60, 0)));
		bomb_sp->addChild(sp_1);

		auto sp_2 = Sprite::create("someUI/X1.png");
		sp_2->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, paint_sp, Vec2(60, 0)));
		paint_sp->addChild(sp_2);*/
		
	}
	else if (gameRewardId == 8)
	{
		
		/*word_sp = Sprite::create("someUI/word2.png");

		auto sp_1 = Sprite::create("someUI/X1.png");
		sp_1->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, bomb_sp, Vec2(60, 0)));
		bomb_sp->addChild(sp_1);

		auto sp_2 = Sprite::create("someUI/X1.png");
		sp_2->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, paint_sp, Vec2(60, 0)));
		paint_sp->addChild(sp_2);

		auto sp_3 = Sprite::create("someUI/X1.png");
		sp_3->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, rain_sp, Vec2(60, 0)));
		rain_sp->addChild(sp_3);*/
		
	}
	else if (gameRewardId == 9)
	{
		
		/*word_sp = Sprite::create("someUI/word4.png");

		auto sp_1 = Sprite::create("someUI/X4.png");
		sp_1->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, bomb_sp, Vec2(60, 0)));
		bomb_sp->addChild(sp_1);

		auto sp_2 = Sprite::create("someUI/X4.png");
		sp_2->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, paint_sp, Vec2(60, 0)));
		paint_sp->addChild(sp_2);

		auto sp_3 = Sprite::create("someUI/X4.png");
		sp_3->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, rain_sp, Vec2(60, 0)));
		rain_sp->addChild(sp_3);*/
		
	}
	else if (gameRewardId == 10)
	{
		
		/*word_sp = Sprite::create("someUI/word4.png");

		auto sp_1 = Sprite::create("someUI/X10.png");
		sp_1->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, bomb_sp, Vec2(60, 0)));
		bomb_sp->addChild(sp_1);

		auto sp_2 = Sprite::create("someUI/X10.png");
		sp_2->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, paint_sp, Vec2(60, 0)));
		paint_sp->addChild(sp_2);

		auto sp_3 = Sprite::create("someUI/X10.png");
		sp_3->setPosition(CommonFunction::getVisibleAchor(0.5, 0.5, rain_sp, Vec2(60, 0)));
		rain_sp->addChild(sp_3);*/
		
	}
	if (!word_sp){ return; }

	word_sp->setPosition(CommonFunction::getVisibleAchor(0.5, 1, bg_sp, Vec2(0, -word_sp->getContentSize().height/2 - 30)));
	bg_sp->addChild(word_sp);
}

void RewardLayer::closeBtnCbk(Ref *sender)
{
	Audio::getInstance()->onClickBtn();
	EventCustom event(GOTONEXTLEVEL);
	_eventDispatcher->dispatchEvent(&event);
	close();

}

void RewardLayer::buyBtnCbk(Ref *sender)
{
	Audio::getInstance()->onClickBtn();
	//过关奖励
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

	#include <android/log.h>
	#include <jni.h>
	#include "platform/android/jni/JniHelper.h"

	JniMethodInfo info;
	int localId = UserDefault::getInstance()->getIntegerForKey(GAMEREWARD);

	log(">>>>>>>localId=%d", localId);

	if (localId <= 10)
	{
		if (GameData::getInstance()->getCurLevel()<= 3)
		{
			if (localId <= 8 )
			{
				bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/PayService", "pay", "(I)V");
				if (ret)
				{
					jobject jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID, localId);
					//弹出奖励层
				}
			}
		}else
		{
			bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/PayService", "pay", "(I)V");
			if (ret)
			{
				jobject jobj = info.env->CallStaticObjectMethod(info.classID, info.methodID, localId);
			}
		}
	}

#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

#endif

	EventCustom event(GOTONEXTLEVEL);
	_eventDispatcher->dispatchEvent(&event);
	close();
}

void RewardLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	if (event->getType() == Event::Type::KEYBOARD)
	{
		event->stopPropagation();
		{
			if (keycode == EventKeyboard::KeyCode::KEY_BACK)  //返回
			{
				EventCustom event(GOTONEXTLEVEL);
				_eventDispatcher->dispatchEvent(&event);
				close();
			}
		}
	}
}