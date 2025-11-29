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
#include "ingredient.h"
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
    //����ָʾ��걻˭ռ�ݣ�1������NPCռ��
    enum Mouse MouseStatus;
    //��ͼ����
    static int mapscale;
    Size visibleSize;
    Vec2 origin;
    static Scene* createScene(std::string s, int direction);
    static GameScene* create(std::string s, int direction);
    bool init(int direction);

    void set_physical_map(TMXTiledMap* map);

    MainCharacter* character;//����
    int mapWidth;   // �����ש����
    int mapHeight; // �����ש����
    int tileWidth;  // ������ש�����ؿ���
    int tileHeight;  // ������ש�����ظ߶�

    NPC* NPC_Willy;
    NPC* NPC_Gus;
    NPC* NPC_Jodi;
    NPC* NPC_Harvey;

    NPC* initNPC(std::string NPC_Name, std::vector<Vec2>& NPC_Path, TMXTiledMap* NPC_Map);
 

    Tools* tools;//�����ͷ
    Pickaxe* pickaxe;//�������
    Seeds* seeds;//��������   
    Seedstwo* seedstwo;//�������� 2
    Wheat* wheat;//����С��   
    Carrot* carrot;//����carrot
    Gloves* gloves;//��������
    Axe* axe;//���븫��
    Wood* wood;//����wood
    Kettle* kettle;//ˮ��
    Fertilizer* fertilizer;//����
    FarmProduct* farmproduct;//ũ����
    Mineral* mineral;//��ʯ��
    //
    Store* store;//�̵�
    Fishing* fishing;//�����
    CookLayer* cookLayer;

    std::string scene_name_;

    int clickNPCButtons();
    void showmoneynum();
    void showdark();
    void createRain();
    void createSunEffect();
    void weatherchange();

    //���ܶ���ʾ
    Label* relationTip;
    Sprite* relationFull;
    void NPCTaskManger(NPC* npc);
};

#endif // __HELLOWORLD_SCENE_H__
#pragma once
