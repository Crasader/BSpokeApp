//
//  PaintEffect.cpp
//  ShaderDemo
//
//  Created by luozhipeng on 15/4/11.
//
//

#include "PaintEffect.h"

#define BATCH_NODE_CAPACITY 512

PaintEffect::PaintEffect():
m_testRenderTexture(NULL){
    m_effectType = EffectType_PaintEffect;
}

PaintEffect::~PaintEffect(){
    CC_SAFE_RELEASE(m_testRenderTexture);
    m_testRenderTexture = NULL;
}

PaintEffect* PaintEffect::create(){
    PaintEffect* ret = new PaintEffect();
    if(ret && ret->init()){
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    
    return NULL;
}

bool PaintEffect::init(){
    m_shaderName = "PaintShader";
    
    GLProgram* program = GLProgramCache::getInstance()->getGLProgram(m_shaderName);
    if(!program){
        program = GLProgram::createWithFilenames("shader/base_pos_tex.vert", "shader/paint.frag");
        GLProgramCache::getInstance()->addGLProgram(program, m_shaderName);
    }
    
    return true;
}

void PaintEffect::applyToSprite(Sprite* sprite){
    
//    m_runningPaintItem.resize(BATCH_NODE_CAPACITY);
//    applyOriginEffect();
}

void PaintEffect::applyToSpriteWithParent(Sprite* sprite, Node* parent){
    Size s      = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size pixelSize = Director::getInstance()->getWinSizeInPixels();
    
    //画板位置是父亲的中点
    Vec2 parentZeroPointInWorld = parent->convertToWorldSpace(Vec2::ZERO);
    Size parentSize = parent->getContentSize();
    Vec2 texturePos = Vec2(parentSize.width / 2, parentSize.height / 2);
    
    std::string texturePath = "effect/pattern5.png";
//    std::string texturePath = "CloseSelected.png";
    std::string plistPath   = "effect/pattern5.plist";
    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(texturePath);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistPath, texture);
    
    m_renderTexture = RenderTexture::create(parentSize.width, parentSize.height);
    m_renderTexture->setPosition(texturePos);
    m_renderTexture->setAutoDraw(true);
    m_renderTexture->setKeepMatrix(true);
    
    //窗口起始点是父亲的(0,0)点的世界坐标
    m_renderTexture->setVirtualViewport(parentZeroPointInWorld, Rect(0,0,s.width,s.height), Rect(0,0,pixelSize.width,pixelSize.height));
    parent->addChild(m_renderTexture);
    
    m_paintNode = SpriteBatchNode::createWithTexture(texture, BATCH_NODE_CAPACITY);
    m_paintNode->setPosition(Vec2::ZERO);
    m_paintNode->setContentSize(parentSize);
    m_renderTexture->addChild(m_paintNode);
    
    testColorArea(sprite);
    
    Color3B color = sprite->getColor();
    this->updateColor(color);
}

void PaintEffect::cancelToSprite(Sprite* sprite){
    m_renderTexture->removeFromParent();
}

void PaintEffect::runAction(const Vec2& delta){
    int strength = delta.length();
    for(int i = 0; i < strength; i++){
        PaintItem item = generatePaintItem(delta);
        m_runningPaintItem.push_back(item);
    }
}

void PaintEffect::updateColor(const cocos2d::Color3B &color){
    m_color = Vec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, 1.f);
    m_renderTexture->getSprite()->setColor(color);
}

void PaintEffect::render(){
    if(m_renderTexture){
        m_renderTexture->visit(cocos2d::Director::getInstance()->getRenderer(), m_renderTexture->getParent()->getNodeToWorldTransform(), true);
    }
}

void PaintEffect::applyOriginEffect(){
}

void PaintEffect::update(float dt){
    if(m_runningPaintItem.size() == 0)
        return;

    for(std::vector<PaintItem>::iterator it = m_runningPaintItem.begin(); it != m_runningPaintItem.end();){
        bool isStop;
        PaintItem& item = (*it);
        
        isStop = this->applyAnimate(item, dt);
        if(isStop){
            it = m_runningPaintItem.erase(it);
        }else{
            it++;
        }
    }
}

