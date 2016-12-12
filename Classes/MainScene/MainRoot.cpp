//
//  MainRoot.cpp
//  GoldMiner
//
//  项目GitHub地址:https://github.com/ZhongTaoTian
//  项目思路和架构讲解博客:http://www.jianshu.com/users/5fe7513c7a57/latest_articles
//  微博:http://weibo.com/5622363113/fans?topnav=1&wvr=6&mod=message&need_filter=1

#include "MainRoot.hpp"
#include "Game.hpp"
#include "UserDataManager.hpp"
#include "Shop.hpp"
#include "SoundTool.hpp"
#include "Const.hpp"

Scene *MainLayer::createScene()
{
    Scene *scene = Scene::create();
    
    auto layer = MainLayer::create();
    scene->addChild(layer);
    
    return scene;
}

bool MainLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto mainCsb = CSLoader::createNode("Layer.csb");
    animation = CSLoader::createTimeline("Layer.csb");
    mainCsb->runAction(animation);
    addChild(mainCsb);
    
    cloud1 = static_cast<ImageView *>(Helper::seekWidgetByName(static_cast<Widget *>(mainCsb), "cloud1"));
    cloud2 = static_cast<ImageView *>(Helper::seekWidgetByName(static_cast<Widget *>(mainCsb), "cloud2"));
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/Resources/general-sheet.plist");
    SpriteBatchNode *batchNode = SpriteBatchNode::create("res/Resources/general-sheet.png");
    addChild(batchNode);
    
    leftLeg = Sprite::createWithSpriteFrameName("miner-leg-0.png");
    leftLeg->setPosition(kWinSizeWidth * 0.1579, kWinSizeHeight * 0.1613);
    batchNode->addChild(leftLeg, 10);
    
    face = Sprite::createWithSpriteFrameName("miner-face-whistle-0.png");
    face->setPosition(kWinSizeWidth * 0.2174, kWinSizeHeight * 0.5965);
    batchNode->addChild(face);
    
    light = Sprite::createWithSpriteFrameName("cave-0.png");
    light->setPosition(Vec2(kWinSizeWidth * 0.7083, kWinSizeHeight * 0.445));
    batchNode->addChild(light);
    
    // add button Action
    auto startBtn = static_cast<Button *>(Helper::seekWidgetByName(static_cast<Widget *>(mainCsb), "startButton"));
    startBtn->addTouchEventListener(CC_CALLBACK_2(MainLayer::startButtonTouch, this));
    
    return true;
}

void MainLayer::onEnter()
{
    Layer::onEnter();

    // playAnimation
    animation->gotoFrameAndPlay(0, 25, false);

    // 播放云动画
    moveCloud(cloud1, 10, 20);
    moveCloud(cloud2, 30, 15);
    
    // 播放吹Miner动画
    this->playMinerAnimation(leftLeg, "miner-leg-", 0.15);
    this->playMinerAnimation(face, "miner-face-whistle-", 0.25);
    this->playMinerAnimation(light, "cave-", 0.35);
    
    SoundTool::getInstance()->playBackgroundMusic("res/music/backMusic.mp3");
}

void MainLayer::startButtonTouch(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED) {
        
        kPlayClickSound
        
        if (UserDataManager::getInstance()->getStageNum() <= 1) {
            auto gameScene = Game::createScene(false, false, false, false, 0);
            Director::getInstance()->replaceScene(gameScene);
        } else {
            auto shopScene = Shop::createScene();
            Director::getInstance()->replaceScene(shopScene);
        }
    }
}

void MainLayer::playMinerAnimation(cocos2d::Sprite *sprite, string imageName, float frameDelat)
{
    Vector<SpriteFrame *> frames;
    
    // 播放抖腿动画
    for (int i = 0; i < 3; i++) {
        auto frameName = imageName + to_string(i) + ".png";
        SpriteFrame *sf = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        frames.pushBack(sf);
    }
    
    Animation *shake = Animation::createWithSpriteFrames(frames);
    shake->setDelayPerUnit(frameDelat);
    shake->setRestoreOriginalFrame(true);
    Animate *shakeAn = Animate::create(shake);
    sprite->runAction(RepeatForever::create(shakeAn));
}

void MainLayer::moveCloud(cocos2d::ui::ImageView *cloud, float time1, float time2)
{
    float startX = cloud->getPosition().x;
    MoveTo *move1 = MoveTo::create(time1, Vec2(kWinSizeWidth + 130, cloud1->getPosition().y));
    MoveTo *move2 = MoveTo::create(0, Vec2(-kWinSizeWidth - 130, cloud1->getPosition().y));
    MoveTo *move3 = MoveTo::create(time2, Vec2(startX, cloud1->getPosition().y));
    Sequence *seque = Sequence::create(move1, move2, move3, NULL);
    cloud->runAction(RepeatForever::create(seque));
}
