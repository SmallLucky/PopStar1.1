#include "Audio.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
USING_NS_CC;

#define USERDEFAULT UserDefault::getInstance()

Audio* Audio::m_instance = nullptr;

Audio* Audio::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new Audio();
	}
	return m_instance;
}

void Audio::playBGM()
{
	if (USERDEFAULT->getBoolForKey("ISMUSICON") == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music_des/Fireflies.wma", true);

	//music_des/Fireflies.wma
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0);
}

void Audio::playMainBGM()
{
	
	if (USERDEFAULT->getBoolForKey("ISMUSICON") == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music_des/effectmainpage_1.WAV", false);
}

void Audio::onClickBtn()
{
	if (USERDEFAULT->getBoolForKey("ISMUSICON") == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music_des/push_1.wav", false);
}

void Audio::playReadyGo()
{
	if (USERDEFAULT->getBoolForKey("ISMUSICON") == true)
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music_des/readygo.wma", false);
}

void Audio::playPop()
{
	if (USERDEFAULT->getBoolForKey("ISMUSICON") == true)
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music_des/pop.wav", false);
}

void Audio::playCombo(int size)
{
	if (!USERDEFAULT->getBoolForKey("ISMUSICON"))
	{
		return;
	}

	if (size == 4)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music_des/combo_1.wma", false);

	}
	else if (size >= 5)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music_des/combo_3.wma", false);

	}

}

void Audio::playBomb()
{
	if (USERDEFAULT->getBoolForKey("ISMUSICON") == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music_des/Props_Bomb.wma", false);
}

void Audio::playPaint()
{
	if (USERDEFAULT->getBoolForKey("ISMUSICON") == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music_des/Props_Paint.wma", false);
}

void Audio::playRainbow()
{
	if (USERDEFAULT->getBoolForKey("ISMUSICON") == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music_des/Props_Rainbow.wma", false);
}

void Audio::playClear()
{
	if (USERDEFAULT->getBoolForKey("ISMUSICON") == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music_des/pop.wma", false);

}

void Audio::playNextLevel()		//游戏胜利 到下一关的时候
{
	//if (USERDEFAULT->getBoolForKey("ISMUSICON") == true)
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music_des/NextGameRound.wma", false);
}

void Audio::playfireWork()		//烟花音效 随机三种
{

	if (USERDEFAULT->getBoolForKey("ISMUSICON") == true)
	{
		srand((unsigned int)time(NULL));
		int index = abs(std::rand() % 3) + 1;

		if (index == 1)
		{
			//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music_des/fireworks_01.wma", false);

		}
		else if (index == 2)
		{
			//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music_des/fireworks_02.wma", false);

		}
		else
		{
			//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music_des/fireworks_03.wma", false);

		}
	}

}

void Audio::playLose()			//游戏失败
{
	if (USERDEFAULT->getBoolForKey("ISMUSICON") == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music_des/gameOver.mp3", false);
}

void Audio::prepare()
{
	//CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music_des/mainBGM_1.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music_des/Fireflies.wma");

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music_des/effectmainpage_1.WAV");

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music_des/combo_1.wma");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music_des/combo_3.wma");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music_des/gameOver.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music_des/pop.wma");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music_des/Props_Bomb.wma");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music_des/Props_Paint.wma");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music_des/Props_Rainbow.wma");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music_des/push_1.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music_des/readygo.wma");
}
