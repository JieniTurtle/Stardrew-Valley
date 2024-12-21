#ifndef __MINERAL_H__
#define __MINERAL_H__

#include "cocos2d.h"
#include "global.h"

#include "ui/CocosGUI.h"
USING_NS_CC;
#define ScaleFactor 2  //扩大因子，地图乘2

// 矿物类
class Mineral : public cocos2d::Layer {
public:

    ui::CheckBox* stonecheckbox;//stone复选框
    ui::CheckBox* gemcheckbox;//gem复选框

    int stonenum= stone_number;//stone数量
    int gemnum = mineral_number;//gem数量

    Vec2 mapPosition;//地图位置
    int maplength;//地图长度
    int mapwidth;//地图宽度
    Size visibleSize;//获取当前游戏视图窗口的尺寸
    int mapWidth;  // 横向瓷砖数量
    int mapHeight; // 纵向瓷砖数量
    int tileWidth; // 单个瓷砖的像素宽度
    int tileHeight; // 单个瓷砖的像素高度
    virtual bool init(TMXTiledMap* map);
    void setcheckbox();//设置复选框

    void showstonenum();//显示数量
    void showgemnum();
 
    static Mineral* create(TMXTiledMap* map);

};


#endif
