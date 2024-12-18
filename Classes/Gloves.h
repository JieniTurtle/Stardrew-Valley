#ifndef __GLOVES_H__
#define __GLOVES_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Wheat.h"
USING_NS_CC;
#define ScaleFactor 2  //扩大因子，地图乘2
#define MaturePlantID 5548//成熟图块id
#define AbleHoeID 809  //可耕做的图块id
// 工具类
class Gloves : public cocos2d::Layer {
public:
    bool isgloves;//是否戴手套
    ui::CheckBox* glovescheckbox;//手套复选框
    Vec2 mapPosition;//地图位置
    int maplength;//地图长度
    int mapwidth;//地图宽度
    Size visibleSize;//获取当前游戏视图窗口的尺寸
    int mapWidth;  // 横向瓷砖数量
    int mapHeight; // 纵向瓷砖数量
    int tileWidth; // 单个瓷砖的像素宽度
    int tileHeight; // 单个瓷砖的像素高度
    //
    virtual bool init(TMXTiledMap* map,int& wheatnum);
    void setglovescheckbox();//设置手套复选框

    static Gloves* create(TMXTiledMap* map, int& wheatnum);
    void glovesListenerMouse(TMXTiledMap* map, int& wheatnum);//手套监听函数

};


#endif
#pragma once
