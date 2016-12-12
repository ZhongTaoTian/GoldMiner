//
//  Game.cpp
//  GoldMiner
//
//  Created by 维尼的小熊 on 16/11/27.
//  项目GitHub地址:https://github.com/ZhongTaoTian
//  项目思路和架构讲解博客:http://www.jianshu.com/users/5fe7513c7a57/latest_articles
//  微博:http://weibo.com/5622363113/fans?topnav=1&wvr=6&mod=message&need_filter=1
//

#include "Game.hpp"
#include "Pause.hpp"
#include "UserDataManager.hpp"
#include "StageTipsLayer.hpp"
#include "Shop.hpp"

#define kWorldTag 1000

Scene *Game::createScene(bool isBuyBomb, bool isBuyPotion, bool isBuyDiamonds, bool isStoneBook, int payMoney)
{
    Scene *scene = Scene::createWithPhysics();
    
    auto world = scene->getPhysicsWorld();
//    world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    world->setGravity(Vec2::ZERO);
    
    auto gameLayer = Game::create(isBuyBomb, isBuyPotion, isBuyDiamonds, isStoneBook, payMoney);
    scene->addChild(gameLayer);
    
    PhysicsBody *body = PhysicsBody::createEdgeBox(Size(kWinSizeWidth, kWinSizeHeight));
    body->setCategoryBitmask(10);
    body->setCollisionBitmask(10);
    body->setContactTestBitmask(10);
    
    Node *node = Node::create();
    node->setPosition(kWinSize * 0.5);
    node->addComponent(body);
    node->setColor(Color3B::RED);
    node->setTag(kWorldTag);
    scene->addChild(node);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/Resources/level-sheet.plist");
    
    return scene;
}

