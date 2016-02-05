//
//  SceneManager.h
//  BSpokeApp
//
//  Created by luozhipeng on 15/5/9.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//  Cocos Scene 管理类 （功能：包括Scene创建和销毁， Scene之间的通信）

#ifndef __BSpokeApp__SceneManager__
#define __BSpokeApp__SceneManager__

#include "cocos2d.h"
#include "EffectScene.h"
#include "ArrangeScene.h"
#include "LookbookScene.h"
#include "LeggingPreviewScene.h"

class SceneManager{

public:
    static SceneManager* getInstance();
    
public:
    void enterEffectScene();
    void enterArrangeScene();
    void enterLookbookScene();
    void enterLeggingPreviewScene();
    
    void leaveEffectScene();
    void leaveArrangeScene();
    void leaveLookbookScene();
    void leaveLeggingPreviewScene();
    
public:
    EffectScene* getEffectScene();
    ArrangeScene* getArrangeScene();
    LookbookScene* getLookbookScene();
    LeggingPreviewScene* getLeggingPreviewScene();
    
public:
    void destroyEffectScene();
    void destroyArrangeScene();
    void destroyLookbookScene();
    void destroyLeggingPreviewScene();
    
public:
    void setEffectTextureFromArrange();
    void setEffectTextureFromLookbook();
    void saveEffectTexture(bool isToArrange);
    void onSaveEffectTextureToArrangeCallback(cocos2d::Texture2D* texture);
    void onSaveEffectTextureToLookbookCallback(cocos2d::Texture2D* texture);
    void setDelayDestroyEffectScene();
    
protected:
    SceneManager();
    virtual ~SceneManager();
    
private:
    EffectScene* m_effectScene;
    ArrangeScene* m_arrangeScene;
    LookbookScene* m_lookbookScene;
    LeggingPreviewScene* m_leggingPreviewScene;
    
private:
    bool m_isDelayDestroyEffectScene;
};

#endif /* defined(__BSpokeApp__SceneManager__) */
