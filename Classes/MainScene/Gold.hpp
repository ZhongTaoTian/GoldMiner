//
//  Gold.hpp
//  GoldMiner
//
//  项目GitHub地址:https://github.com/ZhongTaoTian
//  项目思路和架构讲解博客:http://www.jianshu.com/users/5fe7513c7a57/latest_articles
//  微博:http://weibo.com/5622363113/fans?topnav=1&wvr=6&mod=message&need_filter=1

#ifndef Gold_hpp
#define Gold_hpp

#include <stdio.h>
#include "Const.hpp"

class Gold : public Sprite{
    
public:
//    enum class GoldType
//    {
//        SMALLGOLD = 101,
//        MIDDLEGOLD,
//        BIGGOLD,
//        SMALLSTONE,
//        MIDDLESTONE,
//        BIGSTONE,
//        GOLDBAG,
//        DIAMONDS
//    };
//    
//    GoldType type;
    float hookRote;
    int score;
    int backSpeed;
    
    static Gold *create(string name, float scaleX, float scaleY, float rotate, bool isBuyPotion, bool isBuyDiamonds, bool isStoneBook);
    virtual bool init(string name, float scaleX, float scaleY,  float rotate, bool isBuyPotion, bool isBuyDiamonds, bool isStoneBook);
    
private:
    
    int stoneCoe = 1;
    int diamondsCoe = 1;
    int power = 1;
};

#endif /* Gold_hpp */
