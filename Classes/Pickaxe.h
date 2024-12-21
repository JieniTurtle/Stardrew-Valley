#ifndef __PICKAXE_H__
#define __PICKAXE_H__

#include "cocos2d.h"
#include "global.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
#define ScaleFactor 2  //扩大因子，地图乘2
#define StoneID 5564  //普通石块
#define GemID 5530  //宝石快
#define GemExp 20
// 稿子类
class Pickaxe : public cocos2d::Layer {
public:
    bool ispickaxe;//是否拿
    ui::CheckBox* pickaxecheckbox;//复选框
    Vec2 mapPosition;//地图位置
    int maplength;//地图长度
    int mapwidth;//地图宽度
    Size visibleSize;//获取当前游戏视图窗口的尺寸
    int mapWidth;  // 横向瓷砖数量
    int mapHeight; // 纵向瓷砖数量
    int tileWidth; // 单个瓷砖的像素宽度
    int tileHeight; // 单个瓷砖的像素高度
    virtual bool init(TMXTiledMap* map);
    void setpickaxecheckbox();//设置锄头复选框

    static Pickaxe* create(TMXTiledMap* map);
    void pickaxeListenerMouse(TMXTiledMap* map);//锄地监听函数

};


#endif

