#include "Effect.h"
#include "Star.h"
#include "Audio.h"

void showStarParticleEffect(int color, Point position, Node* node)
{
	ParticleExplosion* effect = ParticleExplosion::create();

	effect->setTexture(Director::getInstance()->getTextureCache()->addImage("star.png"));
	//effect->setTotalParticles(250);
	effect->setTotalParticles(500);
	effect->setStartColor(getColor4F(color));
	effect->setStartColorVar(Color4F(0, 0, 0, 1));
	effect->setEndColor(getColor4F(color));
	effect->setEndColorVar(Color4F(0, 0, 0, 1));
	effect->setStartSize(20.0f);
	effect->setGravity(Point(0, -300));
	effect->setLife(2.0f);
	effect->setSpeed(350);
	effect->setSpeedVar(10);
	effect->setPosition(position);
	effect->setAutoRemoveOnFinish(true);
	node->addChild(effect);
}

void showStarParEffect(int color, Point position, Node* node)
{
	//ExplodingRing.plist
	//Flower.plist
	auto effect = ParticleSystemQuad::create("Flower.plist");
	//auto effect = ParticleSystemQuad::create("ExplodingRing.plist");
	if (effect)
	{
		effect->setPosition(position);
		effect->setStartColor(getColor4F(color));
		effect->setDuration(0.01f);
		effect->setSpeed(150);
		effect->setSpeedVar(50);
		effect->setLife(0.01f);
		effect->setTotalParticles(150);
		effect->setScale(0.8f);
		effect->setAutoRemoveOnFinish(true);
		node->addChild(effect);
	}

}

Color4F getColor4F(int color)
{
	switch (color){
	case Star::star_color::PURPLE:
		return Color4F(0.74, 0.30, 0.99, 1);
		//return Color4F(189,78,253,1);
	case Star::star_color::BLUE:
		return Color4F(84 / 255.0f, 203 / 255.0f, 254 / 255.0f, 1);
	case Star::star_color::RED:
		return Color4F(253 / 255.0f, 80 / 255.0f, 126 / 255.0f, 1);
	case Star::star_color::YELLOW:
		return Color4F(253 / 255.0f, 234 / 255.0f, 84 / 255.0f, 1);
	case Star::star_color::GREEN:
		return Color4F(132 / 255.0f, 226 / 255.0f, 111 / 255.0f, 1);
	}
	return Color4F(1, 1, 1, 0);
}

Color3B getColor3B(int color)
{
	switch (color){
	case Star::star_color::PURPLE:
		return Color3B(Color4F(0.74, 0.30, 0.99, 1));
		//return Color4F(189,78,253,1);
	case Star::star_color::BLUE:
		return Color3B(Color4F(84 / 255.0f, 203 / 255.0f, 254 / 255.0f, 1));
	case Star::star_color::RED:
		return Color3B(Color4F(253 / 255.0f, 80 / 255.0f, 126 / 255.0f, 1));
	case Star::star_color::YELLOW:
		return Color3B(Color4F(253 / 255.0f, 234 / 255.0f, 84 / 255.0f, 1));
	case Star::star_color::GREEN:
		return Color3B(Color4F(132 / 255.0f, 226 / 255.0f, 111 / 255.0f, 1));
	}
	Color3B(Color4F(1, 1, 1, 0));
}

void showComboEffect(int size, Node* node)
{
	if (size<4)
		return;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Sprite* comboSprite;

	if (size >= 7)
	{
		comboSprite = Sprite::create("combo_4.png");
	}
	else if (size >= 6)
	{
		comboSprite = Sprite::create("combo_3.png");
	}
	else if (size >= 5)
	{
		comboSprite = Sprite::create("combo_2.png");
	}
	else
	{
		comboSprite = Sprite::create("combo_1.png");
	}

	if (!comboSprite)
	{
		return;
	}
	comboSprite->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 + 50));
	node->addChild(comboSprite);

	Blink* blink = Blink::create(1.0f, 10);
	CallFunc* remove = CallFunc::create([=](){
		if (comboSprite->getParent())
		{
			comboSprite->removeFromParentAndCleanup(true);
		}
	});
	Sequence* action = Sequence::create(blink, remove, nullptr);
	comboSprite->runAction(action);
}

void explodeSpecialH(int color, Point position, Node* node)
{
	Size size = Director::getInstance()->getWinSize();

	float scaleX = 4;
	float scaleY = 0.7;
	float time = 0.3;
	Point startPosition = position;
	float speed = 0.6f;

	//auto colorSpriteRight = Sprite::create("colorHRight.png");
	auto colorSpriteRight = Sprite::create("star.png");

	colorSpriteRight->setColor(getColor3B(color));

	if (!colorSpriteRight)
	{
		return;
	}

	node->addChild(colorSpriteRight, 10);

	Point endPosition1 = Point(position.x + size.width, position.y);

	colorSpriteRight->setPosition(startPosition);

	colorSpriteRight->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),
		MoveTo::create(speed, endPosition1),
		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteRight)),
		NULL));

	//auto colorSpriteLeft = Sprite::create("colorHLeft.png");
	auto colorSpriteLeft = Sprite::create("star.png");

	colorSpriteLeft->setColor(getColor3B(color));

	if (!colorSpriteLeft)
	{
		return;
	}
	node->addChild(colorSpriteLeft, 10);

	Point endPosition2 = Point(position.x - size.width, position.y);

	colorSpriteLeft->setPosition(startPosition);

	colorSpriteLeft->runAction(Sequence::create(ScaleTo::create(time, scaleX, scaleY),

		MoveTo::create(speed, endPosition2),

		CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, colorSpriteLeft)),
		NULL));
}

void explodeSpecialV(int color, Point position, Node* node)
{

}