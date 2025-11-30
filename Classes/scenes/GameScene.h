#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "animals.h"
#include"MainCharacter.h"
#include "ui/CocosGUI.h"
#include "global.h"
#include "NPC.h"

#include "Seeds.h"
#include "Seedstwo.h"
#include "Wheat.h"
#include "Wood.h"
#include "Fertilizer.h"
#include "FarmProduct.h"
#include "Store.h"
#include "Carrot.h"
#include "Fishing.h"
#include "Mineral.h"
#include "ingredient.h"
#include "ToolBase.h"
#include "ToolType.h"
#include "ToolFactory.h"
#include <map>
USING_NS_CC;
# define NewFishingExp 20
# define NewPickaxeExp 40
# define NewCookLayerExp 60
# define ExpTime 1
#define WeatherChangeTime 20
extern int experience;
extern int money;
enum Mouse {
    notTaken,
    TakenByNPC
};

class GameScene : public cocos2d::Scene
{
public:
    //鼠标指示器被谁占据，1表示被NPC占据
    enum Mouse MouseStatus;
    //地图缩放
    static int mapscale;
    Size visibleSize;
    Vec2 origin;
    static Scene* createScene(std::string s, int direction);
    static GameScene* create(std::string s, int direction);
    bool init(int direction);

    void set_physical_map(TMXTiledMap* map);

    MainCharacter* character;//角色
    int mapWidth;   // 地图的砖块数量
    int mapHeight; // 地图的砖块数量
    int tileWidth;  // 每个砖块的像素宽度
    int tileHeight;  // 每个砖块的像素高度

    NPC* NPC_Willy;
    NPC* NPC_Gus;
    NPC* NPC_Jodi;
    NPC* NPC_Harvey;

    NPC* initNPC(std::string NPC_Name, std::vector<Vec2>& NPC_Path, TMXTiledMap* NPC_Map);
 
    // 工具管理 - 使用工厂模式，统一通过容器管理（完全多态化）
    std::map<ToolType, ToolBase*> toolMap;  // 工具容器，通过工厂创建和管理
    
    // 工具访问器 - 通过类型获取工具（体现工厂模式和多态）
    ToolBase* getTool(ToolType type) const {
        auto it = toolMap.find(type);
        return (it != toolMap.end()) ? it->second : nullptr;
    }

    Seeds* seeds;//种子类   
    Seedstwo* seedstwo;//种子类 2
    Wheat* wheat;//小麦类   
    Carrot* carrot;//胡萝卜carrot
    Wood* wood;//木头wood
    Fertilizer* fertilizer;//肥料
    FarmProduct* farmproduct;//农产品
    Mineral* mineral;//矿石
    //
    Store* store;//商店
    Fishing* fishing;//钓鱼
    CookLayer* cookLayer;

    std::string scene_name_;

    int clickNPCButtons();
    void showmoneynum();
    void showdark();
    void createRain();
    void createSunEffect();
    void weatherchange();

    //关系度显示
    Label* relationTip;
    Sprite* relationFull;
    void NPCTaskManger(NPC* npc);
};

#endif // __HELLOWORLD_SCENE_H__
#pragma once
