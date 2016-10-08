#pragma  once

#include "cocos2d.h"
#include "Star.h"
#include "GameLayer.h"
#include <deque>

using namespace cocos2d;
using namespace std;

class GameLayer;

class StarMatrix : public Node
{
public:

	StarMatrix();
	~StarMatrix();
	static StarMatrix* create(GameLayer* layer);

	virtual bool init(GameLayer* layer);

	void updateStar(float delta);

	void onTouch(const Point& p);

	void setNeedClear(bool b);

	void initMatrix();	//初始化矩阵

	void resetLeftStarPos();	//重置剩下的星星

	void SaveStarData();		//保存数据
	
	void createStarFromData();	//从缓存创建星星

	void refreshScore();

	bool isEnded();

	int getLeftStarNum();

private:
   
	Point getPositionByIndex(int i,int j);

	Star* getStarByTouch(const Point& p);

	void genSelectedList(Star* s);

	void deleteSelectedList();

	void adjustMatrix();

	void clearMatrixOneByOne();

	void gotoNextLevel();

	void clean();

public:

	const static int ROW_NUM = 10;
	const static int COL_NUM = 10;

public:

	void changeStar(Star* s, int color);

	void boomSelectedList(Star* s);

	CC_SYNTHESIZE(bool, isBoom, Boom);	//set get

	CC_SYNTHESIZE(bool, isChange, ChangeColor);

	Star* changstar;

	Point point;

	CC_SYNTHESIZE(bool, isTouch, isTouch);

	EventListenerTouchOneByOne * myListener;

	//当购买通关时，回到最初的分数
	int oldScore;

	//检测是否过关
	void checkGuoGuan();
	bool isGuoGuan;

private:

	Star* stars[ROW_NUM][COL_NUM];

	deque<Star*> selectedList;	//选中的目标星星

	GameLayer* m_layer;

	bool needClear;

    static float ONE_CLEAR_TIME;

	float clearSumTime;

private:

	std::vector<int > m_starIndex_IList;
	std::vector<int > m_starIndex_JList;
	std::vector<int > m_starColorList;
};