#include "HelloWorldScene.h"
#include "ArrangeScene.h"

#include "EffectManager.h"

USING_NS_CC;

#define MENUITEM_TAG_CLOSE 10
#define MENUITEM_TAG_SHADER1 1
#define MENUITEM_TAG_SHADER2 2
#define MENUITEM_TAG_SHADER3 3
#define MENUITEM_TAG_SHADER4 4

#define MENUITEM_TAG_LEFTARROW 1
#define MENUITEM_TAG_RIGHTARROW 2

#define MENUITEM_TAG_SAVEDATA 1
#define MENUITEM_TAG_LOADDATA 2
#define MENUITEM_TAG_GOTOARRANGE 3
#define MENUITEM_TAG_STOP 4

#define IMAGE_COUNT 4

const std::string ImagePath[IMAGE_COUNT]{
    "image/heart.png", "image/star.png", "image/star2.png", "image/start600.png"
};
int m_CurImageIndex;


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    initShader();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    this->m_visibleSize = visibleSize;
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    
    auto leftArrowBtn = MenuItemImage::create("res/b1.png", "res/b1.png", CC_CALLBACK_1(HelloWorld::arrowCallback, this));
    leftArrowBtn->setPosition(Vec2(visibleSize.width / 2 - 100, 40));
    leftArrowBtn->setTag(MENUITEM_TAG_LEFTARROW);
    
    
    Sprite* normalImage = Sprite::create("res/b1.png");
    normalImage->setFlippedX(true);
    auto rightArrowBtn = MenuItemSprite::create(normalImage, NULL, CC_CALLBACK_1(HelloWorld::arrowCallback, this));
    rightArrowBtn->setPosition(Vec2(visibleSize.width / 2 + 100, 40));
    rightArrowBtn->setTag(MENUITEM_TAG_RIGHTARROW);
    
    auto saveDataBtn = MenuItemFont::create("save data", CC_CALLBACK_1(HelloWorld::saveAndLoadCallback, this));
    saveDataBtn->setPosition(Vec2(origin.x + visibleSize.width - saveDataBtn->getContentSize().width/2 ,
                              origin.y + visibleSize.height - saveDataBtn->getContentSize().height));
    saveDataBtn->setTag(MENUITEM_TAG_SAVEDATA);
    
    auto loadDataBtn = MenuItemFont::create("load data", CC_CALLBACK_1(HelloWorld::saveAndLoadCallback, this));
    loadDataBtn->setPosition(Vec2(origin.x + visibleSize.width - loadDataBtn->getContentSize().width/2 ,
                                  origin.y + visibleSize.height - saveDataBtn->getContentSize().height - loadDataBtn->getContentSize().height));
    loadDataBtn->setTag(MENUITEM_TAG_LOADDATA);
    
    auto arrangeBtn = MenuItemFont::create("go to arrange", CC_CALLBACK_1(HelloWorld::saveAndLoadCallback, this));
    arrangeBtn->setPosition(Vec2(origin.x + visibleSize.width - arrangeBtn->getContentSize().width/2 ,
                                  origin.y + visibleSize.height - saveDataBtn->getContentSize().height - loadDataBtn->getContentSize().height - arrangeBtn->getContentSize().height));
    arrangeBtn->setTag(MENUITEM_TAG_GOTOARRANGE);
    
    auto stopBtn = MenuItemFont::create("stop change", CC_CALLBACK_1(HelloWorld::saveAndLoadCallback, this));
    stopBtn->setPosition(Vec2(origin.x + visibleSize.width - stopBtn->getContentSize().width/2 ,
                                 origin.y + visibleSize.height - saveDataBtn->getContentSize().height - loadDataBtn->getContentSize().height - arrangeBtn->getContentSize().height - stopBtn->getContentSize().height));
    stopBtn->setTag(MENUITEM_TAG_STOP);
    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    closeItem->setTag(MENUITEM_TAG_CLOSE);

    auto shader1 = MenuItemFont::create("shader1", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    shader1->setPosition(Vec2(origin.x + visibleSize.width - shader1->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height + shader1->getContentSize().height));
    shader1->setTag(MENUITEM_TAG_SHADER1);
    
    auto shader2 = MenuItemFont::create("shader2", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    shader2->setPosition(Vec2(origin.x + visibleSize.width - shader1->getContentSize().width/2 ,
                              origin.y + closeItem->getContentSize().height + shader1->getContentSize().height + shader2->getContentSize().height));
    shader2->setTag(MENUITEM_TAG_SHADER2);
    
    auto shader3 = MenuItemFont::create("shader3", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    shader3->setPosition(Vec2(origin.x + visibleSize.width - shader3->getContentSize().width/2 ,
                              origin.y + closeItem->getContentSize().height + shader1->getContentSize().height + shader2->getContentSize().height+ shader3->getContentSize().height));
    shader3->setTag(MENUITEM_TAG_SHADER3);
    
    auto shader4 = MenuItemFont::create("shader4", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    shader4->setPosition(Vec2(origin.x + visibleSize.width - shader4->getContentSize().width/2 ,
                              origin.y + closeItem->getContentSize().height + shader1->getContentSize().height + shader2->getContentSize().height+ shader3->getContentSize().height + shader4->getContentSize().height));
    shader4->setTag(MENUITEM_TAG_SHADER4);
    
    
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(leftArrowBtn, rightArrowBtn, saveDataBtn, loadDataBtn, arrangeBtn, stopBtn, closeItem, shader1, shader2,shader3,shader4, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    this->m_shaderType  = 1;
    this->m_basePixel   = 8.0;
    this->m_curChangeDataCount = 0;
    
    m_CurImageIndex = 0;
    
    auto sprite = Sprite::create();
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(m_visibleSize.width / 2, m_visibleSize.height / 2));
//    auto sprite = Label::createWithSystemFont("你好", "Arial", 18);
    m_sprite = sprite;
    
    this->reloadSprite(m_CurImageIndex);
    
    
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch* touch, Event* event)->bool{
        return true;
    };
    listener->onTouchMoved = [=](Touch* touch, Event* event){
        if(m_shaderType == 1){
            this->runStrategy1(touch->getLocation());
        }else if(m_shaderType == 2){
            
        }
    };
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    Device::setAccelerometerEnabled(true);
    EventListenerAcceleration* listener1 = EventListenerAcceleration::create([=](Acceleration * acc, Event * event){
//        if(fabs(acc->x) > 0.9 || fabs(acc->y) > 0.9){
//            this->_scheduler->scheduleUpdate(this, 0, false);
//        }
//        if(abs(acc->x) > 0.02 || abs(acc->y) > 0.02){
            printf("%f, %f \n", acc->x, acc->y);
            EffectManager::getInstance()->applyChangeToSprite(m_sprite, Vec2(acc->x, acc->y));
//        }
    });
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
    
    return true;
}

void HelloWorld::initShader(){
    GLProgram* program1 = GLProgram::createWithFilenames("shader/demo1.vert", "shader/demo1.frag");
    GLProgramCache::getInstance()->addGLProgram(program1, "shader1");
    
    GLProgram* program2 = GLProgram::createWithFilenames("shader/demo1.vert", "shader/demo2.frag");
    GLProgramCache::getInstance()->addGLProgram(program2, "shader2");
    
    GLProgram* program3 = GLProgram::createWithFilenames("shader/demo1.vert", "shader/demo3.frag");
    GLProgramCache::getInstance()->addGLProgram(program3, "shader3");
    
    GLProgram* program4 = GLProgram::createWithFilenames("shader/demo1.vert", "shader/demo4.frag");
    GLProgramCache::getInstance()->addGLProgram(program4, "shader4");
    
    m_shader2Data.clipY = new float[16];
    m_shader2Data.clipDistance = new float[16];
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Node* node = static_cast<Node*>(pSender);
    switch(node->getTag()){
        case MENUITEM_TAG_CLOSE:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
            MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
            return;
#endif
            
            Director::getInstance()->end();
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
#endif
            break;
            
        default:
            this->changeShader(node->getTag());
            break;
    }
}

void HelloWorld::arrowCallback(cocos2d::Ref* pSender){
    Node* node = static_cast<Node*>(pSender);
    switch (node->getTag()) {
        case MENUITEM_TAG_LEFTARROW:
            m_CurImageIndex --;
            if(m_CurImageIndex < 0)
                m_CurImageIndex = IMAGE_COUNT;
            this->reloadSprite(m_CurImageIndex);
            
            break;
            
        case MENUITEM_TAG_RIGHTARROW:
            m_CurImageIndex ++;
            m_CurImageIndex %= IMAGE_COUNT;
            this->reloadSprite(m_CurImageIndex);
            
            break;
            
        default:
            break;
    }
}

void HelloWorld::saveAndLoadCallback(cocos2d::Ref* pSender){
    Node* node = static_cast<Node*>(pSender);
    switch (node->getTag()) {
        case MENUITEM_TAG_SAVEDATA:
            this->saveCurShaderData();
            break;
            
        case MENUITEM_TAG_LOADDATA:
            this->reloadCurShaderData();
            break;
            
        case MENUITEM_TAG_GOTOARRANGE:
            this->gotoArrange();
            break;
            
        case MENUITEM_TAG_STOP:
            this->_scheduler->unscheduleUpdate(this);
            break;
            
        default:
            break;
    }
}

void HelloWorld::update(float dt){
    m_totalTime += dt;
    
    if(m_totalTime > 0.1){
        this->runStrategy();
        m_totalTime = 0.0;
    }
}

void HelloWorld::runStrategy(){
    this->changeShader(m_shaderType);
}

void HelloWorld::runStrategy1(const cocos2d::Vec2& location){
    Vec2 percent(location.x / m_visibleSize.width, location.y / m_visibleSize.height);
    m_sprite->getGLProgramState()->setUniformVec2("u_coreCoord", percent);
}

void HelloWorld::onUniformCallback(cocos2d::GLProgram *program, cocos2d::Uniform *uniform){
    m_mutex.lock();
    GLint uniformLocation = uniform->location;
    
    if(uniformLocation == m_changeDataCountLocation){
        program->setUniformLocationWith1i(uniformLocation, m_curChangeDataCount);
    }else if(uniformLocation == m_changePositionLocation){
        program->setUniformLocationWith2fv(uniformLocation, (GLfloat*)this->m_changePosition, (unsigned int)m_curChangeDataCount);
    }else if(uniformLocation == m_changeSizeLocation){
        program->setUniformLocationWith1fv(uniformLocation, (GLfloat*)this->m_changeSize, (unsigned int)m_curChangeDataCount);
    }
    m_mutex.unlock();
}

void HelloWorld::changeShader(int shaderType){
    m_shaderType = shaderType;
    
    if(shaderType == 1){
        GLProgram* program = GLProgramCache::getInstance()->getGLProgram("shader1");
        m_sprite->setGLProgram(program);
        
        GLProgramState* state = m_sprite->getGLProgramState();
        state->setUniformFloat("pixelSize", m_basePixel);
        state->setUniformVec2("textureSize", Vec2(m_textureSize.width, m_textureSize.height));
        state->setUniformVec2("centerCoord", m_sprite->getPosition());
        state->setUniformFloat("edgeRadius", 0.5);
        state->setUniformVec2("u_coreCoord", Vec2(0.5, 0.5));
    }else if(shaderType == 2){
        GLProgram* program = GLProgramCache::getInstance()->getGLProgram("shader2");
        m_sprite->setGLProgram(program);
        
        GLProgramState* state = m_sprite->getGLProgramState();
        state->setUniformVec2("textureSize", Vec2(m_textureSize.width, m_textureSize.height));
        
        float u_ratio = random<float>(-0.5, 0.5);
        state->setUniformFloat("u_ratio", u_ratio);

        int u_clipLineCount = random<int>(16, 16);
        state->setUniformInt("u_clipLineCount", u_clipLineCount);

        float* u_clipY        = m_shader2Data.clipY;
        float* u_clipDistance = m_shader2Data.clipDistance;
        
        for(int i = 0; i < u_clipLineCount; i++){
            float clipY = random(-1.0, 1.0);
            float clipDistance = random(0.02, 0.07);
            
            u_clipY[i] = clipY;
            u_clipDistance[i] = clipDistance;
        }

        state->setUniformCallback("u_clipY", [=](GLProgram* program, Uniform* uniform){
            program->setUniformLocationWith1fv(uniform->location, u_clipY, u_clipLineCount);
        });
        state->setUniformCallback("u_clipDistance", [=](GLProgram* program, Uniform* uniform){
            program->setUniformLocationWith1fv(uniform->location, u_clipDistance, u_clipLineCount);
        });
        
        m_shader2Data.shaderName    = "shader2";
        m_shader2Data.textureSize   = this->m_textureSize;
        m_shader2Data.ratio         = u_ratio;
        m_shader2Data.clipLineCount = u_clipLineCount;
        
    }else if(shaderType == 3){
//        GLProgram* program = GLProgramCache::getInstance()->getGLProgram("shader3");
//        m_sprite->setGLProgram(program);
//        GLProgramState* state = m_sprite->getGLProgramState();
//        
//        state->setUniformFloat("pixelSize", m_basePixel);
//        state->setUniformVec2("textureSize", Vec2(m_textureSize.width, m_textureSize.height));
//        state->setUniformFloat("pixelRadius", 0.4);
//        state->setUniformVec2("u_coreCoord", Vec2(0.5, 0.5));
        
        EffectManager::getInstance()->applyEffectToSprite(m_sprite, EffectType::DotMoveEffect);
        
    }else if(shaderType == 4){
        GLProgram* program = GLProgramCache::getInstance()->getGLProgram("shader4");
        m_sprite->setGLProgram(program);
        GLProgramState* state = m_sprite->getGLProgramState();
        
        Texture2D* texture = Director::getInstance()->getTextureCache()->addImage("inkmark.png");
        state->setUniformTexture("u_tex2", texture);
        
        Vec2 u_offset1 = Vec2(random(-0.2, 0.2), random(-0.1, 0.1));
        Vec2 u_offset2 = Vec2(random(-0.2, 0.2), random(-0.2, 0.2));
        Vec2 u_offset3 = Vec2(random(-0.2, 0.2), random(-0.4, 0.4));
        Vec2 u_offset4 = Vec2(random(-0.2, 0.2), random(-0.7, 0.7));
        
        state->setUniformVec2("u_offset1", u_offset1);
        state->setUniformVec2("u_offset2", u_offset2);
        state->setUniformVec2("u_offset3", u_offset3);
        state->setUniformVec2("u_offset4", u_offset4);
        
        m_shader4Data.shaderName = "shader4";
        m_shader4Data.blendTextureName = "inkmark.png";
        m_shader4Data.offset1 = u_offset1;
        m_shader4Data.offset2 = u_offset2;
        m_shader4Data.offset3 = u_offset3;
        m_shader4Data.offset4 = u_offset4;
    }
}

void HelloWorld::saveCurShaderData(){
    if(m_shaderType == 1){
    
    }else if(m_shaderType == 2){
        //序列化数据
    }
}

void HelloWorld::reloadSprite(int index){
    m_sprite->setTexture(ImagePath[index]);
    m_sprite->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
    this->m_textureSize = m_sprite->getContentSize();
}

void HelloWorld::reloadCurShaderData(){
    if(m_shaderType == 1){
    
    }else if(m_shaderType == 2){
//        float percent = this->m_textureSize.width / m_shader2Data.textureSize.width;  //放大的比率
        
        std::string shaderName = m_shader2Data.shaderName;
        
        GLProgram* program = GLProgramCache::getInstance()->getGLProgram(shaderName);
        m_sprite->setGLProgram(program);
        
        GLProgramState* state = m_sprite->getGLProgramState();
        state->setUniformVec2("textureSize", Vec2(m_textureSize.width, m_textureSize.height));
        
        float u_ratio = m_shader2Data.ratio;
        state->setUniformFloat("u_ratio", u_ratio);
        
        int u_clipLineCount = m_shader2Data.clipLineCount;
        state->setUniformInt("u_clipLineCount", u_clipLineCount);
        
        float* u_clipY        = m_shader2Data.clipY;
        float* u_clipDistance = m_shader2Data.clipDistance;
        
        state->setUniformCallback("u_clipY", [=](GLProgram* program, Uniform* uniform){
            program->setUniformLocationWith1fv(uniform->location, u_clipY, u_clipLineCount);
        });
        state->setUniformCallback("u_clipDistance", [=](GLProgram* program, Uniform* uniform){
            program->setUniformLocationWith1fv(uniform->location, u_clipDistance, u_clipLineCount);
        });
    }else if(m_shaderType == 4){
        GLProgram* program = GLProgramCache::getInstance()->getGLProgram(m_shader4Data.shaderName);
        m_sprite->setGLProgram(program);
        GLProgramState* state = m_sprite->getGLProgramState();
        
        Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(m_shader4Data.blendTextureName);
        state->setUniformTexture("u_tex2", texture);
        
        state->setUniformVec2("u_offset1", m_shader4Data.offset1);
        state->setUniformVec2("u_offset2", m_shader4Data.offset2);
        state->setUniformVec2("u_offset3", m_shader4Data.offset3);
        state->setUniformVec2("u_offset4", m_shader4Data.offset4);
    }
}

void HelloWorld::gotoArrange(){
    Scene* scene = Scene::create();
    
    ArrangeLayer* layer = ArrangeLayer::create();
    
    scene->addChild(layer);
    
    Director::getInstance()->pushScene(scene);
    
    ShaderData data;
    data.shaderType = m_shaderType;
    
    if(m_shaderType == 2){
        data.shader2Data = &m_shader2Data;
    }else if(m_shaderType == 4){
        data.shader4Data = &m_shader4Data;
    }
    
    layer->setImageData(ImagePath[m_CurImageIndex].c_str(), data);
}