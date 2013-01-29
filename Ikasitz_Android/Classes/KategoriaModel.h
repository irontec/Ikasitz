//
//  KategoriaModel.h
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 18/01/13.
//
//

#ifndef Ikasitz_KategoriaModel_h
#define Ikasitz_KategoriaModel_h

#include "cocos2d.h"

class LevelModel;

class KategoriaModel : public CCObject {
public:
    
    KategoriaModel(){};
    ~KategoriaModel(){};
        
    CC_SYNTHESIZE(int, m_ID, ID);
    CC_SYNTHESIZE(char*, m_Izena, Izena);
    CC_SYNTHESIZE(char*, m_Deskribapena, Deskribapena);
    CC_SYNTHESIZE(char*, m_FileName, FileName);
    CC_SYNTHESIZE(char*, m_ZipURL, ZipURL);
    CC_SYNTHESIZE(char*, m_Token, Token);
    CC_SYNTHESIZE(CCArray*, m_Levels, Pantailak);
};

#endif
