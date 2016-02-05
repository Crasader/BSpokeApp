#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__


#define RANDOM_MAX_SIZE 128

#include "cocos2d.h"

struct Shader2Data{
    std::string     shaderName;
    cocos2d::Size   textureSize;
    float ratio;
    int clipLineCount;
    float* clipY;
    float* clipDistance;
};

struct Shader4Data{
    std::string shaderName;
    std::string blendTextureName;
    cocos2d::Vec2 offset1;
    cocos2d::Vec2 offset2;
    cocos2d::Vec2 offset3;
    cocos2d::Vec2 offset4;
};

struct ShaderData{
    int shaderType;
    
    union{
        Shader2Data* shader2Data;
        Shader4Data* shader4Data;
    };
};

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    void initShader();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void arrowCallback(cocos2d::Ref* pSender);
    void saveAndLoadCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
public:
    void update(float dt);
    void runStrategy();
    void runStrategy1(const cocos2d::Vec2& location);
    
public:
    void onUniformCallback(cocos2d::GLProgram* program, cocos2d::Uniform* uniform);
    
private:
    void changeShader(int shaderType);
    void saveCurShaderData();
    
private:
    void reloadSprite(int index);
    void reloadCurShaderData();
    
private:
    void gotoArrange();
    
private:
    cocos2d::Sprite* m_sprite;
    
private:
    GLint m_changeDataCountLocation;
    GLint m_changeSizeLocation;
    GLint m_changePositionLocation;
    
private:
    int m_shaderType;
    
private:
    cocos2d::Size m_visibleSize;
    cocos2d::Size m_textureSize;
    float m_basePixel;
    
    int m_curChangeDataCount;
    float m_changeSize[RANDOM_MAX_SIZE];
    cocos2d::Vec2 m_changePosition[RANDOM_MAX_SIZE];
    
    float m_totalTime;
    bool** m_pixelMask;
    
    std::mutex m_mutex;
    
    Shader2Data m_shader2Data;
    Shader4Data m_shader4Data;
};

#endif // __HELLOWORLD_SCENE_H__
