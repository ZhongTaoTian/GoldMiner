#黄金矿工
好久没有写点东西了,最近工作有一点忙,生活上事情也比较繁琐,趁着最近有点时间,写个小游戏供大家娱乐下!随便恭喜木木同学被挖去了新公司,祝工作顺利~

##关于项目(代码下载地址在文章最下面点击GitHub链接)
项目说明:采用cocos2d-X3.12游戏引擎,基于C++开发,支持Android,iOS以及wp系统

开发工具:支持Xcode,eclipse,visual studio都可以,提前在机器上创建一个cocos2d-x的空工程,将Class文件删除,把下载好的代码Class文件拖入到工程,把res文件放入到项目的Resoures目录下运行就OK了~

辅助工具:`Cocostudio`

项目比较简单,适合有一定编程经验对游戏有兴趣想入门的同学.

![游戏截图](http://ww4.sinaimg.cn/mw690/0068uRu1jw1fakp2ilr4og306o04g1ky.gif)

![游戏截图](http://ww2.sinaimg.cn/mw690/0068uRu1jw1fakp2l0lzfg306o04g4qr.gif)

![游戏截图](http://ww1.sinaimg.cn/mw690/0068uRu1jw1fakp2hy574j30df09075z.jpg)

![粒子效果](http://ww2.sinaimg.cn/mw690/0068uRu1jw1fakp2hig76g306o04g1kz.gif)

###首页场景
本游戏的布局基本都是cocostudio布局的,放一张在cocostudio中布局的样式图

![cocostudio布局样式](http://ww2.sinaimg.cn/mw690/0068uRu1gw1famxlucyp7j311g142akx.jpg)

通过CSLoder加载csb文件添加到对应的场景中展示,具体代码如下
```cpp
auto mainCsb = CSLoader::createNode("csb文件名");
this->addChild(mainCsb);
```

Logo放大出现动画也在cocostudio中创建,通过CSLoader获取Timeline对象,播放指定的帧动画
```cpp
    animation = CSLoader::createTimeline("Layer.csb");
    mainCsb->runAction(animation);
    // 播放指定的动画
    animation->gotoFrameAndPlay(0, 25, false);
```

云飘动动画以及人物吹口哨抖腿的动画都是通过代码实现的,当然也可以在cocostudio中制作骨骼动画,通过在工程中加载导出的js文件播放动画也可以,这里的动画比较简单,就直接通过代码实现了,如果项目中用到比较复杂的动画推荐采用加载js动画的方式.这里由于没有用到骨骼动画,就不做相应的介绍了,有兴趣的同学可以自己研究下~

 由于游戏需要用到存储的数据比较小,这里对用户游戏数据持久化存储是采用`UserDefault`来进行.如果项目中需要存储的数据量比较庞大,建议使用数据库建表来进行存储,这样更方便数据的查询与管理.

 在工程中通过一个单例`UserDataManager`来管理用户的数据,提供背景音乐是否静音,音效是否静音,用户账户金币以及当前游戏的关数四个成员变量.
 ```c++
 UserDataManager *UserDataManager::getInstance()
{
    if (s_SharedUserDataManager == nullptr) {
        s_SharedUserDataManager = new UserDataManager();

        s_SharedUserDataManager->_musicMute = UserDefault::getInstance()->getBoolForKey(musicMuteKey, false);
        s_SharedUserDataManager->_soundMute = UserDefault::getInstance()->getBoolForKey(soundMuteKey, false);
        s_SharedUserDataManager->_allMoney = UserDefault::getInstance()->getIntegerForKey(userAllMoneyKey, 0);
        s_SharedUserDataManager->_stageNum = UserDefault::getInstance()->getIntegerForKey(userStageNumKey, 1);
    }

    return s_SharedUserDataManager;
}
 ```

监听按钮的点击事件,可以在cocostudio中对节点进行命名,然后在代码中通过下面方法获取对应name的节点
```c++
    // 获取startButton节点
    auto startBtn = static_cast<Button *>(Helper::seekWidgetByName(static_cast<Widget *>(mainCsb), "startButton"));

```

需要注意的是,按钮点击回调函数需要在.h文件提前声明,或者可以采用lambda表达式也可
 - 采用函数的回调写法
     // 添加按钮的事件
    ```c++
tartBtn->addTouchEventListener(CC_CALLBACK_2(MainLayer::startButtonTouch, this));
```
 - 采用lambda表达式写法
    ```c++
       startBtn->addTouchEventListener([=](Ref *sender, Widget::TouchEventType touchType){
        // button click callBack
    });
    ```
StartButton点击后会有两个逻辑:通过`UserDataManager`获取用户游戏关数
  - 用户没有游戏记录或者当前记录是游戏是第一关,直接进入游戏场景,开始游戏.
  - 用户有游戏记录并且关数大于1,直接进入商店场景.

###商店场景
同样也是通过采用cocostudio来进行布局的,效果如下
![商店场景cocostudio效果](http://ww1.sinaimg.cn/mw690/0068uRu1gw1famyje8uddj318s0z6k14.jpg)

商店场景也比较简单,账户余额通过上文中的`UserDataManager`可以获取用户的金币数.
```
UserDataManager::getInstance()->getUserAllMoney();
```

商品采用Button来展示,这样可以获取玩家选择的当前商品,每一个商品只能购买一次,如果购买了商品后,对应商品上显示1的图标.商品描述通过点击商品,展示对应的商品作用描述.

```c++
auto csb = CSLoader::createNode("ShopScene.csb");
    this->addChild(csb);

    // 添加商品描述容器
    goodsDesVec.push_back(Value("炸药.购买以后,当抓到较重且金额不多的物品时,按下上方炸药即可炸毁物品,以便节省时间.功效为下一关"));
    goodsDesVec.push_back(Value("力量药水.购买以后,在下一关力量会增加,抓到物品后拉回速度会增加20%.功效为下一关"));
    goodsDesVec.push_back(Value("优质矿石.购买后在下一关中收购钻石的价格将变成原价格的3倍,但不保证下一关一定会有钻石~其效果为下一关"));
    goodsDesVec.push_back(Value("矿石收藏书.购买后下一关的矿石的价格将会是原有价格的3倍,其功效为下一关"));

    // 初始化商品描述Text
    goodsDesText = static_cast<Text *>(Helper::seekWidgetByName(static_cast<Widget *>(csb), "shopDetail"));
    Text *userMoney = static_cast<Text *>(Helper::seekWidgetByName(static_cast<Widget *>(csb), "userMoney"));
    userMoney->setString("$" + to_string(UserDataManager::getInstance()->getAllMoney()));

    // 获取购买按钮.并且添加按钮的点击事件
    Button *buyButton = static_cast<Button *>(Helper::seekWidgetByName(static_cast<Widget *>(csb), "buyButton"));
    buyButton->addTouchEventListener([=](Ref *sender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED) {
            int index = lastSelected->getTag() - 1;

            auto oneIV = buyOnes.at(index);
            if (oneIV->isVisible()) {
                return;
            }
            // 获取商品价格
            int price = 0;
            switch (index) {
                case 0:
                    price = kBombPrice;
                    break;
                case 1:
                    price = kPotionPrice;
                    break;
                case 2:
                    price = kDiamondsPrice;
                    break;
                case 3:
                    price = kStoneBookPrice;
                    break;
            }

            if (UserDataManager::getInstance()->getAllMoney() - payMoneyCount - price >= 0) {
                payMoneyCount += price;
                userMoney->setString("$" + to_string(UserDataManager::getInstance()->getAllMoney() - payMoneyCount));
                oneIV->setVisible(true);
            }
        }
    });

```

点击下一关,切换到游戏场景.

###游戏场景
一样也是在cocostudio中布局,这里需要注意的是并不是采用一个csb文件就全部将节点添加完毕,这里分三块布局,如下图所示三块
![顶部](http://ww4.sinaimg.cn/mw690/0068uRu1gw1famz3276kvj30u00k4401.jpg)
![level](http://ww4.sinaimg.cn/mw690/0068uRu1gw1famz31cvksj30tq0jwmzy.jpg)
![钩子](http://ww4.sinaimg.cn/mw690/0068uRu1gw1famz31q41tj308o06ua9y.jpg)

游戏Layer提供一个快速创建场景的方法
```c++
// 参数分别为 是否购买了炸弹.力量药水.砖石升值书.石头收藏书.在商店的花销
static Scene *createScene(bool isBuyBomb, bool isBuyPotion, bool isBuyDiamonds, bool isStoneBook, int payMoney);
```
在上面函数中,创建游戏场景,给游戏场景添加刚体世界,用户后期进行碰撞的响应,代码如下
```c++
   Scene *scene = Scene::createWithPhysics();

    auto world = scene->getPhysicsWorld();
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
```

在游戏Layer的init初始化方法中,通关用户当前关数获取对应的level.csb文件,然后添加到游戏的layer中.获取level.csb中的所有矿石节点,并且给每个矿石添加刚体body,以便后期进行与钩子的碰撞事件.
```c++
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

    loadStageInfo();

    return true;
}
```

进入游戏场景后,首先展示关卡过关提示,通过用户当前关卡数计算出过关需要的金额.展示完毕后,正式开始游戏.
 - 添加点击屏幕点击事件
 ```
     // 添加点击事件
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(Game::touchCallBack, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
 ```
 - 开始钩子左右摆动动画
 ```
 void Game::startShakeHookAnimation()
{
    float duration = 1;
    float angle = 65;
    rope->runAction(RepeatForever::create(Sequence::create(RotateTo::create(duration, angle), RotateTo::create(duration, 0), RotateTo::create(duration, -angle), RotateTo::create(duration, 0), NULL)));
}
 ```
 - 开启倒计时
```
schedule(CC_SCHEDULE_SELECTOR(Game::updateTime), 1, 59, 0);
```

当屏幕点击时,如果钩子是在摇摆阶段,停止钩子摇摆动画,开始伸长钩子动画
```c++
bool Game::touchCallBack(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (!ropeChangeing) {
        rope->pause();
        ropeChangeing = true;
        minerTimeLine->gotoFrameAndPlay(0, 105, true);
        schedule(CC_SCHEDULE_SELECTOR(Game::addRopeHeight), 0.025);
    }

    return false;
}
```

等待钩子碰撞的回调,如果碰撞后,不是场景的边缘则代表钩到了矿石
```c++
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
```

碰撞后停止钩子伸长动画,执行钩子拉回函数
```c++
void Game::subRopeHeight(float dt)
{
    middleCircle->setPosition(circlePosition);

    ropeHeight -= backSpeed;
    if (ropeHeight <= 20) {
        ropeHeight = 20;
        minerTimeLine->pause();
        // 恢复原样, 继续摇摆
        rope->resume();
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

    //爆炸效果
    CCParticleSystem* particleSystem = CCParticleExplosion::create();
    particleSystem->setTexture(CCTextureCache::sharedTextureCache()->addImage("stars.png"));
    addChild(particleSystem);

    rope->setSize(Size(3, ropeHeight));
}
```

### 项目总结
项目写的比较匆忙,并且cocos2d-X更新到3.0+版本后,好多函数都弃用了...框架内部还是有许多Bug,当然我相信这难不倒大家的~

感觉光靠文字来讲述一个项目实在是太困难.希望大家还是参考工程代码,当遇到无法看懂或者不理解的时候参考下我写的Blog应该会更好一些.这个游戏项目说实话还是非常简单的,相信大家仔细研究下都可以实现的.

好久没用C++了T_T,有什么问题和不足之处大家同样还是可以留言.

以后我会分享一些有意思的小项目.希望朋友继续关注维尼的小熊.


### 代码下载地址(如果觉得有帮助,请点击Star★)
[代码下载地址,记得Star★和Follow](https://github.com/ZhongTaoTian/GoldMiner)
#### 小熊的技术博客

[点击链接我的博客,欢迎关注](http://www.jianshu.com/users/5fe7513c7a57/latest_articles)

### 小熊的新浪微博
[我的新浪微博,欢迎关注](http://weibo.com/5622363113/profile?topnav=1&wvr=6)
