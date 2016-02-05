//
//  LeggingHelper.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/5/6.
//
//  leggings的帮助类   参数的定义

#ifndef __ShaderDemo__LeggingHelper__
#define __ShaderDemo__LeggingHelper__

#include <string>
#include "Legging.h"

class LeggingHelper{

public:
    static LeggingHelper* getInstance();
    void init();
    
public:
    std::string convertLeggingInfoToString(const LeggingType& type, const char* color = "", bool isColorShort = true, const LeggingOrientation& orientation = LeggingOrientation_None, const LeggingSide& side = LeggingSide_None);
    
public:
    void setCurrentLeggingType(LeggingType type){ m_curLeggingType = type; };
    LeggingType getCurrentLeggingType(){ return m_curLeggingType; };
    
private:
    LeggingType m_curLeggingType;
    
};



#endif /* defined(__ShaderDemo__LeggingHelper__) */
