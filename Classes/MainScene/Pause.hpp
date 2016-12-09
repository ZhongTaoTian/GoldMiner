//
//  Pause.hpp
//  GoldMiner
//
//  Created by sfbest on 2016/11/28.
//
//

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
