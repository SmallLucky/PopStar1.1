#include "LoginRewordLayer.h"
#include "CommonFunction.h"
#include "Audio.h"
#include "GameData.h"
#include "Constant.h"
#include "DataSave.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include <android/log.h>
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

#endif

LoginRewordLayer::LoginRewordLayer()
{

}

LoginRewordLayer::~LoginRewordLayer()
{

}

LoginRewordLayer* LoginRewordLayer::create(int eventId)
{
	LoginRewordLayer *pRet = new(std::nothrow) LoginRewordLayer();

	if (pRet && pRet->init(eventId))
	{ 
		pRet->autorelease(); 
		return pRet; 
	} 
	else 
	{ 
		delete pRet; 
		pRet = nullptr; 
		return nullptr; 
	} 
}

bool LoginRewordLayer::init(int eventId)
{
	if (!DownTopLayer::init())
	{
		return false;
	}

	addUI(eventId);

	return true;
}

void LoginRewordLayer::onEnter()
{
	DownTopLayer::onEnter();
}

void LoginRewordLayer::addUI(int eventId)
{
	
}

void LoginRewordLayer::closeBtnCbk(Ref *sender)
{

}

void LoginRewordLayer::buyBtnCbk(Ref *sender)
{

}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

extern"C"
{
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_PayService_eventSuccess(JNIEnv* env, jobject thiz, jint eventId, jintArray protId, jintArray propNum)
	{
		// id   propId   propNum
		LOGD("......eventSuccess_eventId=%d", eventId);

		jsize len = (env)->GetArrayLength(protId);
		jint *protId_body = (env)->GetIntArrayElements(protId, 0);
		jint *propNum_body = (env)->GetIntArrayElements(propNum, 0);

		// 登录礼包    10元    100 赠送151 =251 钻石 （领取以后不再出现）
		if(eventId == 1)
		{
			LOGD(".........eventId==1");
			for (int i = 0; i<len; i++)
			{
				if (protId_body[i] == 1)
				{
					int dimNum = GameData::getInstance()->getDiamondNum();
					GameData::getInstance()->setDiamondNum(dimNum + propNum_body[i]);
					LOGD("....dimNum=%d",propNum_body[i]);
				}
			}


			int localId = UserDefault::getInstance()->getIntegerForKey(LOGINREWARD);

			UserDefault::getInstance()->setIntegerForKey(LOGINREWARD, localId + 1);

			UserDefault::getInstance()->flush();

			LOGD("......localId+1,localId=%d", UserDefault::getInstance()->getIntegerForKey(LOGINREWARD));

			//发送消息到UI层
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(REFRISHUILAYER);
		}
		/*
		 第一关通关礼包  10元   120钻石,三种道具各3个 
		 （每次通关第一关都出现，领完以后第一关不再出现任何礼包）
		 */
		if(eventId == 2)
		{
			LOGD(".........eventId==2");
			for (int i = 0; i<len; i++)
			{
				if (protId_body[i] == 1)
				{
					int dimNum = GameData::getInstance()->getDiamondNum();
					GameData::getInstance()->setDiamondNum(dimNum + propNum_body[i]);
					LOGD("....dimNum=%d",propNum_body[i]);
				}
				if (protId_body[i] == 2)
				{
					int bombNum = GameData::getInstance()->getBombNum();
					GameData::getInstance()->setBombNum(bombNum + propNum_body[i]);
					LOGD("....bombNum=%d",propNum_body[i]);
				}
				if (protId_body[i] == 3)
				{
					int paintNum = GameData::getInstance()->getPaintNum();
					GameData::getInstance()->setPaintNum(paintNum + propNum_body[i]);
					LOGD("....paintNum=%d",propNum_body[i]);
				}
				if (protId_body[i] == 4)
				{
					int rainbNum = GameData::getInstance()->getRanbowNum();
					GameData::getInstance()->setRanbowNum(rainbNum + propNum_body[i]);
					LOGD("....rainNum=%d",propNum_body[i]);
				}
			}

			int reward_one = UserDefault::getInstance()->getIntegerForKey(REWARD_LEVEL_ONE);
			UserDefault::getInstance()->setIntegerForKey(REWARD_LEVEL_ONE, reward_one + 1);
			UserDefault::getInstance()->flush();
			LOGD("......reward_one+1,reward_one=%d", UserDefault::getInstance()->getIntegerForKey(REWARD_LEVEL_ONE));

			//发送消息到UI层
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(REFRISHUILAYER);
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(GOTONEXTLEVEL);
		}

		/*
			第二关通关礼包  0元    30钻石  
			（领取以后不再出现，回到正常通关或失败礼包）
			*/

		if(eventId == 3)
		{
			LOGD(".........eventId==3");
			for (int i = 0; i<len; i++)
			{
				if (protId_body[i] == 1)
				{
					int dimNum = GameData::getInstance()->getDiamondNum();
					GameData::getInstance()->setDiamondNum(dimNum + propNum_body[i]);
					LOGD("....dimNum=%d",propNum_body[i]);
				}
			}

			int reward_two = UserDefault::getInstance()->getIntegerForKey(REWARD_LEVEL_TWO);
			UserDefault::getInstance()->setIntegerForKey(REWARD_LEVEL_TWO, reward_two + 1);
			UserDefault::getInstance()->flush();
			LOGD("......reward_two+1,reward_two=%d", UserDefault::getInstance()->getIntegerForKey(REWARD_LEVEL_TWO));

			//发送消息到UI层
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(REFRISHUILAYER);
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(GOTONEXTLEVEL);
		}

		/*
			第二关失败礼包  0元      
			三种道具各1个（领取以后不再出现，回到正常通关或失败礼包）
			*/
		if(eventId == 4)
		{
			LOGD(".........eventId==4");
			for (int i = 0; i<len; i++)
			{
				if (protId_body[i] == 1)
				{
					int dimNum = GameData::getInstance()->getDiamondNum();
					GameData::getInstance()->setDiamondNum(dimNum + propNum_body[i]);
					LOGD("....dimNum=%d",propNum_body[i]);
				}
				if (protId_body[i] == 2)
				{
					int bombNum = GameData::getInstance()->getBombNum();
					GameData::getInstance()->setBombNum(bombNum + propNum_body[i]);
					LOGD("....bombNum=%d",propNum_body[i]);
				}
				if (protId_body[i] == 3)
				{
					int paintNum = GameData::getInstance()->getPaintNum();
					GameData::getInstance()->setPaintNum(paintNum + propNum_body[i]);
					LOGD("....paintNum=%d",propNum_body[i]);
				}
				if (protId_body[i] == 4)
				{
					int rainbNum = GameData::getInstance()->getRanbowNum();
					GameData::getInstance()->setRanbowNum(rainbNum + propNum_body[i]);
					LOGD("....rainNum=%d",propNum_body[i]);
				}
			}

			int reward_two_fail = UserDefault::getInstance()->getIntegerForKey(REWARD_LEVEL_TWO_FAIL);
			UserDefault::getInstance()->setIntegerForKey(REWARD_LEVEL_TWO_FAIL, reward_two_fail + 1);
			UserDefault::getInstance()->flush();
			LOGD("......reward_two_fail+1,reward_two_fail=%d", UserDefault::getInstance()->getIntegerForKey(REWARD_LEVEL_TWO_FAIL));

			//发送消息到UI层
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(GOTONEXTLEVEL);
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(REFRISHUILAYER);
		}

		/*
			关卡失败礼包（钻石不足30）     
			10元      220钻石 
			*/
		if(eventId == 5)
		{
			LOGD(".........eventId==5");
			for (int i = 0; i<len; i++)
			{
				if (protId_body[i] == 1)
				{
					int dimNum = GameData::getInstance()->getDiamondNum();
					GameData::getInstance()->setDiamondNum(dimNum + propNum_body[i]);
					LOGD("....dimNum=%d",propNum_body[i]);
				}
			}
			//发送消息到UI层
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(GOTONEXTLEVEL);
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(REFRISHUILAYER);
		}

		/*
			关卡胜利礼包    
			10元   50钻石,三种道具各5个 
			*/

		if(eventId == 6)
		{
			LOGD(".........eventId==6");
			for (int i = 0; i<len; i++)
			{
				if (protId_body[i] == 1)
				{
					int dimNum = GameData::getInstance()->getDiamondNum();
					GameData::getInstance()->setDiamondNum(dimNum + propNum_body[i]);
					LOGD("....dimNum=%d",propNum_body[i]);
				}
				if (protId_body[i] == 2)
				{
					int bombNum = GameData::getInstance()->getBombNum();
					GameData::getInstance()->setBombNum(bombNum + propNum_body[i]);
					LOGD("....bombNum=%d",propNum_body[i]);
				}
				if (protId_body[i] == 3)
				{
					int paintNum = GameData::getInstance()->getPaintNum();
					GameData::getInstance()->setPaintNum(paintNum + propNum_body[i]);
					LOGD("....paintNum=%d",propNum_body[i]);
				}
				if (protId_body[i] == 4)
				{
					int rainbNum = GameData::getInstance()->getRanbowNum();
					GameData::getInstance()->setRanbowNum(rainbNum + propNum_body[i]);
					LOGD("....rainNum=%d",propNum_body[i]);
				}
			}
			//发送消息到UI层
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(REFRISHUILAYER);
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(GOTONEXTLEVEL);
		}

		//道具不足，购买道具，10元3种道具各7个
		if(eventId == 7)
		{
			LOGD(".........eventId==7");
			for (int i = 0; i<len; i++)
			{
				if (protId_body[i] == 1)
				{
					int dimNum = GameData::getInstance()->getDiamondNum();
					GameData::getInstance()->setDiamondNum(dimNum + propNum_body[i]);
					LOGD("....dimNum=%d",propNum_body[i]);
				}
				if (protId_body[i] == 2)
				{
					int bombNum = GameData::getInstance()->getBombNum();
					GameData::getInstance()->setBombNum(bombNum + propNum_body[i]);
					LOGD("....bombNum=%d",propNum_body[i]);
				}
				if (protId_body[i] == 3)
				{
					int paintNum = GameData::getInstance()->getPaintNum();
					GameData::getInstance()->setPaintNum(paintNum + propNum_body[i]);
					LOGD("....paintNum=%d",propNum_body[i]);
				}
				if (protId_body[i] == 4)
				{
					int rainbNum = GameData::getInstance()->getRanbowNum();
					GameData::getInstance()->setRanbowNum(rainbNum + propNum_body[i]);
					LOGD("....rainNum=%d",propNum_body[i]);
				}
			}
			//发送消息到UI层
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(REFRISHUILAYER);
		}

		//商店购买道具
		for (int i = 8; i <= 13; i++)
		{
			if (eventId == i)
			{
				for (int j = 0; j< len; j++)
				{
					if (protId_body[j] == 1)
					{
						int dimNum = GameData::getInstance()->getDiamondNum();
						GameData::getInstance()->setDiamondNum(dimNum + propNum_body[j]);
						LOGD("....dimNum=%d",propNum_body[j]);

					}
					else if (protId_body[j] == 2)
					{
						int bombNum = GameData::getInstance()->getBombNum();
						GameData::getInstance()->setBombNum(bombNum + propNum_body[j]);
						LOGD("....bombNum=%d",propNum_body[j]);
					}
					else if (protId_body[j] == 3)
					{
						int paintNum = GameData::getInstance()->getPaintNum();
						GameData::getInstance()->setPaintNum(paintNum + propNum_body[j]);
						LOGD("....paintNum=%d",propNum_body[j]);
					}
					else if (protId_body[j] == 4)
					{
						int rainbNum = GameData::getInstance()->getRanbowNum();
						GameData::getInstance()->setRanbowNum(rainbNum + propNum_body[j]);
						LOGD("....rainbNum=%d",propNum_body[j]);
					}
					//发送消息到shop层
					Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(REFRIEHSHOPUI);
					//发送消息到UI层
					Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(REFRISHUILAYER);
				}
			}
		}

		/*
			关卡失败礼包（钻石>=30）   
			扣除30钻石，确定下一关，取消到第一关 
			*/
		if(eventId == 14)
		{
			LOGD(".........eventId==14");
			//发送消息到UI层

			int dimNum = GameData::getInstance()->getDiamondNum();
			GameData::getInstance()->setDiamondNum(dimNum -30);

			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(GOTONEXTLEVEL);
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(REFRISHUILAYER);
		}

		(env)->ReleaseIntArrayElements(propNum, protId_body, 0);
		(env)->ReleaseIntArrayElements(propNum, propNum_body, 0);
	}

	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_PayService_eventFail(JNIEnv* env, jobject thiz, jstring eventId)
	{
		std::string event_Id = JniHelper::jstring2string(eventId);

		int id = atoi(event_Id.c_str());

		LOGD(".......eventFail_Id=%d", id);

		if (id == 2 || id == 3 || id == 6 )
		{
			//下一关
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(GOTONEXTLEVEL);
		}

		if(id == 4 || id == 5 || id ==14)
		{
			//回到第一关
			GameData::getInstance()->setCurScore(0);
			GameData::getInstance()->setCurLevel(1);
			GameData::getInstance()->setOldScore(0);
			GameData::getInstance()->praseJsonData();
			//清除数据
			DataSava::getInstance()->clearJsonData();
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(REFRISHUILAYER);
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CLEANSTAR);
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(GOTONEXTLEVEL);
		}
	}

	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_PayService_eventClose(JNIEnv* env, jobject thiz, jstring eventId)
	{
		std::string event_Id = JniHelper::jstring2string(eventId);

		LOGD(".......eventClose_Id=%d", atoi(event_Id.c_str()));
	}
}
#endif