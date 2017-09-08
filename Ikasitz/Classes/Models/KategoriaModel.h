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
#include "LevelModel.h"

using namespace cocos2d;

class LevelModel;

class KategoriaModel : public Ref {
public:
    
    KategoriaModel(){};
    ~KategoriaModel()
    {
        free(m_Izena);
        free(m_Deskribapena);
        free(m_FileName);
        free(m_ZipURL);
        free(m_Token);
    };
        
    CC_SYNTHESIZE(int, m_ID, ID);
    CC_SYNTHESIZE(char*, m_Izena, Izena);
    CC_SYNTHESIZE(char*, m_Deskribapena, Deskribapena);
    CC_SYNTHESIZE(char*, m_FileName, FileName);
    CC_SYNTHESIZE(char*, m_ZipURL, ZipURL);
    CC_SYNTHESIZE(char*, m_Token, Token);
    //CC_SYNTHESIZE(auto , m_Levels, Pantailak);
protected: Vector<LevelModel *> * m_Levels;
public: virtual Vector<LevelModel *> * getPantailak(void) const { return m_Levels; }
public: virtual void setPantailak(Vector<LevelModel *> * var){ m_Levels = var;}
};

#endif
