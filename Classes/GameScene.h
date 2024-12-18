#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "animals.h"
#include"MainCharacter.h"
#include "ui/CocosGUI.h"

#include "NPC.h"

#include "Seeds.h"
#include "Seedstwo.h"
#include "Gloves.h"
#include "Wheat.h"
#include "Axe.h"
#include "Wood.h"
#include "Kettle.h"
#include "Fertilizer.h"
#include "FarmProduct.h"
#include "Store.h"
#include "Carrot.h"
#include "Fishing.h"
#include "Mineral.h"
#include "Pickaxe.h"
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
    Size visibleSize;
    Vec2 origin;
    static Scene* createScene(std::string s, int direction);
    static GameScene* create(std::string s, int direction);
    bool init(int direction);
    void CheckboxOnlyone();//只能选择一个复选框

    void set_physical_map(TMXTiledMap* map);

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
 

    Tools* tools;//传入锄头
    Pickaxe* pickaxe;//传入稿子
    Seeds* seeds;//传入种子   
    Seedstwo* seedstwo;//传入种子 2
    Wheat* wheat;//传入小麦   
    Carrot* carrot;//传入carrot
    Gloves* gloves;//传入手套
    Axe* axe;//传入斧子
    Wood* wood;//传入wood
    Kettle* kettle;//水壶
    Fertilizer* fertilizer;//肥料
    FarmProduct* farmproduct;//农作物
    Mineral* mineral;//矿石类
    //
    Store* store;//商店
    Fishing* fishing;//钓鱼竿
    std::string scene_name_;

    int clickNPCButtons();
    void CloseOtherCheckbox(ui::CheckBox* selectedCheckbox);
    void showmoneynum();
};

#endif // __HELLOWORLD_SCENE_H__
#pragma once
