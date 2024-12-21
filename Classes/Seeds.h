#ifndef __SEEDTOOLS_H__
#define __SEEDTOOLS_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "global.h"
USING_NS_CC;
#define ScaleFactor 2  //扩大因子，地图乘2
#define HoeOverID 1753  //耕作后的图块id
#define RawPlantID 5529//发芽图块id没浇水
#define MaturePlantID 5548//成熟图块id
#define MatureTime 2.0f //成熟时间
extern int NoWaterDieTime; //没有浇水最长生存时间
#define AbleHoeID 809  //可耕做的图块id
// 种子类
class Seeds : public cocos2d::Scene {
public:
    bool isseeds;//是否拿种子

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
    int scheduleCounter;
    //
    virtual bool init(TMXTiledMap* map);
    void setseedscheckbox();//设置锄头复选框 
    static Seeds* create(TMXTiledMap* map);
    void seedsListenerMouse(TMXTiledMap* map);//锄地监听函数
    void showseedsnum();//屏幕显示种子数量
    //调度函数，判断是否复原土地，有浇水操作时取消该动作
    void Seeds::myUpdateFunction(Action* rebackaction, TMXLayer* tileLayer, int tileX, int tileY, std::string updateKey);
}; 
#endif
