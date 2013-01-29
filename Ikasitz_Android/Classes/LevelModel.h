//
//  LevelModel.h
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 21/01/13.
//
//

#ifndef Ikasitz_LevelModel_h
#define Ikasitz_LevelModel_h

#include "cocos2d.h"

USING_NS_CC;

class LevelModel : public CCObject {
public:
    
    LevelModel(){};
    ~LevelModel(){};
    
    CC_SYNTHESIZE(int, m_ID, ID);
    CC_SYNTHESIZE(char*, m_Izena, Izena);
    CC_SYNTHESIZE(char*, m_Irudia, Irudia);
    CC_SYNTHESIZE(char*, m_Maskara, Markara);
    CC_SYNTHESIZE(char*, m_List, List);
    CC_SYNTHESIZE(CCArray*, m_Hitzak, Hitzak);
};


#endif
