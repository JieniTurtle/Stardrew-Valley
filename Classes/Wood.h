#ifndef __WOOD_H__
#define __WOOD_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
#define ScaleFactor 2  //扩大因子，地图乘2
// 木头类
class Wood : public cocos2d::Scene {
public:
    bool iswood;//是否拿木头
    int woodnum;//木头数量
    ui::CheckBox* woodcheckbox;//种子复选框
    Vec2 mapPosition;//地图位置
    int maplength;//地图长度
    int mapwidth;//地图宽度
    Size visibleSize;//获取当前游戏视图窗口的尺寸


    int mapWidth;  // 横向瓷砖数量
    int mapHeight; // 纵向瓷砖数量
    int tileWidth; // 单个瓷砖的像素宽度
    int tileHeight; // 单个瓷砖的像素高度
    //
    virtual bool init(TMXTiledMap* map);
    void setwoodcheckbox();//设置木头复选框 
    static Wood* create(TMXTiledMap* map);
    //void wheatListenerMouse(TMXTiledMap* map);//锄地监听函数
    void showwoodnum();//屏幕显示木头数量
};
#endif
#pragma once
