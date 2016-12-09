//
//  Const.hpp
//  GoldMiner
//
//  Created by MacBook on 16/11/25.
//
//

#ifndef Const_hpp
#define Const_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

USING_NS_CC;
using namespace std;

#define kWinSizeWidth Director::getInstance()->getWinSize().width
#define kWinSizeHeight Director::getInstance()->getWinSize().height
#define kWinSize Director::getInstance()->getWinSize()

#define kPlayClickSound SoundTool::getInstance()->playEffect("res/music/Select.mp3");

static const char musicMuteKey[30] = "musicMuteKey";
static const char soundMuteKey[30] = "soundMuteKey";
static const char userStageNumKey[30] = "userStageNumKey";
static const char userAllMoneyKey[30] = "userAllMoneyKey";

#endif /* Const_hpp */