void PaintEffect::testColorArea(Sprite* sprite){
    //sprite position relative m_renderTexture
    Vec2 spriteInWorldPos   = sprite->convertToWorldSpace(sprite->getAnchorPointInPoints());
    Vec2 spriteInTexturePos = m_renderTexture->convertToNodeSpace(spriteInWorldPos);
    m_spritePosOffsetCenter = spriteInTexturePos - m_renderTexture->getAnchorPointInPoints();
    
    Size s = Director::getInstance()->getVisibleSize();
    Size pixelSize = Director::getInstance()->getWinSizeInPixels();
    
    CC_SAFE_RELEASE(m_testRenderTexture);
    
    m_testRenderTexture = RenderTexture::create(sprite->getContentSize().width, sprite->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
    m_testRenderTexture->retain();
    m_testRenderTexture->setKeepMatrix(true);
    
    m_testRenderTexture->setVirtualViewport(Vec2(spriteInWorldPos.x - sprite->getContentSize().width / 2, spriteInWorldPos.y - sprite->getContentSize().height / 2), Rect(0,0,s.width,s.height), Rect(0,0,pixelSize.width,pixelSize.height));
    
    m_testRenderTexture->begin();
    sprite->visit(Director::getInstance()->getRenderer(), sprite->getParent()->getNodeToWorldTransform(), true);
    m_testRenderTexture->end();
    
    m_testTextureFinishCommand.init(m_testRenderTexture->getGlobalZOrder());
    m_testTextureFinishCommand.func = CC_CALLBACK_0(PaintEffect::onTestColorAreaCallback, this);
    
    Director::getInstance()->getRenderer()->addCommand(&m_testTextureFinishCommand);
}

void PaintEffect::onTestColorAreaCallback(){
    Image* image = m_testRenderTexture->newImage();
    
    int width   = image->getWidth();
    int height  = image->getHeight();
    long length = image->getDataLen();
    int bytePerPixel = image->getBitPerPixel() / 8;
    int alphaPos = bytePerPixel - 1;
    
    //线的粗细
    int offset  = 3;
    //当获得边缘点后跳过检测点数目
    int seriesCount = 6;
    
    unsigned char* data = image->getData();
    unsigned char* newData = (unsigned char*)malloc(sizeof(unsigned char) * length);
    
    memcpy(newData, data, length);
//    image->saveToFile("/Users/luozhipeng/Desktop/a/1.png", false);
    
    Vec2 edge;
    
    for(int i = 0; i < height; i++){
        
        for(int j = 0; j < width; j++){
            //检查9宫格内是否有透明像素点，没就是内部像素，而不是边界像素
            int x,y;
            bool hasAlphaPixel    = false;
            bool hasNotAlphyPixel = false;
            
            //top left
            x = j - offset;
            y = i - offset;
            if(x >= 0 && x < width && y >= 0 && y < height){
                if(newData[(y * width + x)*bytePerPixel + alphaPos] == 0){
                    hasAlphaPixel = true;
                }else{
                    hasNotAlphyPixel = true;
                }
            }
            
            //top middle
            x = j;
            y = i - offset;
            if(x >= 0 && x < width && y >= 0 && y < height){
                if(newData[(y * width + x)*bytePerPixel + alphaPos] == 0){
                    hasAlphaPixel = true;
                }else{
                    hasNotAlphyPixel = true;
                }
            }
            
            //top right
            x = j + offset;
            y = i - offset;
            if(x >= 0 && x < width && y >= 0 && y < height){
                if(newData[(y * width + x)*bytePerPixel + alphaPos] == 0){
                    hasAlphaPixel = true;
                }else{
                    hasNotAlphyPixel = true;
                }
            }
            
            //middle left
            x = j - offset;
            y = i;
            if(x >= 0 && x < width && y >= 0 && y < height){
                if(newData[(y * width + x)*bytePerPixel + alphaPos] == 0){
                    hasAlphaPixel = true;
                }else{
                    hasNotAlphyPixel = true;
                }
            }
            
            //middle middle
            x = j;
            y = i;
            if(x >= 0 && x < width && y >= 0 && y < height){
                if(newData[(y * width + x)*bytePerPixel + alphaPos] == 0){
                    hasAlphaPixel = true;
                }else{
                    hasNotAlphyPixel = true;
                }
            }
            
            //middle right
            x = j + offset;
            y = i;
            if(x >= 0 && x < width && y >= 0 && y < height){
                if(newData[(y * width + x)*bytePerPixel + alphaPos] == 0){
                    hasAlphaPixel = true;
                }else{
                    hasNotAlphyPixel = true;
                }
            }
            
            //bottom left
            x = j - offset;
            y = i + offset;
            if(x >= 0 && x < width && y >= 0 && y < height){
                if(newData[(y * width + x)*bytePerPixel + alphaPos] == 0){
                    hasAlphaPixel = true;
                }else{
                    hasNotAlphyPixel = true;
                }
            }
            
            //bottom middle
            x = j;
            y = i + offset;
            if(x >= 0 && x < width && y >= 0 && y < height){
                if(newData[(y * width + x)*bytePerPixel + alphaPos] == 0){
                    hasAlphaPixel = true;
                }else{
                    hasNotAlphyPixel = true;
                }
            }
            
            //bottom right
            x = j + offset;
            y = i + offset;
            if(x >= 0 && x < width && y >= 0 && y < height){
                if(newData[(y * width + x)*bytePerPixel + alphaPos] == 0){
                    hasAlphaPixel = true;
                }else{
                    hasNotAlphyPixel = true;
                }
            }
            
            if(hasAlphaPixel && hasNotAlphyPixel){
                //因为texture是y翻转的，所以要翻转
                edge.x = j - width / 2;
                edge.y = -(i - height / 2);
                
                m_edgeVec.push_back(edge);
                
                j += seriesCount;
            
            }else{
//                data[(i * width + j)*bytePerPixel] = 0;
//                data[(i * width + j)*bytePerPixel + 1] = 0;
//                data[(i * width + j)*bytePerPixel + 2] = 0;
//                data[(i * width + j)*bytePerPixel + 3] = 0;
            }
        }
    }
//    image->saveToFile("/Users/luozhipeng/Desktop/a/1.png", false);
//    printf(" \n \n \n");
    
    CC_SAFE_RELEASE(m_testRenderTexture);
    m_testRenderTexture = NULL;
    free(newData);
    
//    for(Vec2 pos : m_edgeVec){
//        Sprite* sprite = Sprite::create("CloseSelected.png");
//        sprite->setPosition(pos);
//        m_paintNode->addChild(sprite);
//    }
}

PaintEffect::PaintItem PaintEffect::generatePaintItem(const Vec2 dir){
    PaintEffect::PaintItem ret;
    float strength = dir.length();
    
    printf("----- %f \n", strength);
    
    ret.during   = strength * 0.1f;
    ret.originDuring = ret.during;
    ret.diretion = dir;
    ret.diretion.normalize();
    ret.perpDirection = ret.diretion.getPerp();
    ret.rotate = CC_RADIANS_TO_DEGREES(dir.getAngle(Vec2(1, 0)));
    ret.scale  = random(0.05f, 0.2f) * strength;
    
    if(ret.scale > 0.4f){
        ret.during = 0.0f;
    }
    
//    float positionOffset = random(10, 50) * strength;
//    Vec2 dirPosition = ret.diretion * positionOffset;
//    
//    float dirPerpPositionOffset = random(-150, 150);
//    Vec2 dirPerpPosition = ret.perpDirection * dirPerpPositionOffset;
//    Vec2 posititon(0,0);
    Vec2 posititon = m_edgeVec[random(0, (int)m_edgeVec.size() - 1)];
//    for(Vec2& edge : m_edgeVec){
//        
//        float angle    = edge.getAngle(dir);
//        
//        if(fabsf(angle) < 0.02f){
//            posititon = edge;
//        }
//    }
    posititon += m_spritePosOffsetCenter;
    printf("position: %f, %f \n", posititon.x, posititon.y);
    
    //根据方向设置锚点
    Vec2 anchor;
    {
        Vec2 dirNegative = dir;
        dirNegative.negate();
        
        float angle = dirNegative.getAngle();
        float k     = dirNegative.y / dirNegative.x;
        
        if(-M_PI_4 < angle && angle <= M_PI_4){
            anchor.x = 1.f;
            anchor.y = 0.5f * k + 0.5f;
        }else if(M_PI_4 < angle && angle <= M_PI_4 * 3){
            anchor.y = 1.f;
            anchor.x = 0.5f / (dirNegative.y / dirNegative.x ) + 0.5;
        }else if(angle > M_PI_4 * 3 || angle < - M_PI_4 * 3){
            anchor.x = 0.f;
            anchor.y = -0.5 * k + 0.5;
        }else{
            anchor.y = 0.f;
            anchor.x = -0.5 / k + 0.5;
        }
    };
    
    printf("anchor: %f, %f \n", anchor.x, anchor.y);
    
    int patternId = random(1, 3);
    char pattern[64];
    sprintf(pattern, "pattern5_%d.png", patternId);
    
    Sprite* sprite = Sprite::createWithSpriteFrameName(pattern);
    
//    sprite->setAnchorPoint(anchor);
    sprite->setPosition(posititon);
    sprite->setRotation(ret.rotate);
    sprite->setScale(ret.scale);
    
    m_paintNode->addChild(sprite);
    
    ret.sprite = sprite;
    
    return ret;
}

bool PaintEffect::applyAnimate(PaintItem& paintItem, float dt){
    if(paintItem.during <= 0.f)
        return true;
    
    paintItem.during = MAX(paintItem.during - dt * 2, 0);
    
    float duringPercent = paintItem.during / paintItem.originDuring;
    
//    if(int(paintItem.during * 100) % 3 != 0)
//        return true;
    
//    printf("%f, \n", paintItem.during);
    
    //垂直移动方向的偏移
//    float offset    = random(-4.f, 4.f);
//    Vec2 perpOffset = paintItem.perpDirection* offset;
    Vec2 perpOffset = Vec2::ZERO;
    
    //移动方向的偏移
    float offset = (1.f - duringPercent) * 8;
    Vec2 dirOffset  = paintItem.diretion * offset;
//    Vec2 dirOffset = Vec2::ZERO;
    
    //大小
//    float scaleOffset = random(-0.1f, 0.f);
//    printf("scale %f, \n", scale);
    float scaleOffset = 0.0f;
    
    //旋转
//    float rotateOffset = random(-5.f, 5.f);
    float rotateOffset = 0.0f;
    
    Sprite* sprite = paintItem.sprite;
    
//    sprite->setOpacity(255 * duringPercent);
    sprite->setRotation(sprite->getRotation() + rotateOffset);
    sprite->setScale(sprite->getScale() + scaleOffset);
    sprite->setPosition(sprite->getPosition() + perpOffset + dirOffset);
    
    return false;
}
