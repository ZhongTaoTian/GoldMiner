//
//  Pause.hpp
//  GoldMiner
//
//  项目GitHub地址:https://github.com/ZhongTaoTian
//  项目思路和架构讲解博客:http://www.jianshu.com/users/5fe7513c7a57/latest_articles
//  微博:http://weibo.com/5622363113/fans?topnav=1&wvr=6&mod=message&need_filter=1

#ifndef Pause_hpp
#define Pause_hpp

#include <stdio.h>
#include "Const.hpp"

class Pause : public Layer {
    
public:
    virtual bool init(bool nextBtnCanClick);
    static Pause *create(bool nextBtnCanClick);
    static void showPause(Node *parent, bool nextBtnCanClick);
    ~Pause(){
        CCLOG("pause销毁");
    };
public:
    Node *csb;
private:
    void musicButtonOnTouch(Ref *sender, Widget::TouchEventType type);
    void soundButtonOnTouch(Ref *sender, Widget::TouchEventType type);
    
    void updateMusicButton();
    void updateSoundButton();
};

#endif /* Pause_hpp */
