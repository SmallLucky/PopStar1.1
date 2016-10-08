#pragma  once

#include "cocos2d.h"
using namespace cocos2d;

class Star : public Sprite
{
public:
	enum star_color
	{
		PURPLE,
		BLUE,
		RED,
		YELLOW,
		GREEN
	};

	static Star* create(int color);

	bool isSelected();

	inline void setSelected(bool b){selected = b;}

	int getColor();

	void setDesPosition(const Point& p);

	inline Point getDesPosition(){return desPosition;}

	void updatePosition();

	inline int getIndexI(){return index_i;}

	inline int getIndexJ(){return index_j;}

	inline void setIndex_ij(int i,int j){index_i = i;index_j = j;}

	void removeStar();	//删除一个星星

private:
	char* getImage(int color);

public:
	const static int MOVE_SPEED = 8;	//移动速度
	static const int COLOR_MAX_NUM = 5;	//颜色数量
	static const int STAR_WIDTH = 48;	//星星宽度
	static const int STAR_HEIGHT = 48;	//星星高度

private:

	int color;
	Point desPosition;
	bool selected;
	int index_i;
	int index_j;

};