//
//  MainRoot.hpp
//  GoldMiner
//
//  Created by MacBook on 16/11/22.
//
//

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

