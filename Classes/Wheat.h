#ifndef __WHEAT_H__
#define __WHEAT_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
#define ScaleFactor 2  //扩大因子，地图乘2
// 小麦类
class Wheat : public cocos2d::Scene {
public:
    bool iswheat;//是否拿小麦
    int wheatnum;//小麦数量
    ui::CheckBox* wheatcheckbox;//种子复选框
    Vec2 mapPosition;//地图位置
    int maplength;//地图长度
    int mapwidth;//地图宽度
    Size visibleSize;//获取当前游戏视图窗口的尺寸
    int tempotileX, tempotileY;
    TMXLayer* tempotileLayer;
    int mapWidth;  // 横向瓷砖数量
    int mapHeight; // 纵向瓷砖数量
    int tileWidth; // 单个瓷砖的像素宽度
    int tileHeight; // 单个瓷砖的像素高度
    //
    virtual bool init(TMXTiledMap* map);
    void setwheatcheckbox();//设置锄头复选框 
    static Wheat* create(TMXTiledMap* map);
   //void wheatListenerMouse(TMXTiledMap* map);//锄地监听函数
    void showwheatnum();//屏幕显示种子数量
};
#endif
