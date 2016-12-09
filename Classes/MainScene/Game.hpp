//
//  Game.hpp
//  GoldMiner
//
//  Created by MacBook on 16/11/27.
//
//

#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include "Const.hpp"
#include "Gold.hpp"

class Game : public Layer {
    
public:
    static Scene *createScene(bool isBuyBomb, bool isBuyPotion, bool isBuyDiamonds, bool isStoneBook, int payMoney);
    virtual bool init(bool isBuyBomb, bool isBuyPotion, bool isBuyDiamonds, bool isStoneBook, int payMoney);
    static Game *create(bool isBuyBomb, bool isBuyPotion, bool isBuyDiamonds, bool isStoneBook, int payMoney);

private:
    Text *allMoney;
    Text *targetMoney;
    Text *stageNum;
    Text *time;
    long passScroe;
    bool showStageTips;
    ImageView *rope;
    Sprite *middleCircle;
    Sprite *leftHook;
    Sprite *rightHook;
    
    cocostudio::timeline::ActionTimeline *minerTimeLine;
    
    int timeCount;
    int backSpeed = 10;
    int curStageScore;
    bool ropeChangeing;
    int ropeHeight = 20;
    int curPayMoney;
    
    Point circlePosition;
    bool isOpenHook;
    
    Gold *goldSprite;
    
    bool isBuyPotion;
    bool isBuyDiamonds;
    bool isBuyStoneBook;
    Button *bompButton;
    
private:
    void addButtonAction(Node *csbNode);
    void onEnter();
    void setUpText(Widget *csb);
    
    void startGame();
    void stopGame();
    
    void updateTime(float dt);
    
    void startShakeHookAnimation();
    void stopShakeHookAnimation();
    
    void addRopeHeight(float dt);
    void subRopeHeight(float dt);
    
    void loadStageInfo();

    bool touchCallBack(Touch *touch, Event *event);
    bool physicsBegin(PhysicsContact &contact);
    void pullGold(PhysicsContact &contact);
};

#endif /* Game_hpp */
