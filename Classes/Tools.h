#ifndef __TOOLS_H__
#define __TOOLS_H__

#include "cocos2d.h"
#include "Seeds.h"
#include "Gloves.h"
#include "Wheat.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
#define ScaleFactor 2  //扩大因子，地图乘2
#define HoeOverID 1753  //耕作后的图块id
#define AbleHoeID 809  //可耕做的图块id
// 工具类
class Tools: public cocos2d::Layer{
public:
    bool ishoe;//是否拿锄头
    ui::CheckBox* hoecheckbox;//锄头复选框
    Vec2 mapPosition;//地图位置
    int maplength;//地图长度
    int mapwidth;//地图宽度
    Size visibleSize;//获取当前游戏视图窗口的尺寸
    int mapWidth ;  // 横向瓷砖数量
    int mapHeight ; // 纵向瓷砖数量
    int tileWidth ; // 单个瓷砖的像素宽度
    int tileHeight ; // 单个瓷砖的像素高度
    virtual bool init(TMXTiledMap* map);
    void sethoecheckbox();//设置锄头复选框
 
    static Tools* create(TMXTiledMap* map);
    void hoeListenerMouse(TMXTiledMap* map);//锄地监听函数
   
};


#endif