Game *Game::create(bool isBuyBomb, bool isBuyPotion, bool isBuyDiamonds, bool isStoneBook, int payMoney)
{
    Game *pRet = new Game();
    if (pRet && pRet->init(isBuyBomb, isBuyPotion, isBuyDiamonds, isStoneBook, payMoney))
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

bool Game::init(bool isBuyBomb, bool isBuyPotion, bool isBuyDiamonds, bool isStoneBook, int payMoney)
{
    if (!Layer::init()) return false;
    
    auto csb = CSLoader::createNode("GameLayer.csb");
    this->addChild(csb, 10);
    
    this->isBuyPotion = isBuyPotion;
    this->isBuyDiamonds = isBuyDiamonds;
    this->isBuyStoneBook = isStoneBook;
    
    bompButton = static_cast<Button *>(Helper::seekWidgetByName(static_cast<Widget *>(csb), "BompButton"));
    bompButton->setVisible(isBuyBomb);
    bompButton->addTouchEventListener([=](Ref *ref, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            // click bomp
            if (isOpenHook) {
                bompButton->setVisible(false);
                // 炸毁物品
                backSpeed = 10;
                
                isOpenHook = false;
                leftHook->setRotation(0);
                rightHook->setRotation(0);
                
                //爆炸效果
                auto postion = rope->convertToWorldSpace(middleCircle->getPosition());
                ParticleSystemQuad *bompEm = ParticleSystemQuad::create("res/Boom.plist");
                bompEm->setPosition(postion);
                this->addChild(bompEm);
                
                SoundTool::getInstance()->playEffect("res/music/bomb.mp3");
                
                goldSprite->removeFromParent();
                
            }
        }
    });
    
    // 获取序列帧动画
    minerTimeLine = CSLoader::createTimeline("GameLayer.csb");
    this->runAction(minerTimeLine);
    
    auto hookCsb = CSLoader::createNode("Hook.csb");
    hookCsb->setPosition(kWinSizeWidth * 0.48, kWinSizeHeight * 0.856);
    this->addChild(hookCsb, 11);
    
    rope = static_cast<ImageView *>(Helper::seekWidgetByName(static_cast<Widget *>(hookCsb), "rope"));
    middleCircle = static_cast<Sprite *>(rope->getChildByTag(59));
    leftHook = static_cast<Sprite *>(middleCircle->getChildByTag(60));
    rightHook = static_cast<Sprite *>(middleCircle->getChildByTag(61));
    curPayMoney = payMoney;
    
    // 添加钩子刚体
    PhysicsBody *hookBody = PhysicsBody::createCircle(20);
    hookBody->setContactTestBitmask(10);
    hookBody->setCollisionBitmask(10);
    hookBody->setCategoryBitmask(10);
    middleCircle->addComponent(hookBody);
    circlePosition = middleCircle->getPosition();
    this->addButtonAction(csb);
    
    setUpText(static_cast<Widget *>(csb));
    
    timeCount = 60;
    
    // 添加碰撞事件
    auto physicsListener = EventListenerPhysicsContact::create();
    physicsListener->onContactBegin = CC_CALLBACK_1(Game::physicsBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(physicsListener, this);
    _eventDispatcher->removeCustomEventListeners("nextStage");
    _eventDispatcher->addCustomEventListener("nextStage", [=](EventCustom *cus){
        // 过关成功,保存数据
        UserDataManager *user = UserDataManager::getInstance();
        user->setAllMoney(UserDataManager::getInstance()->getAllMoney() - curPayMoney + curStageScore);
        user->setStageNum(user->getStageNum()+1);
        user->saveUserData();
        
        // 进入商店
        Scene *shopScene = Shop::createScene();
        Director::getInstance()->replaceScene(shopScene);
    });
    
    loadStageInfo();

    return true;
}

bool Game::physicsBegin(cocos2d::PhysicsContact &contact)
{
    if (contact.getShapeB()->getBody()->getNode()->getTag() != kWorldTag) {
        // 碰到金块, 打开钩子
        if (!isOpenHook) {
            this->pullGold(contact);
        }
    } else {
        this->backSpeed = 10;
    }
    
    this->unschedule(CC_SCHEDULE_SELECTOR(Game::addRopeHeight));
    this->schedule(CC_SCHEDULE_SELECTOR(Game::subRopeHeight), 0.025);
    
    return true;
}

void Game::loadStageInfo()
{
    // 加载关卡信息
    int stageNum = UserDataManager::getInstance()->getStageNum();
    int levelIndex = stageNum % 5;
    if (levelIndex == 0) {
        levelIndex = 5;
    }
    
    string levelCsbName = "level" + to_string(levelIndex) + ".csb";
    auto goldCsb = CSLoader::createNode(levelCsbName);
    this->addChild(goldCsb);
    
    // 所有金块
    auto goldsLayout = Helper::seekWidgetByName(static_cast<Widget *>(goldCsb), "goldPanel");
    Vector<Node *> golds = goldsLayout->getChildren();
    for (Node *subNode : golds) {
        Size bodySize = Size(subNode->getContentSize().width * subNode->getScaleX(), subNode->getContentSize().height * subNode->getScaleY());
        PhysicsBody *goldBody = PhysicsBody::createEdgeBox(bodySize);
        goldBody->setCategoryBitmask(10);
        goldBody->setCollisionBitmask(10);
        goldBody->setContactTestBitmask(10);
        subNode->addComponent(goldBody);
    }
}

void Game::pullGold(cocos2d::PhysicsContact &contact)
{
    // 钓到了东西
    isOpenHook = true;
    
    auto gold = contact.getShapeB()->getBody()->getNode();
    
    goldSprite = Gold::create(gold->getName(), gold->getScaleX(), gold->getScaleY(), gold->getRotation(), isBuyPotion, isBuyDiamonds, isBuyStoneBook);
    middleCircle->addChild(goldSprite);
    contact.getShapeB()->getBody()->removeFromWorld();
    this->backSpeed = goldSprite->backSpeed;
    leftHook->runAction(RotateTo::create(0.05, -goldSprite->hookRote));
    rightHook->runAction(RotateTo::create(0.05, goldSprite->hookRote));
    
    gold->getPhysicsBody()->setEnabled(false);
    gold->setVisible(false);
}

void Game::subRopeHeight(float dt)
{
    middleCircle->setPosition(circlePosition);
    
    ropeHeight -= backSpeed;
    if (ropeHeight <= 20) {
        ropeHeight = 20;
        minerTimeLine->pause();
        // 恢复原样, 继续摇摆
        this->startShakeHookAnimation();
        this->unschedule(CC_SCHEDULE_SELECTOR(Game::subRopeHeight));
        ropeChangeing = false;
        
        if (isOpenHook) {
            isOpenHook = false;
            leftHook->setRotation(0);
            rightHook->setRotation(0);
            
            if (goldSprite != nullptr) {
                // 加分动画
                Label *scoreLabel = Label::create();
                scoreLabel->setColor(Color3B(50, 200, 0));
                scoreLabel->setSystemFontSize(25);
                scoreLabel->setString(to_string(goldSprite->score));
                scoreLabel->setPosition(rope->convertToWorldSpace(middleCircle->getPosition()));
                this->addChild(scoreLabel, 1000);
                
                SoundTool::getInstance()->playEffect("res/music/laend.mp3");
                
                curStageScore += goldSprite->score;
                auto spawn = Spawn::create(MoveTo::create(0.5, Vec2(allMoney->getPosition().x + 10, allMoney->getPosition().y)), Sequence::create(ScaleTo::create(0.25, 3), ScaleTo::create(0.25, 0.1), NULL), NULL);
                auto seque = Sequence::create(spawn, CallFuncN::create([=](Node *node){
                    
                    scoreLabel->removeFromParent();
                    allMoney->setString(to_string(curStageScore + UserDataManager::getInstance()->getAllMoney() - curPayMoney));
                    
                }),NULL);
                scoreLabel->runAction(seque);
                
                // 加分
                goldSprite->removeFromParent();
                goldSprite = nullptr;
            }
        }
    }
    
    rope->setSize(Size(3, ropeHeight));
}

void Game::setUpText(Widget *csb)
{
    auto userManager = UserDataManager::getInstance();
    
    allMoney = static_cast<Text *>(Helper::seekWidgetByName(csb, "allMoney"));
    targetMoney = static_cast<Text *>(Helper::seekWidgetByName(csb, "passScore"));
    stageNum = static_cast<Text *>(Helper::seekWidgetByName(csb, "stage"));
    time = static_cast<Text *>(Helper::seekWidgetByName(csb, "time"));
    
    int stageIndex = userManager->getStageNum();
    allMoney->setString(to_string(userManager->getAllMoney() - curPayMoney));
    stageNum->setString(to_string(stageIndex));
    time->setString("60");
    passScroe = 650 + 275 * (stageIndex - 1) + 410 * (stageIndex - 1);
    targetMoney->setString(to_string(passScroe));
}

void Game::onEnter()
{
    Layer::onEnter();
    
    if (!showStageTips) {
        showStageTips = true;
        
        SoundTool::getInstance()->playEffect("res/music/level.mp3");
        
        StageTipsLayer::showStageTipsLayer(this, UserDataManager::getInstance()->getStageNum(), [=](){
            this->startGame();
        });
    }
}

void Game::updateTime(float dt)
{
    timeCount--;
    time->setString(to_string(timeCount));
    
    if (timeCount == 0) {
        this->stopGame();
    }
}

void Game::startGame()
{
    // 添加点击事件
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(Game::touchCallBack, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->startShakeHookAnimation();
    schedule(CC_SCHEDULE_SELECTOR(Game::updateTime), 1, 59, 0);
}

void Game::startShakeHookAnimation()
{
    rope->setRotation(0);
    
    float duration = 1;
    float angle = 65;
    rope->runAction(RepeatForever::create(Sequence::create(RotateTo::create(duration, angle), RotateTo::create(duration, 0), RotateTo::create(duration, -angle), RotateTo::create(duration, 0), NULL)));
}

void Game::stopGame()
{
    // 停止一切时间
    rope->stopAllActions();
    this->stopAllActions();
    this->unscheduleAllSelectors();
    
    SoundTool::getInstance()->playEffect("res/music/finish.mp3");
    
    // 判断获取的分数是否能过关
    if (passScroe > (UserDataManager::getInstance()->getAllMoney() - curPayMoney + curStageScore)) {
        // 分数不够
        StageFailOrSucessLayer::showTips(this, StageFailOrSucessLayer::TipsType::FAIL, curStageScore, 0);
    } else {
        // Pass
        StageFailOrSucessLayer::showTips(this, StageFailOrSucessLayer::TipsType::SUCESS, curStageScore, curPayMoney);
    }
}

bool Game::touchCallBack(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (!ropeChangeing) {
        
        SoundTool::getInstance()->playEffect("res/music/lastart.mp3");
        
        rope->stopAllActions();
        ropeChangeing = true;
        minerTimeLine->gotoFrameAndPlay(0, 105, true);
        schedule(CC_SCHEDULE_SELECTOR(Game::addRopeHeight), 0.025);
    }
    
    return false;
}

void Game::addRopeHeight(float dt)
{
    middleCircle->setPosition(circlePosition);
    ropeHeight += 10;
    rope->setSize(Size(3, ropeHeight));
}

void Game::addButtonAction(Node *csbNode)
{
    Button *settingBtn = static_cast<Button *>(Helper::seekWidgetByName(static_cast<Widget *>(csbNode), "settingButton"));
    settingBtn->addTouchEventListener([=](Ref *sender, Widget::TouchEventType type){
        // 设置窗口
        if (type == Widget::TouchEventType::ENDED) {
            Pause::showPause(Director::getInstance()->getRunningScene(), passScroe <= (UserDataManager::getInstance()->getAllMoney() - curPayMoney + curStageScore));
            this->onExit();
        }
    });
}
