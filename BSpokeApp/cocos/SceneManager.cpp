//
//  SceneManager.cpp
//  BSpokeApp
//
//  Created by luozhipeng on 15/5/9.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#include "SceneManager.h"

static SceneManager* s_sceneManager = NULL;


SceneManager* SceneManager::getInstance(){
    if(!s_sceneManager){
        s_sceneManager = new SceneManager();
    }
    
    return s_sceneManager;
}

SceneManager::SceneManager():
m_effectScene(NULL),
m_arrangeScene(NULL),
m_lookbookScene(NULL),
m_leggingPreviewScene(NULL)
{
    m_isDelayDestroyEffectScene = false;
}

SceneManager::~SceneManager(){
    CC_SAFE_RELEASE(m_effectScene);
    CC_SAFE_RELEASE(m_arrangeScene);
    CC_SAFE_RELEASE(m_lookbookScene);
    CC_SAFE_RELEASE(m_leggingPreviewScene);
}

void SceneManager::enterEffectScene(){
    CCLOG("enter effect");
    
    cocos2d::Scene* scene = cocos2d::Scene::create();
    m_effectScene = this->getEffectScene();
    
    scene->addChild(m_effectScene);
    
    cocos2d::Director::getInstance()->pushScene(scene);
    cocos2d::Director::getInstance()->drawScene();
    
    
//    if(m_arrangeScene && m_arrangeScene->isResetPattern()){
//        m_effectScene->setPattern(m_arrangeScene->getPatternTexture());
//        m_effectScene->setColor(m_arrangeScene->getPatternColor());
//    }
}

void SceneManager::enterArrangeScene(){
    CCLOG("enter arrange");
    cocos2d::Scene* scene = cocos2d::Scene::create();
    m_arrangeScene = this->getArrangeScene();
    
    scene->addChild(m_arrangeScene);
    
    cocos2d::Director::getInstance()->pushScene(scene);
    cocos2d::Director::getInstance()->drawScene();
}

void SceneManager::enterLookbookScene(){
    cocos2d::Scene* scene = cocos2d::Scene::create();
    m_lookbookScene = this->getLookbookScene();
    
    scene->addChild(m_lookbookScene);
    
    cocos2d::Director::getInstance()->pushScene(scene);
//    cocos2d::Director::getInstance()->drawScene();
}

void SceneManager::enterLeggingPreviewScene(){
    cocos2d::Scene* scene = cocos2d::Scene::create();
    m_leggingPreviewScene = this->getLeggingPreviewScene();
    
    scene->addChild(m_leggingPreviewScene);
    
    cocos2d::Director::getInstance()->pushScene(scene);
}

void SceneManager::leaveEffectScene(){
    CCLOG("leave effect");
    cocos2d::Director::getInstance()->popScene();
    cocos2d::Director::getInstance()->drawScene();
}

void SceneManager::leaveArrangeScene(){
    CCLOG("leave arrange");
    cocos2d::Director::getInstance()->popScene();
    cocos2d::Director::getInstance()->drawScene();
}

void SceneManager::leaveLookbookScene(){
    cocos2d::Director::getInstance()->popScene();
	cocos2d::Director::getInstance()->drawScene();
}

void SceneManager::leaveLeggingPreviewScene(){
    cocos2d::Director::getInstance()->popScene();
	cocos2d::Director::getInstance()->drawScene();
}

EffectScene* SceneManager::getEffectScene(){
    if(!m_effectScene){
        m_effectScene = EffectScene::create();
        m_effectScene->retain();
    }
    
    return m_effectScene;
}

ArrangeScene* SceneManager::getArrangeScene(){
    if(!m_arrangeScene){
        m_arrangeScene = ArrangeScene::create();
        m_arrangeScene->retain();
    }
    
    return m_arrangeScene;
}

LookbookScene* SceneManager::getLookbookScene(){
    if(!m_lookbookScene){
        m_lookbookScene = LookbookScene::create();
        m_lookbookScene->retain();
    }
    
    return m_lookbookScene;
}

LeggingPreviewScene* SceneManager::getLeggingPreviewScene(){
    if(!m_leggingPreviewScene){
        CCASSERT(m_arrangeScene != NULL, "get leggingPreviewScene when arrangeScene is nil");
        
        m_leggingPreviewScene = LeggingPreviewScene::createWithData(m_arrangeScene->getLeggingType(), m_arrangeScene->getLeggingColor().c_str(), m_arrangeScene->getArrangeInfo());
        m_leggingPreviewScene->retain();
    }
    
    return m_leggingPreviewScene;
}

void SceneManager::destroyEffectScene(){
    CC_SAFE_RELEASE(m_effectScene);
    m_effectScene = NULL;
    m_isDelayDestroyEffectScene = false;
}

void SceneManager::destroyArrangeScene(){
    CC_SAFE_RELEASE(m_arrangeScene);
    m_arrangeScene = NULL;
}

void SceneManager::destroyLookbookScene(){
    CC_SAFE_RELEASE(m_lookbookScene);
    m_lookbookScene = NULL;
}

void SceneManager::destroyLeggingPreviewScene(){
    CC_SAFE_RELEASE(m_leggingPreviewScene);
    m_leggingPreviewScene = NULL;
}

void SceneManager::setEffectTextureFromArrange(){
    if(!m_effectScene || !m_arrangeScene)
        return;
    
    m_effectScene->setPattern(m_arrangeScene->getSelectedItemTexture());
}

void SceneManager::setEffectTextureFromLookbook(){
    if(!m_effectScene || !m_lookbookScene)
        return;
    
    m_effectScene->setPattern(m_lookbookScene->getFocusItemTexture());
}

void SceneManager::saveEffectTexture(bool isToArrange){
    if(!m_effectScene)
        return;
    
    if(isToArrange){
        m_effectScene->saveTexture(std::bind(&SceneManager::onSaveEffectTextureToArrangeCallback, this, std::placeholders::_1));
    }else{
        m_effectScene->saveTexture(std::bind(&SceneManager::onSaveEffectTextureToLookbookCallback, this, std::placeholders::_1));
    }
}

void SceneManager::onSaveEffectTextureToArrangeCallback(cocos2d::Texture2D* texture){
    if(m_arrangeScene){
        m_arrangeScene->setSelectedItemTexture(texture);
    }
    
    if(m_isDelayDestroyEffectScene)
        this->destroyEffectScene();
}

void SceneManager::onSaveEffectTextureToLookbookCallback(cocos2d::Texture2D* texture){
    if(m_lookbookScene){
        m_lookbookScene->applyTexture(texture);
    }
    
    if(m_isDelayDestroyEffectScene)
        this->destroyEffectScene();
}

void SceneManager::setDelayDestroyEffectScene(){
    m_isDelayDestroyEffectScene = true;
}