#pragma once
#include "cocos2d.h"
using namespace cocos2d;

void showComboEffect(int size, Node* node);

void showStarParticleEffect(int color, Point position, Node* node);
void showStarParEffect(int color, Point position, Node* node);

Color4F getColor4F(int color);
Color3B getColor3B(int color);

void explodeSpecialH(int color, Point position, Node* node);
void explodeSpecialV(int color, Point position, Node* node);
