#ifndef __FISHING_H__
#define __FISHING_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include"Maincharacter.h"
USING_NS_CC;
#define ScaleFactor 2  //扩大因子，地图乘2
#define AbleFishing 1403
// 钓鱼竿类
class Fishing : public cocos2d::Layer {
public:
    bool isfishing;//是否戴fishing
    bool fishingcondition;//是否为钓鱼状态
    ui::CheckBox* fishingcheckbox;//fishing复选框
    Vec2 mapPosition;//地图位置
    int maplength;//地图长度
    int mapwidth;//地图宽度
    Size visibleSize;//获取当前游戏视图窗口的尺寸
    int mapWidth;  // 横向瓷砖数量
    int mapHeight; // 纵向瓷砖数量
    int tileWidth; // 单个瓷砖的像素宽度
    int tileHeight; // 单个瓷砖的像素高度
    Label* isfishingnow;//正在钓鱼标签
    Sprite* isfishingnowtext;//前一个背景
    int scheduleCounter;
   
    //
    virtual bool init(TMXTiledMap* map, MainCharacter* maincharacter,int& fishnum);
    void setfishingcheckbox();//设置fishing复选框
    void ifgainfish(int& fishnum);
    static Fishing* create(TMXTiledMap* map, MainCharacter* maincharacter, int& fishnum);
    void fishingListenerMouse(TMXTiledMap* map, MainCharacter* maincharacter, int& fishnum);//fishing监听函数
    void fishingprocess(TMXTiledMap* map, MainCharacter* maincharacter, int& fishnum);//执行钓鱼状态的动作
    void myUpdateFunction(std::string updateKey);
    void myUpdateFunction3(int& fishnum, std::string updateKey3);
    void myUpdateFunction2(TMXTiledMap* map, MainCharacter* maincharacter, std::string updateKey);
};


#endif
