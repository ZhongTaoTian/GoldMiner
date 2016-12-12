//
//  UserDataManager.cpp
//  GoldMiner
//
//  项目GitHub地址:https://github.com/ZhongTaoTian
//  项目思路和架构讲解博客:http://www.jianshu.com/users/5fe7513c7a57/latest_articles
//  微博:http://weibo.com/5622363113/fans?topnav=1&wvr=6&mod=message&need_filter=1

#include "UserDataManager.hpp"

static UserDataManager *s_SharedUserDataManager = nullptr;

UserDataManager *UserDataManager::getInstance()
{
    if (s_SharedUserDataManager == nullptr) {
        s_SharedUserDataManager = new UserDataManager();
        
        s_SharedUserDataManager->_musicMute = SoundTool::getInstance()->getMusicIsMute();
        s_SharedUserDataManager->_soundMute = SoundTool::getInstance()->getEffectIsMute();
        s_SharedUserDataManager->_allMoney = UserDefault::getInstance()->getIntegerForKey(userAllMoneyKey, 0);
        s_SharedUserDataManager->_stageNum = UserDefault::getInstance()->getIntegerForKey(userStageNumKey, 1);
    }
    
    return s_SharedUserDataManager;
}

void UserDataManager::setMusicMute(bool mute)
{
    _musicMute = mute;
    
    SoundTool::getInstance()->setMusicMute(mute);
}

void UserDataManager::setSoundMute(bool mute)
{
    _soundMute = mute;
    
    SoundTool::getInstance()->setEffectMute(mute);
}
