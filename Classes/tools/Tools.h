#ifndef __TOOLS_H__
#define __TOOLS_H__

#include "ToolBase.h"
#include "global.h"
USING_NS_CC;

#define HoeOverID 1753  //已耕地图片id
#define AbleHoeID 809  //可耕地图片id

// 锄头 - 继承ToolBase基类（命名：Tools实际是Hoe锄头）
class Tools : public ToolBase {
public:
    static Tools* create(TMXTiledMap* map);
    
    // 实现基类的纯虚函数 - 处理锄头特定的点击逻辑
    virtual void handleMouseClick(TMXTiledMap* map, Vec2 clickPos, int tileX, int tileY) override;
    
    // 重写init方法以设置锄头特定的UI
    virtual bool init(TMXTiledMap* map) override;
    
    // 向后兼容的成员变量（在init中同步）
    bool ishoe;
    ui::CheckBox* hoecheckbox;
};

#endif
