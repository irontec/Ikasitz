//
//  UniverseScene.cpp
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 22/01/13.
//
//

#include "UniverseScene.h"
#include "MenuScene.h"
#include "LevelScene.h"
#include "AppDelegate.h"
#include "SpriteButton.h"
#include "VisibleRect.h"
#include "SkyLayer.h"
#include "KategoriaModel.h"

UniverseScene::~UniverseScene()
{
    CC_SAFE_RELEASE(m_pBolasSprites);
    CC_SAFE_RELEASE(m_pUniverses);
}

UniverseScene::UniverseScene()
{
    m_pAPI = IkasitzAPI::sharedInstance();
    m_pUniverses = m_pAPI->getUniverses();
    m_pUniverses->retain();
    imageScale = m_pAPI->getScale();
    
    m_pBolasSprites = CCArray::create();
    m_pBolasSprites->retain();
}

bool UniverseScene::init()
{
    SkyLayer *pSkyLayer = SkyLayer::create();
    addChild(pSkyLayer);
    
    createUniversesLayer();
    
    CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage("back.png");
    SpriteButton *pBackButton = SpriteButton::create(pTexture, this, callfuncO_selector(UniverseScene::backClicked));
    CCPoint bottomLeft = VisibleRect::leftBottom();
    pBackButton->setPosition(ccp(bottomLeft.x + 70, bottomLeft.y + 60));
    addChild(pBackButton);
    
    return true;
}

void UniverseScene::createUniversesLayer()
{
    CCArray *pLayers = CCArray::create();
    
    int numUniverses = m_pUniverses->count();
    
    int numberPages = numUniverses / 3;
    
    if (numUniverses % 3 > 0) {
        numberPages++;
    }
    
    
    CCPoint texturePosition[] = {ccp(110, 150), ccp(350, 150), ccp(590, 150)};
    CCPoint labelPosition[] = {ccp(110, 20), ccp(350, 20), ccp(590, 20)};
    
    int cont = 0;
 
    
    for (int i = 1; i <= numberPages; i++) {
        
        CCLayer *pContainer = CCLayer::create();
        CCLayer *pContentLayer = CCLayer::create();
        
        CCSize contentSize = CCSizeMake(700, 257);
        CCPoint contentPos = ccp(VisibleRect::center().x - contentSize.width/2, VisibleRect::center().y - contentSize.height/2);
        pContentLayer->setContentSize(contentSize);
        pContentLayer->setPosition(contentPos);
        
        int numberRows = numUniverses - (3 * (i-1));
        
        if (numberRows > 3)
            numberRows = 3;
        
        
        for (int j = 1; j <= numberRows; j++) {
            
            KategoriaModel *pKat = (KategoriaModel*) m_pUniverses->objectAtIndex(cont);
            
            std::string imageName = CCFileUtils::sharedFileUtils()->getWriteablePath().append(pKat->getFileName()).append(".png");
            
            CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(imageName.c_str());
            SpriteButton *pUniverse = SpriteButton::create(pTexture ,this, callfuncO_selector(UniverseScene::universeClicked), imageScale);
            pUniverse->setPosition(texturePosition[j-1]);
            pUniverse->setTag(cont);
            pContentLayer->addChild(pUniverse);
            
            CCLabelTTF *pLabel = CCLabelTTF::create(pKat->getIzena(), fontList[0], 22.0);
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
    pScrollLayer->setPagesIndicatorPosition(ccp(VisibleRect::center().x, VisibleRect::center().y - 150));
    pScrollLayer->setDelegate(this);
    pScrollLayer->moveToPage(0);
    addChild(pScrollLayer);
}

void UniverseScene::scrollLayerScrolledToPageNumber(CCScrollLayer* sender, unsigned int page)
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

void UniverseScene::universeClicked(CCObject *sender)
{
    int katPosition = ((SpriteButton*)sender)->getTag();
    KategoriaModel *pkat = (KategoriaModel*) m_pUniverses->objectAtIndex(katPosition);
    m_pAPI->loadCurrentUniverse(pkat);
    
    LevelScene *scene = LevelScene::create();
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void UniverseScene::backClicked(CCObject *sender)
{
    MenuScene *scene = MenuScene::create();
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}
