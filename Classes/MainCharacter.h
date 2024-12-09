
#ifndef __MAINCHARACTER_H__
#define __MAINCHARACTER_H__

#include "cocos2d.h"

USING_NS_CC;

class MainCharacter : public Sprite {
public:
    static MainCharacter* create(const std::string& filename); // 重载创建方法
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);//wasd四个键移动事件
    void addKeyboardListener();//添加键盘监听事件
    virtual bool init(const std::string& filename);
 
    Animation* walkAnimationdown; // 走路动画
    Animation* walkAnimationup;
    Animation* walkAnimationleft;
    Animation* walkAnimationright;
};

#endif 
#pragma once
