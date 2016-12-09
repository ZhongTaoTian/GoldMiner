//
//  Gold.hpp
//  GoldMiner
//
//  Created by sfbest on 2016/12/2.
//
//

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
