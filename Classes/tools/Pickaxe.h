#ifndef __PICKAXE_H__
#define __PICKAXE_H__

#include "ToolBase.h"
#include "global.h"
USING_NS_CC;

#define ScaleFactor 2  //地图缩放，地图*2
#define StoneID 5564  //普通石头
#define GemID 5530  //宝石
#define GemExp 20

// 镐子 - 继承ToolBase基类
class Pickaxe : public ToolBase {
public:
    static Pickaxe* create(TMXTiledMap* map);
    
    // 实现基类的纯虚函数 - 处理镐子特定的点击逻辑
    virtual void handleMouseClick(TMXTiledMap* map, Vec2 clickPos, int tileX, int tileY) override;
    
    // 重写init方法以设置镐子特定的UI
    virtual bool init(TMXTiledMap* map) override;
    
    // 向后兼容的成员变量（在init中同步）
    bool ispickaxe;
    ui::CheckBox* pickaxecheckbox;
};

#endif
