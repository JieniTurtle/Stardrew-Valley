#ifndef __GLOVES_H__
#define __GLOVES_H__

#include "ToolBase.h"
#include "global.h"
USING_NS_CC;

#define MaturePlantID 5548//成熟植物图片id
#define MaturePlantTwoID 5552//成熟植物图片id
#define AbleHoeID 809  //可耕地图片id
#define GainExp 20

// 手套 - 继承ToolBase基类
class Gloves : public ToolBase {
public:
    static Gloves* create(TMXTiledMap* map);
    
    // 实现基类的纯虚函数 - 处理手套特定的点击逻辑
    virtual void handleMouseClick(TMXTiledMap* map, Vec2 clickPos, int tileX, int tileY) override;
    
    // 重写init方法以设置手套特定的UI
    virtual bool init(TMXTiledMap* map) override;
    
    // 向后兼容的成员变量（在init中同步）
    bool isgloves;
    ui::CheckBox* glovescheckbox;
};

#endif
#pragma once
