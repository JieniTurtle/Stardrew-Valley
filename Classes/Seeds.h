#ifndef __SEEDTOOLS_H__
#define __SEEDTOOLS_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
#define ScaleFactor 2  //扩大因子，地图乘2
#define HoeOverID 1753  //耕作后的图块id
#define RawPlantID 5529//发芽图块id
#define MaturePlantID 5548//成熟图块id
#define MatureTime 2.0f //成熟时间
// 种子类
class Seeds : public cocos2d::Scene {
public:
    bool isseeds;//是否拿种子
    int seedsnum;//种子数量
    ui::CheckBox* seedscheckbox;//种子复选框
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
    void setseedscheckbox();//设置锄头复选框 
    static Seeds* create(TMXTiledMap* map);
    void seedsListenerMouse(TMXTiledMap* map);//锄地监听函数
    void showseedsnum();//屏幕显示种子数量
}; 
#endif
