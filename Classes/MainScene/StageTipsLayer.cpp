//
//  StageTipsLayer.cpp
//  GoldMiner
//
//  项目GitHub地址:https://github.com/ZhongTaoTian
//  项目思路和架构讲解博客:http://www.jianshu.com/users/5fe7513c7a57/latest_articles
//  微博:http://weibo.com/5622363113/fans?topnav=1&wvr=6&mod=message&need_filter=1

#include "StageTipsLayer.hpp"
#include "MainRoot.hpp"
#include "UserDataManager.hpp"
#include "Shop.hpp"

bool StageTipsLayer::init(int stageNum)
{
    if (!Layer::init()) {
        return false;
    }
    
    auto csb = static_cast<Widget *>(CSLoader::createNode("StageTipsLayer.csb"));
    this->addChild(csb);
    
    image = static_cast<ImageView *>( Helper::seekWidgetByName(csb, "image"));
    stageNumText = static_cast<Text *>( Helper::seekWidgetByName(csb, "stageNum"));
    
    string stageNumStr = "关卡  " + to_string(stageNum);
    stageNumText->setString(stageNumStr);
    stageNumText->setOpacity(0);
    
    string passScroe = "目标分数  " + to_string(650 + 275 * (stageNum - 1) + 410 * (stageNum - 1));
    passScoreText = static_cast<Text *>( Helper::seekWidgetByName(csb, "passScore"));
    passScoreText->setString(passScroe);
    
    FadeTo *fade = FadeTo::create(0.2, 255);
    Sequence *sequence = Sequence::create(fade, CallFunc::create([=](){
        moveAnimation(image, Vec2(kWinSizeWidth * 0.32, kWinSizeHeight * 0.3), NULL);
        moveAnimation(passScoreText, Vec2(kWinSizeWidth * 0.84, kWinSizeHeight * 0.29), [=](){
            // 执行第一次动画完成回调
            this->runAction(Sequence::create(DelayTime::create(1.2), CallFunc::create([=](){
                this->moveAnimation(image, Vec2(-kWinSizeWidth * 0.32,  kWinSizeHeight * 0.3), NULL);
                this->moveAnimation(passScoreText, Vec2(kWinSizeWidth * 1.3, kWinSizeHeight * 0.29), [=](){
                    if (this->showFinish != nullptr && this->showFinish != NULL) {
                        this->showFinish();
                    }
                    this->removeFromParentAndCleanup(true);
                });

            }), NULL));
        });
        
    }), NULL);
    stageNumText->runAction(sequence);
    
    return true;
}

void StageTipsLayer::moveAnimation(cocos2d::Node *node, cocos2d::Vec2 position, AnimationFinish animFinish)
{
    MoveTo *move = MoveTo::create(0.35, position);
    
    if (animFinish != NULL) {
        Sequence *sequence = Sequence::create(move, CallFunc::create(animFinish), NULL);
        node->runAction(sequence);
    } else {
        node->runAction(move);
    }
}

StageTipsLayer * StageTipsLayer::create(int stageNum)
{
    StageTipsLayer *pRet = new StageTipsLayer();
    if (pRet && pRet->init(stageNum))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

void StageTipsLayer::showStageTipsLayer(cocos2d::Node *parent, int stageNum, AnimationFinish showFinish)
{
    auto layer = StageTipsLayer::create(stageNum);
    layer->showFinish = showFinish;
    parent->addChild(layer, 100);
}

StageFailOrSucessLayer *StageFailOrSucessLayer::create(StageFailOrSucessLayer::TipsType type, int goldCount, int curPayMoney)
{
    StageFailOrSucessLayer *pRet = new StageFailOrSucessLayer();
    if (pRet && pRet->init(type, goldCount, curPayMoney))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool StageFailOrSucessLayer::init(StageFailOrSucessLayer::TipsType type, int goldCount, int curPayMoney)
{
    if (!Layer::init()) {
        return false;
    }
    
    auto csb = CSLoader::createNode("TipsFailOrSucess.csb");
    this->addChild(csb);
    
    if (type == TipsType::FAIL) {
        auto *failText = Helper::seekWidgetByName(static_cast<Widget *>(csb), "FailText");
        
        auto seqe = Sequence::create(MoveTo::create(0.3, Vec2(kWinSizeWidth * 0.5, kWinSizeHeight * 0.5)), DelayTime::create(2.5), CallFuncN::create([=](Node *node){
            
            Director::getInstance()->replaceScene(MainLayer::createScene());
            
        }),NULL);
        failText->runAction(seqe);
        
    } else {
        auto *sucessText = Helper::seekWidgetByName(static_cast<Widget *>(csb), "passStage");
        auto *goldCountText = Helper::seekWidgetByName(static_cast<Widget *>(csb), "goldCount");
        (static_cast<Text *>(goldCountText))->setString("本关挖到的金币为: " + to_string(goldCount));

        auto move = MoveTo::create(0.3, Vec2(kWinSizeWidth * 0.5, kWinSizeHeight * 0.6));
        sucessText->runAction(move);
        
        auto seqe = Sequence::create(MoveTo::create(0.3, Vec2(kWinSizeWidth * 0.5, kWinSizeHeight * 0.4)), DelayTime::create(1.5), CallFuncN::create([=](Node *node){
            // 过关成功,保存数据
            UserDataManager *user = UserDataManager::getInstance();
            user->setAllMoney(user->getAllMoney() - curPayMoney + goldCount);
            user->setStageNum(user->getStageNum()+1);
            user->saveUserData();
            
            // 进入商店
            Scene *shopScene = Shop::createScene();
            Director::getInstance()->replaceScene(shopScene);
        }),NULL);
        goldCountText->runAction(seqe);
    }
    
    return true;
}

void StageFailOrSucessLayer::showTips(cocos2d::Node *parent, StageFailOrSucessLayer::TipsType type, int goldCount, int curPayMoney) {
    auto layer = StageFailOrSucessLayer::create(type, goldCount, curPayMoney);
    parent->addChild(layer, 1002);
}




