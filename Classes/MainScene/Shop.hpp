//
//  Shop.hpp
//  GoldMiner
//
//  Created by sfbest on 2016/12/6.
//
//

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
