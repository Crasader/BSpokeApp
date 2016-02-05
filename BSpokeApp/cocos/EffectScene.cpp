#include "EffectScene.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "ShakeAction.h"
//#include "SceneManager.h"
#include "CocosUtil.h"
#include "util/AudioManager.h"
#include "MyComplexTouchEventListener.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace pp;

#define BOUNDVIEW_SIZE cocos2d::Size(700, 700)

/**
#define MENUITEM_TAG_CLOSE 20
#define MENUITEM_TAG_SHADER1 1
#define MENUITEM_TAG_SHADER2 2
#define MENUITEM_TAG_SHADER3 3
#define MENUITEM_TAG_SHADER4 4

#define MENUITEM_TAG_RUNACTION 9
#define MENUITEM_TAG_PATTERNORLABEL 10
#define MENUITEM_TAG_STOPACTION 11
#define MENUITEM_TAG_ARRANGE 12
#define MENUITEM_TAG_PAUSE 13

#define MENUITEM_TAG_LEFTARROW 1
#define MENUITEM_TAG_RIGHTARROW 2
*/

bool m_isUseLabel = false;
int m_soundEffectId = 0;

EffectScene::~EffectScene(){
    CC_SAFE_RELEASE(m_renderTexture);
    m_renderTexture = NULL;
    
    EffectManager::getInstance()->cancelEffectFromSprite(m_sprite);
    CCLOG("aBC");
}

