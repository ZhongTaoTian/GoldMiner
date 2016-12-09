//
//  UserDataManager.hpp
//  GoldMiner
//
//  Created by sfbest on 2016/11/28.
//
//

#ifndef UserDataManager_hpp
#define UserDataManager_hpp

#include <stdio.h>
#include "Const.hpp"
#include "SoundTool.hpp"

class UserDataManager {

public:
    static UserDataManager *getInstance();
    bool getMusicMute() {
        return SoundTool::getInstance()->getMusicIsMute();
    };
    
    bool getSoundMute() {
        return SoundTool::getInstance()->getEffectIsMute();
    };
    
    long getAllMoney() {
        return _allMoney;
    };
    
    int getStageNum() {
        return _stageNum;
    }
    
    void setMusicMute(bool mute);
    void setSoundMute(bool mute);
    void setAllMoney(long allMoney) {
        _allMoney = allMoney;
    };
    
    void setStageNum(int stageNum) {
        _stageNum = stageNum;
    };
    
    void saveUserData() {
        UserDefault::getInstance()->setIntegerForKey(userAllMoneyKey, _allMoney);
        UserDefault::getInstance()->setIntegerForKey(userStageNumKey, _stageNum);
    };
    
private:
    bool _musicMute;
    bool _soundMute;
    long _allMoney;
    int _stageNum;
};

#endif /* UserDataManager_hpp */
