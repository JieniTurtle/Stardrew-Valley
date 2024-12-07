#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include"MainCharacter.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
class GameScene : public cocos2d::Scene
{
public:
    static Scene* createScene();
    virtual bool init();

  
    MainCharacter* character;//主角
    int mapWidth;   // 横向瓷砖数量
    int mapHeight; // 纵向瓷砖数量
    int tileWidth;  // 单个瓷砖的像素宽度
    int tileHeight;  // 单个瓷砖的像素高度
    CREATE_FUNC(GameScene);

};

#endif // __HELLOWORLD_SCENE_H__
#pragma once
