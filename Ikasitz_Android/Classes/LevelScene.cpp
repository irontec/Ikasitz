//
//  LevelScene.cpp
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 23/01/13.
//
//

#include "LevelScene.h"
#include "AppDelegate.h"
#include "SkyLayer.h"
#include "SpriteButton.h"
#include "UniverseScene.h"
#include "LevelModel.h"
#include "VisibleRect.h"
#include "PlayScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

LevelScene::~LevelScene()
{
    CC_SAFE_RELEASE(m_pBolasSprites);
    CC_SAFE_RELEASE(m_pKategoria);
}

LevelScene::LevelScene()
{
    m_pAPI = IkasitzAPI::sharedInstance();
    imageScale = m_pAPI->getScale();
    
    m_pBolasSprites = CCArray::create();
    m_pBolasSprites->retain();
    
    m_pKategoria = m_pAPI->getSelectedKategoria();
    m_pKategoria->retain();
}

bool LevelScene::init()
{
    SkyLayer *pSkyLayer = SkyLayer::create();
    addChild(pSkyLayer);
    
    createLevelsLayer();
    
    CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage("back.png");
    SpriteButton *pBackButton = SpriteButton::create(pTexture, this, callfuncO_selector(LevelScene::backClicked));
    CCPoint bottomLeft = VisibleRect::leftBottom();
    pBackButton->setPosition(ccp(bottomLeft.x + 70, bottomLeft.y + 60));
    addChild(pBackButton);
    
    //Read preferences
    CCUserDefault *pUserDefaults = CCUserDefault::sharedUserDefault();
    
    float volume = pUserDefaults->getFloatForKey("volume");
    bool sound = pUserDefaults->getBoolForKey("sound");
    
    if (sound) {
        bool isPlaying = SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying();
        
        if (!isPlaying)
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sounds/music.wav", true);
        
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(volume);
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(volume);
    }
    
    return true;
}

void LevelScene::createLevelsLayer()
{
    CCArray *pLayers = CCArray::create();

    CCArray *pLevels = m_pKategoria->getPantailak();
    
    int numLevels = pLevels->count();
    
    int numberPages = numLevels / 4;
    
    if (numLevels % 4 > 0) {
        numberPages++;
    }
    
    int cont = 0;
    
    for (int i = 1; i <= numberPages; i++) {
        
        CCLayer *pContainer = CCLayer::create();
        CCLayer *pContentLayer = CCLayer::create();
        
        CCSize contentSize = CCSizeMake(500, 490);
        CCPoint contentPos = ccp(VisibleRect::center().x - contentSize.width/2, VisibleRect::center().y - contentSize.height/2);
        pContentLayer->setContentSize(contentSize);
        pContentLayer->setPosition(contentPos);
        
        //CCPoint borderPosition[] = {ccp(125, 365), ccp(375, 365), ccp(125, 145), ccp(375, 145)};
        CCPoint texturePosition[] = {ccp(122, 369), ccp(372, 369), ccp(122, 149), ccp(372, 149)};
        CCPoint labelPosition[] = {ccp(125, 250), ccp(375, 250), ccp(125, 30), ccp(375, 30)};
        
        int numberRows = numLevels - (4 * (i-1));
        
        if (numberRows > 4)
            numberRows = 4;
        
        for(int j = 1; j <= numberRows; j++) {
           
            LevelModel *pLevel = (LevelModel*) pLevels->objectAtIndex(cont);
            
            std::string imageName = CCFileUtils::sharedFileUtils()->getWriteablePath().append(pLevel->getList());
            
            /*CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage("borde.png");
            CCSprite *pImageBorder = CCSprite::createWithTexture(pTexture);
            pImageBorder->setPosition(borderPosition[j-1]);
            pContentLayer->addChild(pImageBorder);
            
            pTexture = CCTextureCache::sharedTextureCache()->addImage(imageName.c_str());
            SpriteButton *pLevelButton = SpriteButton::create(pTexture ,this, callfuncO_selector(LevelScene::levelClicked), imageScale);
            pLevelButton->setPosition(texturePosition[j-1]);
            pLevelButton->setTag(cont);
            pContentLayer->addChild(pLevelButton);*/
            
            CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage("borde.png");
            SpriteButton *pLevelButton = SpriteButton::create(pTexture ,this, callfuncO_selector(LevelScene::levelClicked));
            pLevelButton->setPosition(texturePosition[j-1]);
            pLevelButton->setTag(cont);

            pTexture = CCTextureCache::sharedTextureCache()->addImage(imageName.c_str());
            CCSprite *pImageBorder = CCSprite::createWithTexture(pTexture);
            //pImageBorder->setAnchorPoint(ccp(0, 0));
            pImageBorder->setScale(imageScale);
            pImageBorder->setPosition(ccp(87, 95));
            pLevelButton->addChild(pImageBorder);
            
            pContentLayer->addChild(pLevelButton);
     
            
            CCLabelTTF *pLabel = CCLabelTTF::create(pLevel->getIzena(), fontList[0], 22.0);
            pLabel->setPosition(labelPosition[j-1]);
            pLabel->setColor(ccc3(63, 62, 62));
            pLabel->setDimensions(CCSizeMake(257, 50));
            pContentLayer->addChild(pLabel);
            
            cont++;
        }
        
        pContainer->addChild(pContentLayer);
        pLayers->addObject(pContainer);
    }
    
    CCScrollLayer *pScrollLayer = CCScrollLayer::nodeWithLayers(pLayers, 0);
    pScrollLayer->setPagesIndicatorPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y + 70));
    pScrollLayer->setDelegate(this);
    pScrollLayer->moveToPage(0);
    addChild(pScrollLayer);
}

