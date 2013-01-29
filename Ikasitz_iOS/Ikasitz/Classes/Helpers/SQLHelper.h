//
//  SQLHelper.h
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 18/01/13.
//
//

#ifndef __Ikasitz__SQLHelper__
#define __Ikasitz__SQLHelper__

#include <fstream>
#include "sqlite3.h"
#include "cocos2d.h"


USING_NS_CC;

class KategoriaModel;

class SQLHelper
{
private:
    SQLHelper();
    
    //Variables
    sqlite3 *m_pDB;
    std::string m_dbPath;
    
    //Methods
    void init();
    
    //Create methods
    void createDB();
    
public:
    virtual ~SQLHelper();
    static SQLHelper* sharedInstance();
    
    //Queries -- -1 return all 
    CCArray* queryKategoriak(int idKategoria);
    
    //Update
    void updateKategoria(KategoriaModel *pKat);
    
    //Insert
    void insertKategoria(KategoriaModel *pKat);
     
};


#endif /* defined(__Ikasitz__SQLHelper__) */
