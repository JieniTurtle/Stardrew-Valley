#ifndef __FARMPRODUCT_H__
#define __FARMPRODUCT_H__

#include "cocos2d.h"
#include "global.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
#define ScaleFactor 2  //扩大因子，地图乘2
#define HoeOverID 1753  //耕作后的图块id
#define AbleHoeID 809  //可耕做的图块id
#define RawPlantNoWaterID 5529//发芽图块id没浇水
#define RawPlantWaterID 5535//发芽图块id浇水
#define MaturePlantID 5548//成熟图块id
#define MatureTime 8.0f //成熟时间只浇水
// 工具类
class FarmProduct : public cocos2d::Layer {
public:
  
    ui::CheckBox* milkcheckbox;//牛奶复选框
    ui::CheckBox* eggcheckbox;//鸡蛋复选框
    ui::CheckBox* woolcheckbox;//羊毛复选框
    ui::CheckBox* fishcheckbox;//yu复选框

    Vec2 mapPosition;//地图位置
    int maplength;//地图长度
    int mapwidth;//地图宽度
    Size visibleSize;//获取当前游戏视图窗口的尺寸
    int mapWidth;  // 横向瓷砖数量
    int mapHeight; // 纵向瓷砖数量
    int tileWidth; // 单个瓷砖的像素宽度
    int tileHeight; // 单个瓷砖的像素高度
    virtual bool init(TMXTiledMap* map);
    void setcheckbox();//设置复选框

    void showmilknum();//显示数量
    void showwoolnum();
    void showeggnum();
    void showfishnum();
    static FarmProduct* create(TMXTiledMap* map);
   
};


#endif