#include "StarMatrix.h"
#include "GameData.h"
#include "Effect.h"
#include "Audio.h"
#include <iostream>
#include <ctime>
#include "DataSave.h"
#include "ChangeStarColor.h"
#include "Constant.h"

using namespace std;

float StarMatrix::ONE_CLEAR_TIME = 0.05f;

StarMatrix::StarMatrix() :
changstar(nullptr),
isBoom(false), //是否是爆炸状态
isChange(false),
isTouch(false),
myListener(nullptr),
oldScore(0),
isGuoGuan(false)
{
	//下一关
	auto _listener = EventListenerCustom::create(GOTONEXTLEVEL, [=](EventCustom*event){
		gotoNextLevel();
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_listener, 1);

	//保存数据
	auto _listener1 = EventListenerCustom::create(SAVEDATA, [=](EventCustom*event){
		//SaveStarData();
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_listener1, 1);

	//创建星星
	auto _listener2 = EventListenerCustom::create(CREATESTARFROMDATA, [=](EventCustom*event){
		createStarFromData();
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_listener2, 1);

	auto _listenerChange_1 = EventListenerCustom::create(CHANGE_1, [=](EventCustom*event){
		changeStar(changstar, 0);
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerChange_1, 1);

	auto _listenerChange_2 = EventListenerCustom::create(CHANGE_2, [=](EventCustom*event){
		changeStar(changstar, 1);
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerChange_2, 1);

	auto _listenerChange_3 = EventListenerCustom::create(CHANGE_3, [=](EventCustom*event){
		changeStar(changstar, 2);
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerChange_3, 1);

	auto _listenerChange_4 = EventListenerCustom::create(CHANGE_4, [=](EventCustom*event){
		changeStar(changstar, 3);
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerChange_4, 1);

	auto _listenerChange_5 = EventListenerCustom::create(CHANGE_5, [=](EventCustom*event){
		changeStar(changstar, 4);
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerChange_5, 1);

	auto _listenerClean = EventListenerCustom::create(CLEANSTAR, [=](EventCustom*event){
		clean();
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerClean, 1);
}

StarMatrix::~StarMatrix()
{
	_eventDispatcher->removeCustomEventListeners(GOTONEXTLEVEL);
	_eventDispatcher->removeCustomEventListeners(SAVEDATA);
	_eventDispatcher->removeCustomEventListeners(CREATESTARFROMDATA);

	_eventDispatcher->removeCustomEventListeners(CHANGE_1);
	_eventDispatcher->removeCustomEventListeners(CHANGE_2);
	_eventDispatcher->removeCustomEventListeners(CHANGE_3);
	_eventDispatcher->removeCustomEventListeners(CHANGE_4);
	_eventDispatcher->removeCustomEventListeners(CHANGE_5);
	_eventDispatcher->removeCustomEventListeners(CLEANSTAR);
}

StarMatrix* StarMatrix::create(GameLayer* layer)
{
	StarMatrix* ret = new StarMatrix();
	if(ret && ret->init(layer))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool StarMatrix::init(GameLayer* layer)
{
	if(!Node::init())
	{
		return false;
	}
	m_layer = layer;
	needClear = false;
	clearSumTime = 0;
	memset(stars, 0, sizeof(Star*) * ROW_NUM * COL_NUM);
	//initMatrix();

	myListener = EventListenerTouchOneByOne::create();
	myListener->setSwallowTouches(false);
	myListener->onTouchBegan = [](Touch *pTouch, Event *pEvent){
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, this);

	return true;
}

void StarMatrix::updateStar(float delta){
	
	for(int i = 0;i < ROW_NUM;i++)
	{
		for(int j = 0;j< COL_NUM;j++)
		{
			if(stars[i][j]!=nullptr)
			{
				stars[i][j]->updatePosition();	//更新位置
			}
		}
	}

	if(needClear)
	{
		clearSumTime += delta;
		if(clearSumTime > ONE_CLEAR_TIME)
		{
			clearMatrixOneByOne();		//一行行消除
			clearSumTime = 0;
			//吞噬button
			myListener->setSwallowTouches(true);
		}
	}
}

void StarMatrix::onTouch(const Point& p)
{	
	Star* s = getStarByTouch(p);
	point = p;
	if (s)
	{
		ChangeStarColor * change;
		if (getBoom() || getChangeColor())
		{
			if (getBoom())
			{
				int bombNum = GameData::getInstance()->getBombNum();
				if (bombNum>0)
				{
					GameData::getInstance()->setBombNum(bombNum-1);
					boomSelectedList(s);
					deleteSelectedList();
					Audio::getInstance()->playBomb();
					setBoom(false);
				}
				else
				{
					std::cout << "没有炸弹了" << std::endl;
				}

			}
			if (getChangeColor())
			{
				setChangeColor(false);
				//添加层
				change = ChangeStarColor::create();
				change->createChangeBox(p);
				addChild(change,CHANGESTAR_ZORDER);
				changstar = s;
			}
		}
		else
		{
			genSelectedList(s);
			deleteSelectedList();
		}
	}

}

void StarMatrix::changeStar(Star* s, int color)
{
	if (s)
	{
		int x = s->getIndexI();
		int y = s->getIndexJ();
		if (stars[x][y])
		{
			stars[x][y]->removeStar();
			stars[x][y] = nullptr;
		}
		stars[x][y] = Star::create(color);
		stars[x][y]->setPosition(getPositionByIndex(x, y));
		stars[x][y]->setDesPosition(getPositionByIndex(x, y));
		stars[x][y]->setIndex_ij(x, y);
		addChild(stars[x][y]);
		Audio::getInstance()->playPaint();
	}
}


void StarMatrix::setNeedClear(bool b){
	needClear = b;
}
//初始化10*10星星
void StarMatrix::initMatrix()
{
	isGuoGuan = false;
	srand(time(0));
	myListener->setSwallowTouches(false);

	for (int i = 0; i<ROW_NUM; i++)
	{
		for (int j = 0; j<COL_NUM; j++)
		{
			int color = abs(rand() % Star::COLOR_MAX_NUM); //0->4

			//int num = abs(rand() % Star::COLOR_MAX_NUM); //0->4

			//log("**************%d,%d", color, num);
			if (GameData::getInstance()->getCurLevel() <= 1)
			{
				Star* star = Star::create(color);
				stars[i][j] = star;
				star->setPosition(getPositionByIndex(i, j));
				star->setDesPosition(getPositionByIndex(i, j));
				star->setIndex_ij(i, j);
				this->addChild(star);
				j++;
				if (color == 3)
				{
					Star* star1 = Star::create(color + 1); //4
					stars[i][j] = star1;
					star1->setPosition(getPositionByIndex(i, j));
					star1->setDesPosition(getPositionByIndex(i, j));
					star1->setIndex_ij(i, j);
					this->addChild(star1);
				}
				if (color == 2)
				{
					Star* star1 = Star::create(color); //4
					stars[i][j] = star1;
					star1->setPosition(getPositionByIndex(i, j));
					star1->setDesPosition(getPositionByIndex(i, j));
					star1->setIndex_ij(i, j);
					this->addChild(star1);

				}if (color == 1)
				{
					Star* star1 = Star::create(color + 3); //2
					stars[i][j] = star1;
					star1->setPosition(getPositionByIndex(i, j));
					star1->setDesPosition(getPositionByIndex(i, j));
					star1->setIndex_ij(i, j);
					this->addChild(star1);

				}if (color == 0)
				{
					Star* star1 = Star::create(color + 4); //2
					stars[i][j] = star1;
					star1->setPosition(getPositionByIndex(i, j));
					star1->setDesPosition(getPositionByIndex(i, j));
					star1->setIndex_ij(i, j);
					this->addChild(star1);
				}
				if (color == 4)
				{

					Star* star1 = Star::create(color); // 4
					stars[i][j] = star1;
					star1->setPosition(getPositionByIndex(i, j));
					star1->setDesPosition(getPositionByIndex(i, j));
					star1->setIndex_ij(i, j);
					this->addChild(star1);
				}
			}
			else
			{
				Star* star = Star::create(color);
				stars[i][j] = star;
				star->setPosition(getPositionByIndex(i, j));
				star->setDesPosition(getPositionByIndex(i, j));
				star->setIndex_ij(i, j);
				this->addChild(star);
			}
		}
	}
}


Point StarMatrix::getPositionByIndex(int i,int j){
	float x = j * Star::STAR_WIDTH + Star::STAR_WIDTH/2;
	float y = (StarMatrix::COL_NUM - i)*Star::STAR_HEIGHT - Star::STAR_HEIGHT/2;
	return Point(x,y);
}

Star* StarMatrix::getStarByTouch(const Point& p)
{
	int k = p.y/Star::STAR_HEIGHT;//这里要用K转一下int 不然得不到正确结果
	int i = ROW_NUM - 1 - k;
	int j = p.x/Star::STAR_WIDTH;
	if(i >= 0 && i < ROW_NUM && 
	   j >= 0 && j < COL_NUM &&
	   stars[i][j] != nullptr)
	{
		//CCLOG("i=%d,j=%d",i,j);
		return stars[i][j];
	}else
	{
		return nullptr;
	}
}

void StarMatrix::genSelectedList(Star* s)
{
	selectedList.clear();
	deque<Star*> travelList;
	travelList.push_back(s);
	deque<Star*>::iterator it;

	for(it= travelList.begin();it != travelList.end();)
	{
		Star* star = *it;
		Star* linkStar = nullptr;

		int index_i = star->getIndexI();
		int index_j = star->getIndexJ();
		//上
		if(index_i-1 >= 0 && (linkStar = stars[index_i-1][index_j]) )
		{
			if(!linkStar->isSelected() && linkStar->getColor() == star->getColor())
				travelList.push_back(stars[index_i-1][index_j]);
		}
		//下
		if(index_i+1 < ROW_NUM  && (linkStar = stars[index_i+1][index_j]) )
		{
			if(!linkStar->isSelected() && linkStar->getColor() == star->getColor())
				travelList.push_back(stars[index_i+1][index_j]);
		}
		//左
		if(index_j-1 >= 0 && (linkStar = stars[index_i][index_j-1]) )
		{
			if(!linkStar->isSelected() && linkStar->getColor() == star->getColor())
				travelList.push_back(stars[index_i][index_j-1]);
		}
		//右
		if(index_j+1 < COL_NUM && (linkStar = stars[index_i][index_j+1]) )
		{
			if(!linkStar->isSelected() && linkStar->getColor() == star->getColor())
				travelList.push_back(stars[index_i][index_j+1]);
		}

		if(!star->isSelected())
		{
			star->setSelected(true);
			selectedList.push_back(star);
		}

		travelList.pop_front();

		it = travelList.begin();
	}
}

void StarMatrix::boomSelectedList(Star* s)
{
	selectedList.clear();
	Star * star = s;
	int  indext_i = star->getIndexI();
	int  indext_j = star->getIndexJ();
	log("boom:%i,%i", indext_i, indext_j);

	if (star != nullptr)
	{
		selectedList.push_back(stars[indext_i][indext_j]); //ziji

		if (indext_j - 1 >= 0 && stars[indext_i][indext_j - 1] != nullptr) //同行左边这个
		{
			selectedList.push_back(stars[indext_i][indext_j - 1]);
		}
		if (indext_j + 1 <= 9 && stars[indext_i][indext_j + 1] != nullptr)//同行 右边
		{
			selectedList.push_back(stars[indext_i][indext_j + 1]);
		}
	}
	if (indext_i + 1 <= 9)
	{
		if (stars[indext_i + 1][indext_j] != nullptr) //同列 下边
		{
			selectedList.push_back(stars[indext_i + 1][indext_j]);
		}
		if (indext_j - 1 >= 0 && stars[indext_i + 1][indext_j - 1] != nullptr)//右移一列下一行
		{
			selectedList.push_back(stars[indext_i + 1][indext_j - 1]);
		}
		if (indext_j + 1 <= 9 && stars[indext_i + 1][indext_j + 1] != nullptr)
		{
			selectedList.push_back(stars[indext_i + 1][indext_j + 1]);
		}
	}
	if (indext_i - 1 >= 0)
	{
		if (stars[indext_i - 1][indext_j] != nullptr)
		{
			selectedList.push_back(stars[indext_i - 1][indext_j]);
		}
		if (indext_j - 1 >= 0 && stars[indext_i - 1][indext_j - 1] != nullptr)
		{
			selectedList.push_back(stars[indext_i - 1][indext_j - 1]);
		}
		if (indext_j + 1 <= 9 && stars[indext_i - 1][indext_j + 1] != nullptr)
		{
			selectedList.push_back(stars[indext_i - 1][indext_j + 1]);
		}
	}
	//发送消息到UI层
	EventCustom event_1(REFRISHUILAYER);
	_eventDispatcher->dispatchEvent(&event_1);
}

void StarMatrix::deleteSelectedList()
{
	if(selectedList.size() <= 1)
	{
		m_layer->hideLinkNum();
		selectedList.at(0)->setSelected(false);
		return;
	}
	//cout << selectedList.size()<< "连消" << endl;
	for(auto it = selectedList.begin();it != selectedList.end();it++)
	{
		Star* star = *it;

		//粒子效果
		//showStarParticleEffect(star->getColor(),star->getPosition(),this);
		showStarParEffect(star->getColor(), star->getPosition(), this);

		stars[star->getIndexI()][star->getIndexJ()] = nullptr;
		star->removeFromParentAndCleanup(true);
		//播放音效
		Audio::getInstance()->playPop();
		//添加一个分数label移动消失的效果
	}
	//COMBO效果
	showComboEffect(selectedList.size(),this);
	Audio::getInstance()->playCombo(selectedList.size());

	m_layer->showLinkNum(selectedList.size());

	m_layer->showScore(selectedList.size(),point);

	refreshScore();

	adjustMatrix();

	//检测是否过关
	int curlev = GameData::getInstance()->getCurLevel();
	if (GameData::getInstance()->getCurSocre() >= GameData::getInstance()->getTargetScore(curlev))
	{
		if (!isGuoGuan)
		{
			isGuoGuan = true;
			checkGuoGuan();
		}
	}

	if(isEnded())//判断是否结束
	{
		m_layer->floatLeftStarMsg(getLeftStarNum());
	}
}

//检测是否过关
void StarMatrix::checkGuoGuan()
{
	int curlev = GameData::getInstance()->getCurLevel();

	if (GameData::getInstance()->getCurSocre() >= GameData::getInstance()->getTargetScore(curlev))
	{
		Sprite* guoGuan = Sprite::create("stage_clear.png");
		guoGuan->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2);
		addChild(guoGuan);

		MoveTo* move = MoveTo::create(0.5, Vec2(400, 550));
		DelayTime* delayTime = DelayTime::create(1);
		auto action = Sequence::create(move, CallFunc::create([=]
		{
			guoGuan->setScale(0.4);
		}), delayTime, CallFunc::create([=]
		{
			if (guoGuan)
			{
				guoGuan->removeFromParent();
			}
		}), nullptr);

		guoGuan->runAction(action);

	}
}

void StarMatrix::adjustMatrix()
{
	//垂直方向调整
	for(int i = ROW_NUM-1;i>=0;i--)
	{
		for(int j = COL_NUM-1;j>=0;j--)
		{
			if(stars[i][j] == nullptr)
			{
				int up = i;
				int dis = 0;
				while(stars[up][j] == nullptr)
				{
					dis++;
					up--;
					if(up<0)
					{
						break;
					}
				}
				
				for(int begin_i = i - dis;begin_i >= 0;begin_i--)
				{
					if(stars[begin_i][j] == nullptr)
						continue;
					Star* s = stars[begin_i + dis][j] = stars[begin_i][j];
					s->setIndex_ij(begin_i + dis,j);
					s->setDesPosition(getPositionByIndex(begin_i + dis,j));
					stars[begin_i][j] = nullptr;
				}
			}else
			{
				continue;
			}
		}
	}
	//水平方向调整
	for(int j = 0;j < COL_NUM;j++)
	{
		if(stars[ROW_NUM-1][j] == nullptr)
		{
			int des = 0;
			int right = j;
			while(stars[ROW_NUM-1][right] == nullptr)
			{
				des++;
				right++;
			}

			for(int begin_i = 0;begin_i<ROW_NUM;begin_i++)
			{
				for(int begin_j = j + des;begin_j < COL_NUM;begin_j++)
				{
					if(stars[begin_i][begin_j] == nullptr)
						continue;
					Star* s = stars[begin_i][begin_j - des] = stars[begin_i][begin_j];
					s->setIndex_ij(begin_i,begin_j - des);
					s->setDesPosition(getPositionByIndex(begin_i,begin_j - des));
					stars[begin_i][begin_j] = nullptr;
				}
			}
		}
	}
}
//刷新分数
void StarMatrix::refreshScore()
{
	GameData* data = GameData::getInstance();

	data->setCurScore(data->getCurSocre() + selectedList.size()*selectedList.size()*5);	//分数计算
	if (data->getCurSocre() > data->getHistoryScore())
	{
		data->setHistoryScore(data->getCurSocre());
	}
	m_layer->refrishUI();
}

bool StarMatrix::isEnded()
{
	bool bRet = true;
	for(int i=0;i<ROW_NUM;i++)
	{
		for(int j=0;j<COL_NUM;j++)
		{
			if(stars[i][j] == nullptr)
				continue;

			int curColor = stars[i][j]->getColor();
			//上
			if(i-1>=0 && stars[i-1][j]!=nullptr && stars[i-1][j]->getColor() ==  curColor)
				return false;
			//下
			if(i+1<ROW_NUM && stars[i+1][j]!=nullptr && stars[i+1][j]->getColor() == curColor)
				return false;
			//左
			if(j-1>=0 && stars[i][j-1]!=nullptr && stars[i][j-1]->getColor() == curColor)
				return false;
			//右
			if(j+1<COL_NUM && stars[i][j+1]!=nullptr && stars[i][j+1]->getColor() == curColor)
				return false;
		}
	}

	return bRet;
}

void StarMatrix::clearMatrixOneByOne()
{
	for(int i=0;i<ROW_NUM;i++)
	{
		for(int j=0;j<COL_NUM;j++)
		{
			if (stars[i][j] == nullptr)
			{
				continue;
			}
			else
			{
				showStarParticleEffect(stars[i][j]->getColor(), stars[i][j]->getPosition(), this);
				Audio::getInstance()->playPop();
				stars[i][j]->removeFromParentAndCleanup(true);
				stars[i][j] = nullptr;
			}
			return;
		}
	}
	//能够执行到这一句说明Matrix全为空，不在需要清空
	needClear = false;
	//转到下一关或者弹出结束游戏的窗口
	int curlev = GameData::getInstance()->getCurLevel();

	if (GameData::getInstance()->getCurSocre() >= GameData::getInstance()->getTargetScore(curlev))
	{
		GameData::getInstance()->setCurLevel(curlev + 1);
		oldScore = GameData::getInstance()->getCurSocre();

		m_layer->gotoRewardLayer();

	}else
	{
		m_layer->gotoGameOver();				//游戏结束界面
	}
}

int StarMatrix::getLeftStarNum()
{
	int ret = 0;

	for(int i=0;i<ROW_NUM;i++)
	{
		for(int j=0;j<COL_NUM;j++)
		{
			if(stars[i][j] == nullptr)
				continue;
			ret++;
		}
	}
	return ret;
}
//重置剩下的星星
void StarMatrix::resetLeftStarPos()
{
	Audio::getInstance()->playRainbow();
	srand(time(0));

	for (int i = 0; i<ROW_NUM; i++)
	{
		for (int j = 0; j<COL_NUM; j++)
		{
			if (stars[i][j] != nullptr)
			{
				stars[i][j]->removeStar();		//clear old 
				int color = abs(rand() % Star::COLOR_MAX_NUM);
				Star* star = Star::create(color);
				stars[i][j] = star;
				star->setPosition(getPositionByIndex(i, j));
				star->setDesPosition(getPositionByIndex(i, j));

				star->setIndex_ij(i, j);

				this->addChild(star);
			}
		}
	}

	// 继续游戏的时候要判断是否没有的消除了
	if (isEnded())
	{
		m_layer->floatLeftStarMsg(getLeftStarNum());
	}
}

void StarMatrix::gotoNextLevel()
{
	GameData::getInstance()->setCurScore(oldScore);
	m_layer->gotoNextLevel();				//进行下一关
}

void StarMatrix::clean()
{
	cout << "clean...." << endl;

	for (int i = 0; i<ROW_NUM; i++)
	{
		for (int j = 0; j<COL_NUM; j++)
		{
			if (stars[i][j] == nullptr)
			{
				continue;
			}
			else
			{
				stars[i][j]->removeFromParentAndCleanup(true);
				stars[i][j] = nullptr;
			}
			//return;
		}
	}
}

void StarMatrix::SaveStarData()
{
	m_starColorList.clear();
	m_starIndex_IList.clear();
	m_starIndex_JList.clear();

	for (int i = 0; i<ROW_NUM; i++)
	{
		for (int j = 0; j<COL_NUM; j++)
		{
			if (stars[i][j] == nullptr)
				continue;
			m_starColorList.push_back(stars[i][j]->getColor());
			m_starIndex_IList.push_back(stars[i][j]->getIndexI());
			m_starIndex_JList.push_back(stars[i][j]->getIndexJ());
		}
	}

	for (int i = 0; i < m_starColorList.size(); i++)
	{
		DataSava::getInstance()->setStarColor(m_starColorList[i]);
		DataSava::getInstance()->setStarIndexI(m_starIndex_IList[i]);
		DataSava::getInstance()->setStarIndexJ(m_starIndex_JList[i]);
	}
	std::cout << "保存数据" << endl;
}

void StarMatrix::createStarFromData()
{
	DataSava::getInstance()->praseJsonData();
	auto data = DataSava::getInstance();

	int starNum = data->getStarColorList().size();

	log("starNUm=%d", starNum);
	log("I===%d", data->getStarIndexIList().size());
	log("j===%d", data->getStarIndexJList().size());

	for (int i = 0; i < starNum; i++)
	{
		int x = data->getStarIndexIList().at(i);
		int y = data->getStarIndexJList().at(i);
		int color = data->getStarColorList().at(i);

		stars[x][y] = Star::create(color);
		stars[x][y]->setPosition(getPositionByIndex(x, y));
		stars[x][y]->setDesPosition(getPositionByIndex(x, y));

		stars[x][y]->setIndex_ij(x, y);

		this->addChild(stars[x][y]);
	}
	std::cout << "创建星星------------------------" << endl;
}