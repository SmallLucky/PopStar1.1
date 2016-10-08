#include "Star.h"

Star* Star::create(int color)
{
	Star* ret = new Star();
	if(ret && ret->initWithFile(ret->getImage(color)))
	{
		ret->color = color;
		ret->selected = false;
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

char* Star::getImage(int color)
{
	/*
	switch(color){
	case star_color::BLUE:
		return "star2.png";
	case star_color::GREEN:
		return "star5.png";
	case star_color::YELLOW:
		return "star4.png";
	case star_color::RED:
		return "star3.png";
	case star_color::PURPLE:
		return "star1.png";
	}
	*/
	switch (color){
	case star_color::PURPLE:
		return "star1.png";
	case star_color::BLUE:
		return "star2.png";
	case star_color::RED:
		return "star3.png";
	case star_color::YELLOW:
		return "star4.png";
	case star_color::GREEN:
		return "star5.png";
	}
}

bool Star::isSelected()
{
	return selected;
}

int Star::getColor()
{
	return color;
}

void Star::setDesPosition(const Point& p)
{
	desPosition = p;
}

void Star::updatePosition()
{
	if(desPosition.y != getPositionY())
	{
		setPositionY(getPositionY() - MOVE_SPEED);

		if(getPositionY() < desPosition.y)
		{
			setPositionY(desPosition.y);
		}
	}

	if(desPosition.x != getPositionX())
	{
		setPositionX(getPositionX() - MOVE_SPEED);
		if(getPositionX() < desPosition.x)
		{
			setPositionX(desPosition.x);
		}
	}
}

void Star::removeStar()
{
	if (getParent())
	{
		this->removeFromParent();
	}
}