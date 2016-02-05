//
//  CocosUtil.cpp
//  ShaderDemo
//
//  Created by luozhipeng on 15/5/5.
//
//

#include "CocosUtil.h"

USING_NS_CC;
using namespace std;
using namespace pp;

void CocosUtil::stringToSize(const std::string& str, cocos2d::Size& size){
    string s = str;
    string::size_type index = s.find(",");
    
    if(index == string::npos)
        return;
    
    string strWidth  = s.substr(0, index);
    string strHeigth = s.substr(index + 1, s.length());
    
    size.width  = stof(strWidth);
    size.height = stof(strHeigth);
}

void CocosUtil::stringToVec2(const std::string& str, cocos2d::Vec2& vec){
    string s = str;
    string::size_type index = s.find(",");
    
    if(index == string::npos)
        return;
    
    string strX = s.substr(0, index);
    string strY = s.substr(index + 1, s.length());
    
    vec.x = stof(strX);
    vec.y = stof(strY);
}

void CocosUtil::stringToVec2Array(const std::string& str, std::vector<cocos2d::Vec2>& vecArray){
    
    vecArray.clear();
    
    string s = str;
    string separator = " ";
    string substring;
    string::size_type start = 0, index;
    
    do
    {
        index = s.find_first_of(separator, start);
        if (index != string::npos)
        {
            substring = str.substr(start, index - start);
            
            cocos2d::Vec2 subVec;
            stringToVec2(substring, subVec);
            
            vecArray.push_back(subVec);
            
            start = str.find_first_not_of(separator, index);
            
            if (start == string::npos) return;
        }
    }while(index != string::npos);
    
    //the last token
    substring = str.substr(start);
    cocos2d::Vec2 subVec;
    stringToVec2(substring, subVec);
    
    vecArray.push_back(subVec);
}

void CocosUtil::stringToIntArray(const std::string& str, std::vector<int>& intArray){
    intArray.clear();
    
    string s = str;
    string separator = " ";
    string substring;
    string::size_type start = 0, index;
    
    do
    {
        index = s.find_first_of(separator, start);
        if (index != string::npos)
        {
            substring = str.substr(start, index - start);
            
            intArray.push_back(stoi(substring));
            
            start = str.find_first_not_of(separator, index);
            
            if (start == string::npos) return;
        }
    }while(index != string::npos);
    
    //the last token
    substring = str.substr(start);
    intArray.push_back(stoi(substring));
}

cocos2d::Texture2D* CocosUtil::resizeTexture2D(cocos2d::Texture2D* oldTexture, const cocos2d::Size& newSize, bool isFlipped/* = false*/){
    
//    const Size& s = oldTexture->getContentSizeInPixels();
    
    // to get the image size to save
    //        if the saving image domain exceeds the buffer texture domain,
    //        it should be cut
    int savedBufferWidth = (int)newSize.width;
    int savedBufferHeight = (int)newSize.height;
    
    GLubyte *buffer = nullptr;
    GLubyte *tempData = nullptr;
    auto dataLen = savedBufferWidth * savedBufferHeight * 4;
    Texture2D *texture = new (std::nothrow)Texture2D();
    Image *image = new (std::nothrow)Image();
    GLint oldFBO;
    GLuint FBO;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);
    
    Director *director = Director::getInstance();
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    director->setProjection(director->getProjection());
    
    // Calculate the adjustment ratios based on the old and new projections
    Size size = director->getWinSizeInPixels();
    float widthRatio = size.width / newSize.width;
    float heightRatio = size.height / newSize.height;
    
    Mat4 orthoMatrix;
    Mat4::createOrthographicOffCenter((float)-1.0 / widthRatio, (float)1.0 / widthRatio, (float)-1.0 / heightRatio, (float)1.0 / heightRatio, -1, 1, &orthoMatrix);
    director->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, orthoMatrix);

    do
    {
        CC_BREAK_IF(! (buffer = new (std::nothrow) GLubyte[dataLen]));
        
        if(! (tempData = new (std::nothrow) GLubyte[dataLen]))
        {
            delete[] buffer;
            buffer = nullptr;
            break;
        }
        memset(tempData, 0, dataLen);
        texture->initWithData(tempData, dataLen, Texture2D::PixelFormat::RGBA8888, savedBufferWidth, savedBufferHeight, newSize);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, savedBufferWidth, savedBufferHeight);
        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getName(), 0);
        
        CHECK_GL_ERROR_DEBUG();
        
        oldTexture->setAntiAliasTexParameters();
        oldTexture->drawInRect(Rect(0, 0, savedBufferWidth, savedBufferHeight));
        
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(0, 0, savedBufferWidth, savedBufferHeight, GL_RGBA,GL_UNSIGNED_BYTE, tempData);
        
        
        if ( isFlipped ) // -- flip is only required when saving image to file
        {
            // to get the actual texture data
            // #640 the image read from rendertexture is dirty
            for (int i = 0; i < savedBufferHeight; ++i)
            {
                memcpy(&buffer[i * savedBufferWidth * 4],
                       &tempData[(savedBufferHeight - i - 1) * savedBufferWidth * 4],
                       savedBufferWidth * 4);
            }
            
            image->initWithRawData(buffer, dataLen, savedBufferWidth, savedBufferHeight, 8, oldTexture->hasPremultipliedAlpha());
        }
        else
        {
            image->initWithRawData(tempData, dataLen, savedBufferWidth, savedBufferHeight, 8, oldTexture->hasPremultipliedAlpha());
        }
        
        texture->releaseGLTexture();
        texture->initWithImage(image);
        
        CC_SAFE_DELETE(image);
        
    } while (0);
    
    glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
    
    CC_SAFE_DELETE_ARRAY(buffer);
    CC_SAFE_DELETE_ARRAY(tempData);
    
    director->setViewport();
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    
    return texture;
}