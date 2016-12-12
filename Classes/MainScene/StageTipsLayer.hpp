//
//  StageTipsLayer.hpp
//  GoldMiner
//
//  项目GitHub地址:https://github.com/ZhongTaoTian
//  项目思路和架构讲解博客:http://www.jianshu.com/users/5fe7513c7a57/latest_articles
//  微博:http://weibo.com/5622363113/fans?topnav=1&wvr=6&mod=message&need_filter=1

#ifndef StageTipsLayer_hpp
#define StageTipsLayer_hpp

#include <stdio.h>
#include "Const.hpp"


class StageTipsLayer : public Layer {
    
public:
    typedef function<void ()> AnimationFinish;
    static StageTipsLayer * create(int stageNum);
    virtual bool init(int stageNum);
    static void showStageTipsLayer(Node *parent, int stageNum, AnimationFinish showFinish);
    AnimationFinish showFinish;
    
private:
    
    ImageView *image;
    Text *stageNumText;
    Text *passScoreText;
    
    void moveAnimation(Node *node, Vec2 position, AnimationFinish animFinish);
};

class StageFailOrSucessLayer : public Layer {

public:
    enum class TipsType {
        FAIL = 0,
        SUCESS
    };
    static StageFailOrSucessLayer * create(TipsType type, int goldCount, int curPayMoney);
    virtual bool init(TipsType type, int goldCount, int curPayMoney);
    static void showTips(Node *parent, TipsType type, int goldCount, int curPayMoney);
private:
    Text *goldCountText;
};

#endif /* StageTipsLayer_hpp */
