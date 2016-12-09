//
//  UserDataManager.cpp
//  GoldMiner
//
//  Created by tianzhongtao on 2016/11/28.
//

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
