#pragma once
#include <iostream>

//层次 zorder
const int UILAYER_ZORDER =10;
const int POUSELAYER_ZORDER = 30;
const int ADLAYER_ZORDER = 20;
const int REWAEDLAYER_ZORDER = 20;
const int STARMATRIX_ZORDER = 15;
const int CHANGESTAR_ZORDER = 15;
const int SHOPLAYER_ZORDER = 35;
const int BUYDIAMLAYER_ZORDER = 20;

const int GAMEOVERLAYER_ZORDER = 20;	//gameoverlayer
const int GAMEOVERPOPLLYER_ZORDER = 22;	//gameoverpoplayer
const int GAMEOVERLAYERONE_ZORDER = 25;	//gameonverpoplayerone

const int TOUCHSWALL_ZORDER = 30;		//触摸吞噬层
const int TOUCHSWALL_ZORDER_1 = -1;

/*
	消息名字
*/

#define REFRISHUILAYER "refrishuilayer"	// 刷新UI层的显示

//三个道具
#define RESETSTARPOS "resetStarPos"		//
#define ISBOOM "isbool"					//
#define ISCHANGE "ischange"				//

//层消息
#define PAUSELAYER "pauselayer"			//
#define SHOPLAYER "shoplayer"			//
#define BUYDIAMLAYER "diamondlayer"		//
#define GAMEOVERPOPLAYERONE "gameoverpoplayerone" 
#define GAMEOVERPOPLAYER "gameoverpoplayer"
#define WELCOMESCENE "welcomescene"

#define REFRIEHSHOPUI "refrishShopUI"	//刷新商店UI
#define CLEANSTAR "cleanstar"			//清除星星

//变颜色
#define CHANGE_1 "change_1"				//
#define CHANGE_2 "change_2"				//
#define CHANGE_3 "change_3"				//
#define CHANGE_4 "change_4"				//
#define CHANGE_5 "change_5"				//

#define GOTONEXTLEVEL "gotonextlevel"
#define SAVEDATA "savedata"
#define CREATESTARFROMDATA "createstarfromdata"

//奖励
#define LOGINREWARD "loginReward"	//登录奖励
#define ISREWARDCOME "isRewardCome"	//是否弹出了登录奖励
#define GAMEREWARD "gameReward"		//过关奖励
#define MYDAY "MYDAY"				//当天时间

#define REWARD_LEVEL_ONE	"rewardOne"		//第一关过关奖励
#define REWARD_LEVEL_TWO		"rewardTwo"	//第二关过关奖励
#define REWARD_LEVEL_TWO_FAIL		"rewardTwoFail"//第二关失败奖励

//是否继续游戏
#define ISCONTINUE "ISCONTINUE"

