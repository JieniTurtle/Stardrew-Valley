#ifndef __AXE_H__
#define __AXE_H__

#include "cocos2d.h"
#include "Seeds.h"
#include "Gloves.h"
#include "Wheat.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
#define ScaleFactor 2  //扩大因子，地图乘2
#define HoeOverID 1753  //耕作后的图块id
#define AbleHoeID 809  //可耕做的图块id
#define numberOfTree 3  //树图层的数量
#define OneTreeForWood 5  //一棵树等于几个木头
// 工具类
class Axe : public cocos2d::Layer {
public:
    bool isaxe;//是否拿斧头
    ui::CheckBox* axecheckbox;//锄头复选框
    Vec2 mapPosition;//地图位置
    int maplength;//地图长度
    int mapwidth;//地图宽度
    Size visibleSize;//获取当前游戏视图窗口的尺寸
    int mapWidth;  // 横向瓷砖数量
    int mapHeight; // 纵向瓷砖数量
    int tileWidth; // 单个瓷砖的像素宽度
    int tileHeight; // 单个瓷砖的像素高度
    virtual bool init(TMXTiledMap* map, int& woodnum);
    void setaxecheckbox();//设置锄头复选框

    static Axe* create(TMXTiledMap* map, int& woodnum);
    void axeListenerMouse(TMXTiledMap* map, int& woodnum);//锄地监听函数

};


#endif
