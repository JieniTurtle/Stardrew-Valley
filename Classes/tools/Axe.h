#ifndef __AXE_H__
#define __AXE_H__

#include "ToolBase.h"
#include "global.h"
USING_NS_CC;

#define numberOfTree 3  //地图树数量
#define OneTreeForWood 5  //一棵树能获得几块木头
#define WoodExp 20

// 斧头 - 继承ToolBase基类
class Axe : public ToolBase {
public:
    static Axe* create(TMXTiledMap* map);
    
    // 实现基类的纯虚函数 - 处理斧头特定的点击逻辑
    virtual void handleMouseClick(TMXTiledMap* map, Vec2 clickPos, int tileX, int tileY) override;
    
    // 重写init方法以设置斧头特定的UI
    virtual bool init(TMXTiledMap* map) override;
    
    // 向后兼容的成员变量（在init中同步）
    bool isaxe;
    ui::CheckBox* axecheckbox;
};

#endif
