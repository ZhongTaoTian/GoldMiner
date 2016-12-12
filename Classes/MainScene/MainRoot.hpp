//
//  MainRoot.hpp
//  GoldMiner
//
//  项目GitHub地址:https://github.com/ZhongTaoTian
//  项目思路和架构讲解博客:http://www.jianshu.com/users/5fe7513c7a57/latest_articles
//  微博:http://weibo.com/5622363113/fans?topnav=1&wvr=6&mod=message&need_filter=1

#ifndef MainRoot_hpp
#define MainRoot_hpp

#include <stdio.h>
#include "Const.hpp"

class MainLayer : public Layer {
    
public:
    static Scene *createScene();
    virtual bool init();
    CREATE_FUNC(MainLayer);

private:
    ImageView *cloud1;
    ImageView *cloud2;
    Sprite *leftLeg;
    Sprite *face;
    Sprite *light;
    
private:
    cocostudio::timeline::ActionTimeline *animation;
    virtual void onEnter();
    void moveCloud(ImageView *cloud, float time1, float time2);
    void playMinerAnimation(Sprite *sprite, string imageName, float frameDelat);
    
#pragma mark - Action
    void startButtonTouch(Ref *sender, Widget::TouchEventType type);
};

#endif /* MainRoot_hpp */

