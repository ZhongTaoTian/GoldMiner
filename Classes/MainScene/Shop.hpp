//
//  Shop.hpp
//  GoldMiner
//
//  项目GitHub地址:https://github.com/ZhongTaoTian
//  项目思路和架构讲解博客:http://www.jianshu.com/users/5fe7513c7a57/latest_articles
//  微博:http://weibo.com/5622363113/fans?topnav=1&wvr=6&mod=message&need_filter=1

#ifndef Shop_hpp
#define Shop_hpp

#include <stdio.h>
#include "Const.hpp"

class Shop : public Layer {
    
public:
    static Scene * createScene();
    virtual bool init();
    CREATE_FUNC(Shop);
    
private:
    Vector<ImageView *>buyOnes;
    ValueVector goodsDesVec;
    Text *goodsDesText;
    void selectedGoodsButton(Ref *sender, Widget::TouchEventType type);
        
    Button *lastSelected;
    
    int payMoneyCount;
};

#endif /* Shop_hpp */
