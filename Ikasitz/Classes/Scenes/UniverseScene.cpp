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
#include "../AppDelegate.h"
#include "../Helpers/SpriteButton.h"
#include "../Helpers/VisibleRect.h"
#include "../Layers/SkyLayer.h"
#include "../Models/KategoriaModel.h"

UniverseScene::~UniverseScene()
{

}

UniverseScene::UniverseScene()
{
    m_pAPI = IkasitzAPI::sharedInstance();
    m_pUniverses = m_pAPI->getUniverses();
    imageScale = m_pAPI->getScale();
    
    m_pBolasSprites = new Vector<Sprite *>();
}

bool UniverseScene::init()
{
    SkyLayer *pSkyLayer = SkyLayer::create();
    addChild(pSkyLayer);
    
    
    createUniversesLayer();
    
    Texture2D *pTexture = Director::getInstance()->getTextureCache()->addImage("back.png");
    SpriteButton *pBackButton = SpriteButton::create(pTexture, CC_CALLBACK_1(UniverseScene::backClicked, this));
    Point bottomLeft = VisibleRect::leftBottom();
    pBackButton->setPosition(Vec2(bottomLeft.x + 70, bottomLeft.y + 60));
    addChild(pBackButton);
    
    return true;
}

void UniverseScene::createUniversesLayer()
{
    auto pLayers = new Vector<Layer *>();
    
    int numUniverses = (int) m_pUniverses->size();
    
    int numberPages = numUniverses / 3;
    
    if (numUniverses % 3 > 0) {
        numberPages++;
    }
    
    
    Point texturePosition[] = {Vec2(110, 150), Vec2(350, 150), Vec2(590, 150)};
    Point labelPosition[] = {Vec2(110, 20), Vec2(350, 20), Vec2(590, 20)};
    
    int cont = 0;
 
    
    for (int i = 1; i <= numberPages; i++) {
        
        Layer *pContainer = Layer::create();
        Layer *pContentLayer = Layer::create();
        
        Size contentSize = Size(700, 257);
        Point contentPos = Vec2(VisibleRect::center().x - contentSize.width/2, VisibleRect::center().y - contentSize.height/2);
        pContentLayer->setContentSize(contentSize);
        pContentLayer->setPosition(contentPos);
        
        int numberRows = numUniverses - (3 * (i-1));
        
        if (numberRows > 3)
            numberRows = 3;
        
        
        for (int j = 1; j <= numberRows; j++) {
            
            KategoriaModel *pKat = m_pUniverses->at(cont);
            
            std::string imageName = FileUtils::getInstance()->getWritablePath().append(pKat->getFileName()).append(".png");
            
            Texture2D *pTexture = Director::getInstance()->getTextureCache()->addImage(imageName.c_str());
            SpriteButton *pUniverse = SpriteButton::create(pTexture, CC_CALLBACK_1(UniverseScene::universeClicked, this), imageScale);
            pUniverse->setPosition(texturePosition[j-1]);
            pUniverse->setTag(cont);
            pContentLayer->addChild(pUniverse);
            
            auto pLabel = Label::createWithTTF(pKat->getIzena(), fontList[0], 22.0);
            pLabel->setHorizontalAlignment(TextHAlignment::CENTER);
            pLabel->setPosition(labelPosition[j-1]);
            pLabel->setColor(Color3B(63, 62, 62));
            pLabel->setDimensions(257, 50);
            pContentLayer->addChild(pLabel);
            
            cont++;
        }
        
        pContainer->addChild(pContentLayer);
        pLayers->pushBack(pContainer);
        
    }
    
    ScrollLayer *pScrollLayer = ScrollLayer::nodeWithLayers(pLayers, 0);
    pScrollLayer->setPagesIndicatorPosition(Vec2(VisibleRect::center().x, VisibleRect::center().y - 150));
    pScrollLayer->setDelegate(this);
    pScrollLayer->moveToPage(0);
    addChild(pScrollLayer);
}

void UniverseScene::scrollLayerScrolledToPageNumber(ScrollLayer* sender, unsigned int page)
{
    if (sender->getShowPagesIndicator())
    {
        for (auto sprite: *m_pBolasSprites)
        {
            removeChild(sprite, false);
        }
        
        m_pBolasSprites->clear();
        
        int totalScreens = sender->getTotalScreens();
        int currentScreen = sender->getCurrentScreen();
        
        Point pageIndicatorPosition = sender->getPagesIndicatorPosition();
        
        // Prepare Points Array
        float n = (float)totalScreens; //< Total points count in CGFloat.
        float pY = pageIndicatorPosition.y; //< Points y-coord in parent coord sys.
        float d = 30.0f; //< Distance between points.
        
        Point* points = new Point[totalScreens];
        for (int i = 0; i < totalScreens; ++i)
        {
            float pX = pageIndicatorPosition.x + d * ((float)i - 0.5f*(n-1.0f));
            points[i] = Vec2(pX, pY);
        }
        
        for (int i = 0; i < totalScreens; i++) {
            
            Sprite *bola;
            
            if (i == currentScreen) {
                bola = Sprite::create("bola_roja.png");
            } else {
                bola = Sprite::create("bola_gris.png");
            }
           
            bola->setPosition(points[i]);
            m_pBolasSprites->pushBack(bola);
        }
        
        delete [] points;

        for (auto sprite: *m_pBolasSprites)
        {
            addChild(sprite);
        }
    }
}

void UniverseScene::universeClicked(Ref *sender)
{
    int katPosition = ((SpriteButton*)sender)->getTag();
    KategoriaModel *pkat = m_pUniverses->at(katPosition);
    m_pAPI->loadCurrentUniverse(pkat);
    
    LevelScene *scene = LevelScene::create();
    auto pDirector = Director::getInstance();
    auto tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void UniverseScene::backClicked(Ref *sender)
{
    MenuScene *scene = MenuScene::create();
    auto pDirector = Director::getInstance();
    auto tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}
