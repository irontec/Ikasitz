//
//  LevelScene.cpp
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 23/01/13.
//
//

#include "LevelScene.h"
#include "UniverseScene.h"
#include "PlayScene.h"
#include "../AppDelegate.h"
#include "../Layers/SkyLayer.h"
#include "../Helpers/SpriteButton.h"
#include "../Models/LevelModel.h"
#include "../Helpers/VisibleRect.h"

#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

LevelScene::~LevelScene()
{
    CC_SAFE_RELEASE(m_pKategoria);
}

LevelScene::LevelScene()
{
    m_pAPI = IkasitzAPI::sharedInstance();
    imageScale = m_pAPI->getScale();
    
    m_pBolasSprites = new Vector<Sprite *>();
    
    m_pKategoria = m_pAPI->getSelectedKategoria();
    m_pKategoria->retain();
}

bool LevelScene::init()
{
    SkyLayer *pSkyLayer = SkyLayer::create();
    addChild(pSkyLayer);
    
    createLevelsLayer();
    
    Texture2D *pTexture = Director::getInstance()->getTextureCache()->addImage("back.png");
    SpriteButton *pBackButton = SpriteButton::create(pTexture, CC_CALLBACK_1(LevelScene::backClicked, this));
    Point bottomLeft = VisibleRect::leftBottom();
    pBackButton->setPosition(Vec2(bottomLeft.x + 70, bottomLeft.y + 60));
    addChild(pBackButton);
    
    //Read preferences
    auto pUserDefaults = UserDefault::getInstance();
    
    float volume = pUserDefaults->getFloatForKey("volume");
    bool sound = pUserDefaults->getBoolForKey("sound");
    
    if (sound) {
        bool isPlaying = SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
        
        if (!isPlaying)
            SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/music.wav", true);
        
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
        SimpleAudioEngine::getInstance()->setEffectsVolume(volume);
    }
    
    return true;
}

void LevelScene::createLevelsLayer()
{
    auto pLayers = new Vector<Layer *>();

    auto pLevels = m_pKategoria->getPantailak();
    
    int numLevels = (int) pLevels->size();
    
    int numberPages = numLevels / 4;
    
    if (numLevels % 4 > 0) {
        numberPages++;
    }
    
    int cont = 0;
    
    for (int i = 1; i <= numberPages; i++) {
        
        Layer *pContainer = Layer::create();
        Layer *pContentLayer = Layer::create();
        
        Size contentSize = Size(500, 490);
        Point contentPos = Vec2(VisibleRect::center().x - contentSize.width/2, VisibleRect::center().y - contentSize.height/2);
        pContentLayer->setContentSize(contentSize);
        pContentLayer->setPosition(contentPos);
        
        Point texturePosition[] = {Vec2(122, 369), Vec2(372, 369), Vec2(122, 149), Vec2(372, 149)};
        Point labelPosition[] = {Vec2(125, 250), Vec2(375, 250), Vec2(125, 30), Vec2(375, 30)};
        
        int numberRows = numLevels - (4 * (i-1));
        
        if (numberRows > 4)
            numberRows = 4;
        
        for(int j = 1; j <= numberRows; j++) {
           
            LevelModel *pLevel = (LevelModel*) pLevels->at(cont);
            
            std::string imageName = FileUtils::getInstance()->getWritablePath().append(pLevel->getList());
            
            Texture2D *pTexture = Director::getInstance()->getTextureCache()->addImage("borde.png");
            SpriteButton *pLevelButton = SpriteButton::create(pTexture, CC_CALLBACK_1(LevelScene::levelClicked, this));
            pLevelButton->setPosition(texturePosition[j-1]);
            pLevelButton->setTag(cont);

            pTexture = Director::getInstance()->getTextureCache()->addImage(imageName.c_str());
            Sprite *pImageBorder = Sprite::createWithTexture(pTexture);
            //pImageBorder->setAnchorPoint(Vec2(0, 0));
            pImageBorder->setScale(imageScale);
            pImageBorder->setPosition(Vec2(87, 95));
            pLevelButton->addChild(pImageBorder);
            
            pContentLayer->addChild(pLevelButton);
     
            
            auto pLabel = Label::createWithTTF(pLevel->getIzena(), fontList[0], 22.0);
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
    pScrollLayer->setPagesIndicatorPosition(Vec2(VisibleRect::center().x, VisibleRect::bottom().y + 70));
    pScrollLayer->setDelegate(this);
    pScrollLayer->moveToPage(0);
    addChild(pScrollLayer);
}

void LevelScene::scrollLayerScrolledToPageNumber(ScrollLayer* sender, unsigned int page)
{
    if (sender->getShowPagesIndicator())
    {
        for (auto item: *m_pBolasSprites) {
            removeChild(item, false);
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
        
        for(auto item: *m_pBolasSprites) {
            addChild(item, false);
        }

    }
}

void LevelScene::levelClicked(Ref *sender)
{
    int levelPosition = ((SpriteButton*)sender)->getTag();
    m_pAPI->currentLevel = levelPosition;
    LevelModel *pLevel = (LevelModel*) m_pAPI->getSelectedKategoria()->getPantailak()->at(levelPosition);
    PlayScene *scene = PlayScene::create(pLevel);
    auto pDirector = Director::getInstance();
    auto tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void LevelScene::backClicked(Ref *sender)
{
    UniverseScene *scene = UniverseScene::create();
    auto pDirector = Director::getInstance();
    auto tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}
