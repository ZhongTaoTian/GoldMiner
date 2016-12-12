//
//  Gold.cpp
//  GoldMiner
//
//  项目GitHub地址:https://github.com/ZhongTaoTian
//  项目思路和架构讲解博客:http://www.jianshu.com/users/5fe7513c7a57/latest_articles
//  微博:http://weibo.com/5622363113/fans?topnav=1&wvr=6&mod=message&need_filter=1

#include "Gold.hpp"

// Score
#define kSmallGoldScore 100
#define kMiddleGoldScore 200
#define kBigGoldScore 400
#define kSmallStoneScore 25
#define kMiddleStoneScore 50
#define kBigStoneScore 75
#define kDiamondScore 500
#define kBagScore (arc4random_uniform(200) + 50)

// Speed
#define kSmallGoldBackSpeed 3
#define kMiddleGolBackSpeed 2
#define kBigGoldBackSpeed 1.5
#define kSmallStoneBackSpeed 3
#define kMiddleStoneBackSpeed 2
#define kBigStoneBackSpeed 1.5
#define kBagBackSpeed 3
#define kDiamondSpeed 3

Gold *Gold::create(string name, float scaleX, float scaleY, float rotate, bool isBuyPotion, bool isBuyDiamonds, bool isStoneBook)
{
    Gold *gold = new Gold();
    
    if (gold && gold->init(name, scaleX, scaleY, rotate, isBuyPotion, isBuyDiamonds, isStoneBook)) {
        gold->autorelease();
        return gold;
    } else {
        delete gold;
        gold = nullptr;
    }
    
    return nullptr;
}

bool Gold::init(string name, float scaleX, float scaleY, float rotate, bool isBuyPotion, bool isBuyDiamonds, bool isStoneBook)
{
    // 有时候csb名字会不对,这里过滤下
    string imageName = "";
    for (int i = 0; i < name.size(); i++) {
        auto cc = name[i];
        if (cc != '.') {
            imageName += cc;
        } else {
            break;
        }
    }
    imageName += ".png";
    if (!Sprite::initWithSpriteFrameName(imageName)) {
        return false;
    }
    
    if (isBuyPotion) power = 1.2;
    if (isBuyDiamonds) diamondsCoe = 3;
    if (isStoneBook) stoneCoe = 3;
    
    int scale = ((int)(scaleX * 100));
    this->setScale(scaleX, scaleY);
    this->setRotation(rotate);
    
    this->setAnchorPoint(Vec2(0.5, 0.5));
    
    if (name == "gold-0-0.png" && scale == 40) {
        score = kSmallGoldScore;
        backSpeed = kSmallGoldBackSpeed * power;
        hookRote = 16;
        this->setPosition(7.52, -21.24);
    } else if (name == "gold-0-0.png" && scale == 65) {
        hookRote = 36;
        score = kMiddleGoldScore;
        backSpeed = kMiddleGolBackSpeed * power;
        this->setPosition(2.86, -36.33);
    } else if (name == "pulled-gold-1-0.png" && scale == 40) {
        score = kSmallGoldScore;
        backSpeed = kSmallGoldBackSpeed * power;
        hookRote = 12;
        this->setPosition(7.81, -24.17);
    } else if (name == "pulled-gold-1-0.png" && scale == 65) {
        hookRote = 25;
        score = kMiddleGoldScore;
        backSpeed = kMiddleGolBackSpeed * power;
        this->setPosition(8.51, -35.43);
    } else if (name == "pulled-gold-0-0.png" && scale == 90) {
        hookRote = 35;
        score = kBigGoldScore;
        backSpeed = kBigGoldBackSpeed * power;
        this->setPosition(5.2, -48.17);
    } else if (name == "gold-1-6.png" && scale == 90) {
        hookRote = 35;
        score = kBigGoldScore;
        backSpeed = kBigGoldBackSpeed * power;
        this->setPosition(10.83, -44.7);
    } else if (name == "gold-0-1.png" && scale == 65) {
        hookRote = 30;
        score = kMiddleGoldScore;
        backSpeed = kMiddleGolBackSpeed * power;
        this->setPosition(5.66, -34.47);
    } else if (name == "treasure-bag.png") {
        hookRote = 5;
        score = kBagScore;
        backSpeed = kBagBackSpeed * power;
        this->setPosition(6.31, -33.65);
    } else if (name == "stone-0.png" && scale == 80) {
        hookRote = 15;
        score = kSmallStoneScore * stoneCoe;
        backSpeed = kSmallStoneBackSpeed * power;
        this->setPosition(5.8, -26.07);
    } else if (name == "stone-1.png" && scale == 100) {
        hookRote = 30;
        score = kMiddleStoneScore * stoneCoe;
        backSpeed = kMiddleStoneBackSpeed * power;
        this->setPosition(8.56, -29.8);
    } else if (name == "stone-0.png" && scale == 150) {
        hookRote = 30;
        score = kBigStoneScore * stoneCoe;
        backSpeed = kBigStoneBackSpeed * power;
        this->setPosition(5.27, -42.01);
    } else if (name == "diamond.png") {
        hookRote = 6;
        score = kDiamondScore * diamondsCoe;
        backSpeed = kDiamondSpeed * power;
        this->setPosition(6.38, -26.91);
    } else {
        hookRote = 0;
        backSpeed = 10;
        score = 0;
    }
    
    return true;
}

