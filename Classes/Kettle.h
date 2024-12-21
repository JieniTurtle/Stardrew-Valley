#ifndef __KETTLE_H__
#define __KETTLE_H__

#include "cocos2d.h"
#include"global.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
#define ScaleFactor 2  //扩大因子，地图乘2
#define HoeOverID 1753  //耕作后的图块id
#define AbleHoeID 809  //可耕做的图块id
#define RawPlantNoWaterID 5529//发芽图块id没浇水
#define RawPlantNoWaterTwoID 5531//发芽图块id没浇水
#define RawPlantWaterID 5535//发芽图块id浇水
#define RawPlantWaterTwoID 5537//发芽图块id浇水
#define MaturePlantID 5548//成熟图块id
#define MaturePlantTwoID 5552//成熟图块id
//extern int MatureTime; //成熟时间只浇水
// 工具类
class Kettle : public cocos2d::Layer {
public:
    bool iskettle;//是否拿锄头
    ui::CheckBox* kettlecheckbox;//锄头复选框
    Vec2 mapPosition;//地图位置
    int maplength;//地图长度
    int mapwidth;//地图宽度
    Size visibleSize;//获取当前游戏视图窗口的尺寸
    int mapWidth;  // 横向瓷砖数量
    int mapHeight; // 纵向瓷砖数量
    int tileWidth; // 单个瓷砖的像素宽度
    int tileHeight; // 单个瓷砖的像素高度
    virtual bool init(TMXTiledMap* map);
    void setkettlecheckbox();//设置锄头复选框
    int scheduleCounter;//计时器

    static Kettle* create(TMXTiledMap* map);
    void kettleListenerMouse(TMXTiledMap* map);//锄地监听函数
    //随时取消图块变化
    void myUpdateFunction(Action* rebackaction, TMXLayer* tileLayer, int tileX, int tileY, std::string updateKey);
};


#endif
