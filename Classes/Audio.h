#pragma once

class Audio
{
public:
	static Audio* getInstance();
	void prepare();		//遇加载

	//背景音乐
	void playBGM();			//整个游戏游戏	
	void playMainBGM();		//主页面才有		

	void onClickBtn();		//点击按钮	
	void playReadyGo();		//准备游戏	
	void playPop();			//消灭星星时候	

	void playCombo(int size);	//特效音乐	
	void playClear();			//游戏结束消除其余 

	//三个道具
	void playBomb();		//
	void playPaint();		//
	void playRainbow();		//

	void playNextLevel();		//游戏胜利 到下一关的时候  
	void playfireWork();		//烟花音效 随机三种  
	void playLose();			//游戏失败	

private:

	static Audio* m_instance;
};
