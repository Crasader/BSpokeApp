//
//  LeggingHelper.cpp
//  ShaderDemo
//
//  Created by luozhipeng on 15/5/6.
//
//

#include <map>
#include "LeggingHelper.h"

using namespace std;

static LeggingHelper* s_leggingHelper = NULL;
std::map<std::string, std::string> s_colorStrMapping;


LeggingHelper* LeggingHelper::getInstance(){
    if(!s_leggingHelper){
        s_leggingHelper = new LeggingHelper();
        s_leggingHelper->init();
    }
    
    return s_leggingHelper;
}

void LeggingHelper::init(){
    s_colorStrMapping["black"] = "bk";
    s_colorStrMapping["blue"] = "b";
    s_colorStrMapping["darkblue"] = "db";
    s_colorStrMapping["darkgrey"] = "dg";
    s_colorStrMapping["darkpurple"] = "dp";
    s_colorStrMapping["darkred"] = "dr";
    s_colorStrMapping["gray"] = "gry";
    s_colorStrMapping["lightblue"] = "lb";
    s_colorStrMapping["lightfresh"] = "lf";
    s_colorStrMapping["lightgrey"] = "lg";
    s_colorStrMapping["lightpurple"] = "lp";
    s_colorStrMapping["lightred"] = "lr";
    s_colorStrMapping["white"] = "w";
    s_colorStrMapping["yellow"] = "y";
}

std::string LeggingHelper::convertLeggingInfoToString(const LeggingType& type, const char* color/* = ""*/, bool isColorShort/* = true*/, const LeggingOrientation& orientation/* = LeggingOrientation_None*/, const LeggingSide& side/* = LeggingSide_None*/){
    
    std::string ret     = "";
    std::string sColor  = std::string(color);
    
    char c[16];
    
    sprintf(c, "%d", (int)type);
    ret.append(c);
    
    if(sColor != ""){
        if(isColorShort){
            sColor = s_colorStrMapping[sColor];
        }
        sprintf(c, "_%s", sColor.c_str());
        ret.append(c);
    }
    
    if(orientation != LeggingOrientation_None){
        sprintf(c, "_%d", (int)orientation);
        ret.append(c);
    }
    
    if(side != LeggingSide_None){
        if(side == LeggingSide_Left){
            sprintf(c, "_l");
        }else{
            sprintf(c, "_r");
        }
        ret.append(c);
    }
    
    return ret;
}