EffectScene* EffectScene::create()
{
    auto layer = new EffectScene();
    
    if(layer && layer->init()){
        layer->autorelease();
        
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    
    return NULL;
}

// on "init" you need to initialize your instance
bool EffectScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    m_isSleep = true;
    m_effectType = EffectType_NONE;
    m_saveTextureCallback = NULL;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    LayerColor* colorLayer = LayerColor::create(Color4B(17, 21, 27, 255));
    this->addChild(colorLayer);
    
    m_boundNode = Node::create();
    m_boundNode->setContentSize(BOUNDVIEW_SIZE);
    m_boundNode->setAnchorPoint(Vec2(0.5, 0.5));
    m_boundNode->setPosition(origin + Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(m_boundNode);
    
    auto sprite = Sprite::create();
    sprite->setPosition(Vec2(m_boundNode->getContentSize().width / 2, m_boundNode->getContentSize().height / 2));
    m_boundNode->addChild(sprite, 0);
    
    m_sprite = sprite;
    
    
    //touch detect
    MyComplexTouchEventListener* touchListener = MyComplexTouchEventListener::createWithTarget(this);
    touchListener->onDrag = [=](const Vec2& prePosition, const Vec2& curPosition){
        Vec2 direction = curPosition - prePosition;
        if(fabs(direction.x) > 1.4f || fabs(direction.y) > 1.4f){
            if(m_shakeCount == 0){
                this->runEffect(direction);
            }
            m_shakeCount++;
            m_shakeCount %= 3;
        }
    };
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    //shake detect
    Device::setAccelerometerInterval(1.0f / 60);
    EventListenerAcceleration* listener1 = EventListenerAcceleration::create([=](Acceleration * acc, Event * event){
        if(fabs(acc->x) > 1.4f || fabs(acc->y) > 1.4f){
            printf("%f, %f  == %f \n", acc->x, acc->y, acc->timestamp);
            if(m_shakeCount == 0){
                this->runEffect(Vec2(acc->x, acc->y));
            }
            m_shakeCount++;
            m_shakeCount %= 3;
        }
    });
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
    
	/**
    //MenuItem
    Menu* menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    
    float y = 0;
    MenuItemFont *effect1Btn = MenuItemFont::create("effect1", CC_CALLBACK_1(EffectScene::menuCloseCallback, this));
    effect1Btn->setColor(Color3B(0,0,0));
    effect1Btn->setTag(MENUITEM_TAG_SHADER1);
    y += effect1Btn->getContentSize().height;
    effect1Btn->setPosition(origin + Vec2(visibleSize.width - effect1Btn->getContentSize().width, y));
    menu->addChild(effect1Btn);
    
    MenuItemFont *effect2Btn = MenuItemFont::create("effect2", CC_CALLBACK_1(EffectScene::menuCloseCallback, this));
    effect2Btn->setColor(Color3B(0,0,0));
    effect2Btn->setTag(MENUITEM_TAG_SHADER2);
    y += effect2Btn->getContentSize().height;
    effect2Btn->setPosition(origin + Vec2(visibleSize.width - effect2Btn->getContentSize().width, y));
    menu->addChild(effect2Btn);
    
    MenuItemFont *effect3Btn = MenuItemFont::create("effect3", CC_CALLBACK_1(EffectScene::menuCloseCallback, this));
    effect3Btn->setColor(Color3B(0,0,0));
    effect3Btn->setTag(MENUITEM_TAG_SHADER3);
    y += effect3Btn->getContentSize().height;
    effect3Btn->setPosition(origin + Vec2(visibleSize.width - effect3Btn->getContentSize().width, y));
    menu->addChild(effect3Btn);
    
    MenuItemFont *effect4Btn = MenuItemFont::create("effect4", CC_CALLBACK_1(EffectScene::menuCloseCallback, this));
    effect4Btn->setColor(Color3B(0,0,0));
    effect4Btn->setTag(MENUITEM_TAG_SHADER4);
    y += effect4Btn->getContentSize().height;
    effect4Btn->setPosition(origin + Vec2(visibleSize.width - effect4Btn->getContentSize().width, y));
    menu->addChild(effect4Btn);
    
    MenuItemFont *effect5Btn = MenuItemFont::create("effect5", CC_CALLBACK_1(EffectScene::menuCloseCallback, this));
    effect5Btn->setColor(Color3B(0,0,0));
    effect5Btn->setTag(MENUITEM_TAG_SHADER5);
    y += effect5Btn->getContentSize().height;
    effect5Btn->setPosition(origin + Vec2(visibleSize.width - effect5Btn->getContentSize().width, y));
    menu->addChild(effect5Btn);
    
    MenuItemFont *runActionBtn = MenuItemFont::create("run Action", CC_CALLBACK_1(EffectScene::menuCloseCallback, this));
    runActionBtn->setColor(Color3B(0,0,0));
    runActionBtn->setTag(MENUITEM_TAG_RUNACTION);
    y += runActionBtn->getContentSize().height;
    runActionBtn->setPosition(origin + Vec2(visibleSize.width - runActionBtn->getContentSize().width, y));
    menu->addChild(runActionBtn);
    
    MenuItemFont *backBtn = MenuItemFont::create("back", CC_CALLBACK_1(EffectScene::menuCloseCallback, this));
    backBtn->setColor(Color3B(0,0,0));
    backBtn->setTag(MENUITEM_TAG_CLOSE);
    y += backBtn->getContentSize().height;
    backBtn->setPosition(origin + Vec2(visibleSize.width - backBtn->getContentSize().width, y));
    menu->addChild(backBtn);
    
    //go to arrange
    MenuItemFont *arrangeBtn = MenuItemFont::create("go to arrange", CC_CALLBACK_1(EffectScene::menuCloseCallback, this));
    arrangeBtn->setColor(Color3B(0,0,0));
    arrangeBtn->setTag(MENUITEM_TAG_ARRANGE);
    y = visibleSize.height - arrangeBtn->getContentSize().height;
    arrangeBtn->setPosition(origin + Vec2(visibleSize.width - backBtn->getContentSize().width, y));
    menu->addChild(arrangeBtn);
    
    //pause
    MenuItemFont *pauseBtn = MenuItemFont::create("pause", CC_CALLBACK_1(EffectScene::menuCloseCallback, this));
    pauseBtn->setColor(Color3B(0,0,0));
    pauseBtn->setTag(MENUITEM_TAG_PAUSE);
    y = visibleSize.height - arrangeBtn->getContentSize().height - pauseBtn->getContentSize().height;
    pauseBtn->setPosition(origin + Vec2(visibleSize.width - pauseBtn->getContentSize().width, y));
    menu->addChild(pauseBtn);
	**/
    return true;
}


/**
void EffectScene::menuCloseCallback(Ref* pSender)
{
    Node* node = static_cast<Node*>(pSender);
    switch(node->getTag()){
        case MENUITEM_TAG_CLOSE:
        {
            SceneManager::getInstance()->leaveEffectScene();
//            SceneManager::getInstance()->enterLookbookScene();
            SceneManager::getInstance()->saveEffectTexture(false);
        }
            break;
            
        case MENUITEM_TAG_RUNACTION:
//            if(m_isUseLabel){
//                for(int i = 0; i < m_label->getStringLength(); i++){
//                    EffectManager::getInstance()->runEffectActionForSprite(m_label->getLetter(i), Vec2(-3.0f, 0.0f));
//                }
//            }else{
            this->runEffect(Vec2(-3.0f, 0.2f));
//            }
        
            break;
            
        case MENUITEM_TAG_STOPACTION:
            EffectManager::getInstance()->stopEffectActionForSprite(m_sprite);
            break;
            
        case MENUITEM_TAG_PATTERNORLABEL:
//            this->changeSpriteOrLabel();
            break;
            
        case MENUITEM_TAG_ARRANGE:
        {
            SceneManager::getInstance()->saveEffectTexture(true);
            SceneManager::getInstance()->enterArrangeScene();
        }
            break;
            
        case MENUITEM_TAG_PAUSE:
        {
            if(EffectManager::getInstance()->isPause())
                this->resumeEffect();
            else
                this->pauseEffect();
        }
            break;
            
        default:
        {
            this->setEffect(node->getTag());
        }
            break;
    }
}
*/

void EffectScene::setEffect(int effect){
    EffectType effectType = EffectType_NONE;
    
    if(effect == 1){
        effectType = EffectType_DotMoveEffect;
    }else if(effect == 2){
        effectType = EffectType_LineMoveEffect;
    }else if(effect == 3){
        effectType = EffectType_PaintEffect;
    }else if(effect == 4){
        effectType = EffectType_LinePaintEffect;
    }else if(effect == 5){
        effectType = EffectType_ClipEffect;
    }
    
    //    if(m_isUseLabel){
    //        for(int i = 0; i < m_label->getStringLength(); i++){
    //            EffectManager::getInstance()->applyEffectToSprite(m_label->getLetter(i), effectType);
    //        }
    //    }else{
    EffectManager::getInstance()->applyEffectToSprite(m_sprite, effectType);
    //    }
    
    m_effectType = effectType;
}

void EffectScene::setColor(cocos2d::Color3B color){
    m_sprite->setColor(color);
    EffectManager::getInstance()->updateEffectColorToSprite(m_sprite);
}

void EffectScene::setPattern(Texture2D* texture){
    
//    Size contentSize = m_sprite->getContentSize();
    
    Rect rect = Rect::ZERO;
    rect.size = texture->getContentSize();
//    rect.origin = (contentSize - rect.size) / 2;
    
    m_sprite->setTexture(texture);
//    m_sprite->setTextureRect(rect, false, contentSize);
    m_sprite->setTextureRect(rect);
    
    EffectManager::getInstance()->cancelEffectFromSprite(m_sprite);
}

//根据id设置图案
void EffectScene::setPattern(int patternId){
    char fileName[128];
    sprintf(fileName, "pattern/pattern_%d.png", patternId);
    Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(fileName);
    
    this->setPattern(texture);
}

//还原效果
void EffectScene::resetEffect(){
    EffectManager::getInstance()->resetEffectToSprite(m_sprite);
}

//暂停效果
void EffectScene::pauseEffect(){
    EffectManager::getInstance()->pauseEffect();
}

//继续效果
void EffectScene::resumeEffect(){
    EffectManager::getInstance()->resumeEffect();
}

//随机run效果
void EffectScene::randomRunEffect(){
//    m_shakeCount++;
//    m_shakeCount %= 3;
//    
//    if(m_shakeCount % 3 != 0)
//        return;
    
    Vec2 a = Vec2::ZERO;
    a.x = random<float>(5, 5.0);
    a.y = random<float>(-5.0, -5);
    CCLOG("random run effect");
    
    this->runEffect(a);
}

void EffectScene::saveTexture(const std::function<void(Texture2D* texture)>& callback){
    CCLOG("save effect begin");
    EffectManager::getInstance()->stopEffectActionForSprite(m_sprite);
    
    m_saveTextureCallback = callback;
    
    if(!m_renderTexture){
        cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
        cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
        cocos2d::Size pixelSize   = cocos2d::Director::getInstance()->getWinSizeInPixels();
        cocos2d::Vec2 boundNodeZeroToWorld = m_boundNode->convertToWorldSpace(Vec2::ZERO);
        
        m_renderTexture = RenderTexture::create(m_boundNode->getContentSize().width, m_boundNode->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
        m_renderTexture->retain();
        m_renderTexture->setKeepMatrix(true);
        
        m_renderTexture->setVirtualViewport(boundNodeZeroToWorld, Rect(0, 0, visibleSize.width, visibleSize.height), Rect(0, 0, pixelSize.width, pixelSize.height));
    }
    
    m_renderTexture->beginWithClear(0.f, 0.f, 0.f, 0.f);
    m_sprite->visit(Director::getInstance()->getRenderer(), m_sprite->getParent()->getNodeToWorldTransform(), true);
    EffectManager::getInstance()->renderSpriteEffect(m_sprite);
    m_renderTexture->end();
    //    m_testRenderTexture->saveToFile("a.png");
    
    
    m_renderTextureFinishCommand.init(m_renderTexture->getGlobalZOrder());
    m_renderTextureFinishCommand.func = CC_CALLBACK_0(EffectScene::onSaveTextureCallback, this);
    
    Director::getInstance()->getRenderer()->addCommand(&m_renderTextureFinishCommand);
}

void EffectScene::onSaveTextureCallback(){
    CCLOG("save effect end");
    
    Sprite* sprite      = m_renderTexture->getSprite();
    Texture2D* texture  = sprite->getTexture();
    
    Texture2D* newTexture = CocosUtil::resizeTexture2D(texture, texture->getContentSize());
    newTexture->autorelease();
    m_saveTextureCallback(newTexture);
}

void EffectScene::onEnter(){
    Layer::onEnter();
    
    Device::setAccelerometerEnabled(true);
    EffectManager::getInstance()->setEffectManagerDelegate(this);
    EffectManager::getInstance()->setShareEffectParent(m_boundNode);
}

void EffectScene::onExit(){
//    EffectManager::getInstance()->cancelEffectFromSprite(m_sprite);
    Device::setAccelerometerEnabled(false);
    
    EffectManager::getInstance()->setEffectManagerDelegate(NULL);
    EffectManager::getInstance()->setShareEffectParent(NULL);
    
    Layer::onExit();
}

void EffectScene::onEffectsBegan(){
    if(m_effectActionCallback)
        m_effectActionCallback(true);
}

void EffectScene::onEffectsEnded(){
    if(m_effectActionCallback)
        m_effectActionCallback(false);
}

void EffectScene::runEffect(const cocos2d::Vec2& delta){

    EffectManager::getInstance()->runEffectActionForSprite(m_sprite,delta);
    
    this->playSoundEffect();
    this->runSceneAction();
}

void EffectScene::playSoundEffect(){
    AudioManager::getInstance()->stopAudio(m_soundEffectId);
    m_soundEffectId = AudioManager::getInstance()->playAudio("sound/shaker.mp3");
}

void EffectScene::runSceneAction(){
    Scene* scene = Director::getInstance()->getRunningScene();
    scene->setPosition(Vec2::ZERO);
    scene->stopAllActions();
    scene->runAction(ShakeAction::create(20.0f, 1.f));
}