void LevelScene::scrollLayerScrolledToPageNumber(CCScrollLayer* sender, unsigned int page)
{
    if (sender->getShowPagesIndicator())
    {
        CCObject *item;
        CCARRAY_FOREACH(m_pBolasSprites, item)
        {
            CCSprite *sprite = (CCSprite*)item;
            removeChild(sprite, false);
        }
        
        m_pBolasSprites->removeAllObjects();
        
        int totalScreens = sender->getTotalScreens();
        int currentScreen = sender->getCurrentScreen();
        
        CCPoint pageIndicatorPosition = sender->getPagesIndicatorPosition();
        
        // Prepare Points Array
        float n = (float)totalScreens; //< Total points count in CGFloat.
        float pY = pageIndicatorPosition.y; //< Points y-coord in parent coord sys.
        float d = 30.0f; //< Distance between points.
        
        CCPoint* points = new CCPoint[totalScreens];
        for (int i = 0; i < totalScreens; ++i)
        {
            float pX = pageIndicatorPosition.x + d * ((float)i - 0.5f*(n-1.0f));
            points[i] = ccp(pX, pY);
        }
        
        for (int i = 0; i < totalScreens; i++) {
            
            CCSprite *bola;
            
            if (i == currentScreen) {
                bola = CCSprite::create("bola_roja.png");
            } else {
                bola = CCSprite::create("bola_gris.png");
            }
            
            bola->setPosition(points[i]);
            m_pBolasSprites->addObject(bola);
        }
        
        delete [] points;
        
        CCARRAY_FOREACH(m_pBolasSprites, item)
        {
            CCSprite *sprite = (CCSprite*)item;
            addChild(sprite);
        }
    }
}

void LevelScene::levelClicked(CCObject *sender)
{
    int levelPosition = ((SpriteButton*)sender)->getTag();
    m_pAPI->currentLevel = levelPosition;
    LevelModel *pLevel = (LevelModel*) m_pKategoria->getPantailak()->objectAtIndex(levelPosition);
    PlayScene *scene = PlayScene::create(pLevel);
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void LevelScene::backClicked(CCObject *sender)
{
    UniverseScene *scene = UniverseScene::create();
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}