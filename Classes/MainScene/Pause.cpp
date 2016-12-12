//
//  Pause.cpp
//  GoldMiner
//
//  项目GitHub地址:https://github.com/ZhongTaoTian
//  项目思路和架构讲解博客:http://www.jianshu.com/users/5fe7513c7a57/latest_articles
//  微博:http://weibo.com/5622363113/fans?topnav=1&wvr=6&mod=message&need_filter=1

#include "Pause.hpp"
#include "UserDataManager.hpp"
#include "MainRoot.hpp"

bool Pause::init(bool nextBtnCanClick)
{
    if (!Layer::init()) {
        return false;
    }
    
    csb = CSLoader::createNode("PauseLayer.csb");
    addChild(csb);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/Resources/general-sheet.plist");
    
    Button *musicButton = static_cast<Button *>(Helper::seekWidgetByName(static_cast<Widget *>(csb), "musicButton"));
    musicButton->addTouchEventListener(CC_CALLBACK_2(Pause::musicButtonOnTouch, this));
    this->updateMusicButton();
    
    Button *soundButton = static_cast<Button *>(Helper::seekWidgetByName(static_cast<Widget *>(csb), "soundButton"));
    soundButton->addTouchEventListener(CC_CALLBACK_2(Pause::soundButtonOnTouch, this));
    this->updateSoundButton();
        
    Button *nextStage = static_cast<Button *>(Helper::seekWidgetByName(static_cast<Widget *>(csb), "nextStage"));
    nextStage->setEnabled(nextBtnCanClick);
    nextStage->addTouchEventListener([=](Ref *sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            _eventDispatcher->dispatchCustomEvent("nextStage");
        }
    });
    
    Button *backStage = static_cast<Button *>(Helper::seekWidgetByName(static_cast<Widget *>(csb), "backStage"));
    backStage->addTouchEventListener([=](Ref *sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            Director::getInstance()->replaceScene(MainLayer::createScene());
        }
    });
    
    Button *cancelButton = static_cast<Button *>(Helper::seekWidgetByName(static_cast<Widget *>(csb), "cancelButton"));
    cancelButton->addTouchEventListener([=](Ref *sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            
            Director::getInstance()->getRunningScene()->onEnter();
            
            this->removeFromParentAndCleanup(true);
        }
    });
        
    return true;
}

void Pause::updateMusicButton()
{
    string normal;
    string press;
    // add Action
    Button *musicButton = static_cast<Button *>(Helper::seekWidgetByName(static_cast<Widget *>(csb), "musicButton"));
    if (UserDataManager::getInstance()->getMusicMute()) {
        normal = "music-on-btn-0.png";
        press = "music-on-btn-1.png";
    } else {
        normal = "music-off-btn-0.png";
        press = "music-off-btn-1.png";
    }
    musicButton->loadTextures(normal, press, "", TextureResType::PLIST);
}

class Pause *Pause::create(bool nextBtnCanClick)
{
    Pause *pause = new Pause();
    
    if (pause && pause->init(nextBtnCanClick)) {
        pause->autorelease();
        return pause;
    } else {
        delete pause;
        pause = nullptr;
    }
    
    return nullptr;
}

void Pause::updateSoundButton()
{
    string normal;
    string press;
    // add Action
    Button *soundButton = static_cast<Button *>(Helper::seekWidgetByName(static_cast<Widget *>(csb), "soundButton"));
    if (UserDataManager::getInstance()->getSoundMute()) {
        normal = "sound-on-btn-0.png";
        press = "sound-on-btn-1.png";
    } else {
        normal = "sound-off-btn-0.png";
        press = "sound-off-btn-1.png";
    }
    soundButton->loadTextures(normal, press, "", TextureResType::PLIST);
}

void Pause::showPause(cocos2d::Node *parent, bool nextBtnCanClick)
{
    auto pause = Pause::create(nextBtnCanClick);
    parent->addChild(pause);
}

void Pause::musicButtonOnTouch(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED) {
        UserDataManager::getInstance()->setMusicMute(!UserDataManager::getInstance()->getMusicMute());
        this->updateMusicButton();
    }
}

void Pause::soundButtonOnTouch(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED) {
        UserDataManager::getInstance()->setSoundMute(!UserDataManager::getInstance()->getSoundMute());
        this->updateSoundButton();
    }
}
