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
#include "HitzaModel.h"

using namespace cocos2d;

class LevelModel : public Ref {
public:
    
    LevelModel(){};
    ~LevelModel()
    {
        free(m_Izena);
        free(m_Irudia);
        free(m_Maskara);
        free(m_List);
    };
    
    CC_SYNTHESIZE(int, m_ID, ID);
    CC_SYNTHESIZE(char*, m_Izena, Izena);
    CC_SYNTHESIZE(char*, m_Irudia, Irudia);
    CC_SYNTHESIZE(char*, m_Maskara, Markara);
    CC_SYNTHESIZE(char*, m_List, List);
    //CC_SYNTHESIZE(auto , m_Hitzak, Hitzak);
protected: Vector<HitzaModel *> *  m_Hitzak;
public: virtual Vector<HitzaModel *> * getHitzak(void) const { return m_Hitzak; }
public: virtual void setHitzak(Vector<HitzaModel *> * var){ m_Hitzak = var; }

};


#endif
