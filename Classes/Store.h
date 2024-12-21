#ifndef __STORE_H__
#define __STORE_H__

#include "cocos2d.h"
#include "global.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
#define ScaleFactor 2  //扩大因子，地图乘2

#define MilkSellPrice 15
#define WoolSellPrice 12
#define EggSellPrice 12


#define FailSellDisplayTime 0.5

// 工具类
class Store : public cocos2d::Layer {
public:

    Vec2 mapPosition;//地图位置
    int maplength;//地图长度
    int mapwidth;//地图宽度
    Size visibleSize;//获取当前游戏视图窗口的尺寸
    int mapWidth;  // 横向瓷砖数量
    int mapHeight; // 纵向瓷砖数量
    int tileWidth; // 单个瓷砖的像素宽度
    int tileHeight; // 单个瓷砖的像素高度
    MenuItemImage* openItem;//打开按钮
    MenuItemImage* closeItem;//关闭按钮
    MenuItemImage* wheatsellItem;//小麦卖出按钮
    MenuItemImage* carrotsellItem;//carrot卖出按钮
    MenuItemImage* milksellItem;//牛奶卖出按钮
    MenuItemImage* woolsellItem;//羊毛卖出按钮
    MenuItemImage* eggsellItem;//鸡蛋卖出按钮
    MenuItemImage* seedsbuyItem;//seeds卖出按钮
    MenuItemImage* seedstwobuyItem;//seedstwo卖出按钮
    Label* woolsellLabel;//羊毛价格标签
    Label* eggsellLabel;//鸡蛋价格标签
    Label* wheatsellLabel;//小麦价格标签
    Label* carrotsellLabel;//carrot价格标签
    Label* milksellLabel;//牛奶价格标签
    Label* seedsbuyLabel;//seeds价格标签
    Label* seedstwobuyLabel;//seedstwo价格标签
    Layer* StoreScene;//商店界面
    Menu* closemenu;//关闭菜单
    Sprite* emptystoragesprite;//空货架
    //
    void setopenitem();//设置打开按钮
    void setstorescene();//设置商店界面
    void Store::setsell();
    void Store::setbuy();//买入
    virtual bool init(TMXTiledMap* map);
    void menuOpenCallback();//打开商店界面
    static Store* create(TMXTiledMap* map);
    void CloseCallback();//关闭商店界面
    void sellfail();//显示卖出失败
    void buyfail();//显示买入失败
    void pricechange();
};
#endif