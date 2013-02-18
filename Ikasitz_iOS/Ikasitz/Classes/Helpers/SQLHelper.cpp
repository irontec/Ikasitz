//
//  SQLHelper.cpp
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 18/01/13.
//
//

#include "SQLHelper.h"
#include "KategoriaModel.h"


static SQLHelper* _instance = NULL;

SQLHelper* SQLHelper::sharedInstance()
{
    if (_instance == NULL)
        _instance = new SQLHelper();
    return _instance;
}


SQLHelper::~SQLHelper()
{
    sqlite3_close(m_pDB);
}

SQLHelper::SQLHelper()
{
    init();
}

void SQLHelper::init()
{
    int result;
    //Check database if exist
    CCFileUtils *pFileUtils = CCFileUtils::sharedFileUtils();
    m_dbPath = pFileUtils->getWriteablePath();
    
    m_dbPath.append("Ikasitz.db");
    std::ifstream f(m_dbPath.c_str());
    
    if(!f)
    {
        //Database not exist
        result = sqlite3_open(m_dbPath.c_str(),&m_pDB);
        createDB();
    } else {
        result = sqlite3_open(m_dbPath.c_str(),&m_pDB);
    }
    
    if (result != SQLITE_OK)
        CCLOG("OPENING WRONG, %d", result);
}

void SQLHelper::createDB()
{
    char* errMsg = NULL;
    int result;
    
    CCLOG("CREATING GAME TABLES");
    
    result = sqlite3_exec(m_pDB, "CREATE TABLE Pantaila (id INTEGER, puntuazioa INTEGER)", NULL, 0, &errMsg);
    
    if(result != SQLITE_OK)
        CCLOG("DB error with result %d Msg: %s", result, errMsg);
    
    result = sqlite3_exec(m_pDB, "CREATE TABLE Kategoria (id INTEGER, izena TEXT, deskribapena TEXT, izenGarbia TEXT, zipUrl TEXT, token TEXT)", NULL, 0, &errMsg);
    
    if(result != SQLITE_OK)
        CCLOG("DB error with result %d Msg: %s", result, errMsg);
    
    sqlite3_free(errMsg);

}

void SQLHelper::insertKategoria(KategoriaModel *pKat)
{
    sqlite3_stmt *stmt;
    int colIndex = 0;
    
    sqlite3_prepare_v2(m_pDB, "SELECT COUNT(*) FROM Kategoria WHERE id = ?", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, pKat->getID());
    
    sqlite3_step(stmt);
    
    int num = sqlite3_column_int(stmt, 0);
    
    //Ez bada existitzen sortu multzoa 0 baloreekin
    if(num == 0) {
        
        sqlite3_prepare_v2(m_pDB,"INSERT INTO Kategoria VALUES (?,?,?,?,?,?)", -1, &stmt, NULL);
        sqlite3_bind_int(stmt, ++colIndex, pKat->getID());
        sqlite3_bind_text(stmt, ++colIndex, pKat->getIzena(), -1, NULL);
        sqlite3_bind_text(stmt, ++colIndex, pKat->getDeskribapena(), -1, NULL);
        sqlite3_bind_text(stmt, ++colIndex, pKat->getFileName(), -1, NULL);
        sqlite3_bind_text(stmt, ++colIndex, pKat->getZipURL(), -1, NULL);
        sqlite3_bind_text(stmt, ++colIndex, pKat->getToken(), -1, NULL);
        
        
        if (sqlite3_step(stmt) != SQLITE_DONE)
            CCLog("ERROR INSERTING KATEGORIA");
        
        sqlite3_finalize(stmt);
    } else {
        updateKategoria(pKat);
    }

}

void SQLHelper::updateKategoria(KategoriaModel *pKat)
{
    sqlite3_stmt *stmt;
    int colIndex = 0;
        
    sqlite3_prepare_v2(m_pDB, "UPDATE Kategoria SET izena=?, deskribapena=?, izenGarbia=?, zipUrl=?, token=? WHERE id = ?", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, ++colIndex, pKat->getIzena(), -1, NULL);
    sqlite3_bind_text(stmt, ++colIndex, pKat->getDeskribapena(), -1, NULL);
    sqlite3_bind_text(stmt, ++colIndex, pKat->getFileName(), -1, NULL);
    sqlite3_bind_text(stmt, ++colIndex, pKat->getZipURL(), -1, NULL);
    sqlite3_bind_text(stmt, ++colIndex, pKat->getToken(), -1, NULL);
    sqlite3_bind_int(stmt, ++colIndex, pKat->getID());
        
    if (sqlite3_step(stmt) != SQLITE_DONE)
          CCLog("ERROR UPDATING KATEGORIA");
    
    sqlite3_finalize(stmt);

}

CCArray* SQLHelper::queryKategoriak(int idKategoria)
{
    sqlite3_stmt *stmt;
    
    if (idKategoria == -1) {
        sqlite3_prepare_v2(m_pDB, "SELECT * FROM Kategoria", -1, &stmt, NULL);
    } else {
        sqlite3_prepare_v2(m_pDB, "SELECT * FROM Kategoria WHERE id = ?", -1, &stmt, NULL);
        sqlite3_bind_int(stmt, 1, idKategoria);
    }
    
    CCArray *pKategoriak = CCArray::create();
    
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        
        int colIndex = 0;
        
        KategoriaModel *pKat = new KategoriaModel();
   
        pKat->setID(sqlite3_column_int(stmt, colIndex++));
        pKat->setIzena(strdup((char*)(sqlite3_column_text(stmt, colIndex++))));
        pKat->setDeskribapena(strdup((char*)(sqlite3_column_text(stmt, colIndex++))));
        pKat->setFileName(strdup((char*)(sqlite3_column_text(stmt, colIndex++))));
        pKat->setZipURL(strdup((char*)(sqlite3_column_text(stmt, colIndex++))));
        pKat->setToken(strdup((char*)(sqlite3_column_text(stmt, colIndex++))));
          
        pKategoriak->addObject(pKat);
        
        pKat->release();
    }
    
    sqlite3_finalize(stmt);

    return pKategoriak;
}
