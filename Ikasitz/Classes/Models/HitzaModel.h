//
//  HitzaModel.h
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 21/01/13.
//
//

#ifndef Ikasitz_HitzaModel_h
#define Ikasitz_HitzaModel_h

#include "cocos2d.h"

using namespace cocos2d;

class HitzaModel : public Ref {
public:
    
    HitzaModel(){};
    ~HitzaModel(){};
    
    CC_SYNTHESIZE(int, m_ID, ID);
    CC_SYNTHESIZE(char*, m_Izena, Izena);
    CC_SYNTHESIZE(char*, m_Soinua, Soinua);
    CC_SYNTHESIZE(Color3B, m_Color, Kolorea);
};


#endif
