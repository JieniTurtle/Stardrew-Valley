#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include"MainCharacter.h"
#include "ui/CocosGUI.h"
#include "Seeds.h"
#include "Gloves.h"
#include "Wheat.h"
USING_NS_CC;
class GameScene : public cocos2d::Scene
{
public:
    static Scene* createScene(std::string s);
    static GameScene* create(std::string s);
    bool init();
    void CheckboxOnlyone();//只能选择一个复选框
  
    MainCharacter* character;//主角
    int mapWidth;   // 横向瓷砖数量
    int mapHeight; // 纵向瓷砖数量
    int tileWidth;  // 单个瓷砖的像素宽度
    int tileHeight;  // 单个瓷砖的像素高度

    Tools* tools ;//传入工具   
    Seeds* seeds;//传入种子   
    Wheat* wheat ;//传入小麦   
    Gloves* gloves ;//传入手套
    std::string scene_name_;
};

#endif // __HELLOWORLD_SCENE_H__
#pragma once
