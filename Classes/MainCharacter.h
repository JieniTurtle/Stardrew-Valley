
#ifndef __MAINCHARACTER_H__
#define __MAINCHARACTER_H__

#include "cocos2d.h"
#include "Tools.h"
USING_NS_CC;

class MainCharacter : public Sprite {
public:
    static MainCharacter* create(const std::string& filename); // 重载创建方法
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);//wasd四个键移动事件
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);//wasd四个键移动事件
    void addKeyboardListener();//添加键盘监听事件
    void update(float delta);
    //传入地图指针
    void setMap(TMXTiledMap* map) {
        mainmap = map;
    }

    std::string staticnext="characterdown2.png";//静止方向图片
    virtual bool init(const std::string& filename);
    int movementkeys[4];//储存wasd四个键为
    Animation* walkAnimationdown; // 走路动画
    Animation* walkAnimationup;
    Animation* walkAnimationleft;
    Animation* walkAnimationright;
    TMXTiledMap* mainmap;//地图指针
};

#endif 
#pragma once
