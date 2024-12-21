#ifndef __FERTILIZER_H__
#define __FERTILIZER_H__
#include "global.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
#define ScaleFactor 2  //扩大因子，地图乘2
#define RawPlantWaterID 5535//发芽图块id浇水
#define MaturePlantID 5548//成熟图块id
#define RawPlantWaterTwoID 5537//发芽图块id浇水
#define MaturePlantTwoID 5552//成熟图块id
#define MatureTimeFertilizer 1.0f //成熟时间有肥料
// 肥料类
class Fertilizer : public cocos2d::Scene {
public:
    bool isfertilizer;//是否拿肥料

    ui::CheckBox* fertilizercheckbox;//肥料复选框
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
    void setfertilizercheckbox();//设置锄头复选框 
    static Fertilizer* create(TMXTiledMap* map);
    void fertilizerListenerMouse(TMXTiledMap* map);//锄地监听函数
    void showfertilizernum();//屏幕显示种子数量
};
#endif

