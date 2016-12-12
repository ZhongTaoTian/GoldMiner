//
//  UserDataManager.hpp
//  GoldMiner
//
//  项目GitHub地址:https://github.com/ZhongTaoTian
//  项目思路和架构讲解博客:http://www.jianshu.com/users/5fe7513c7a57/latest_articles
//  微博:http://weibo.com/5622363113/fans?topnav=1&wvr=6&mod=message&need_filter=1

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
