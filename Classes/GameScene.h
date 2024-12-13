#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "animals.h"
#include"MainCharacter.h"
#include "ui/CocosGUI.h"
#include "Seeds.h"
#include "Gloves.h"
#include "Wheat.h"
#include "NPC.h"
USING_NS_CC;

enum Mouse {
    notTaken,
    TakenByNPC
};

class GameScene : public cocos2d::Scene
{
public:
    //用于指示鼠标被谁占据，1代表被NPC占据
    enum Mouse MouseStatus;
    //地图比例
    static int mapscale;

    static Scene* createScene(std::string s);
    static GameScene* create(std::string s);
    bool init();
    void CheckboxOnlyone();//只能选择一个复选框
  
    MainCharacter* character;//主角
    int mapWidth;   // 横向瓷砖数量
    int mapHeight; // 纵向瓷砖数量
    int tileWidth;  // 单个瓷砖的像素宽度
    int tileHeight;  // 单个瓷砖的像素高度

    NPC* NPC_Willy;
    NPC* NPC_Gus;
    NPC* NPC_Jodi;
    NPC* NPC_Harvey;

    NPC* initNPC(std::string NPC_Name, std::vector<Vec2>& NPC_Path, TMXTiledMap* NPC_Map);

    void addMouseListener();
    void Mouseupdate(float delta);
    void onMouseDown(cocos2d::Event* event);
    void MouseClickNPC(NPC* npc);
    Tools* tools ;//传入工具   
    Seeds* seeds;//传入种子   
    Wheat* wheat ;//传入小麦   
    Gloves* gloves ;//传入手套
    std::string scene_name_;

    int clickNPCButtons();
};

#endif // __HELLOWORLD_SCENE_H__
#pragma once
