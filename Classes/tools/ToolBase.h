#ifndef __TOOL_BASE_H__
#define __TOOL_BASE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "global.h"

USING_NS_CC;

#define ScaleFactor 2  //地图缩放，地图*2

// 工具基类 - 提取所有工具的公共功能
class ToolBase : public cocos2d::Layer {
public:
    // 公共成员变量
    bool isActive;  // 工具是否激活（替代ishoe, isaxe等）
    ui::CheckBox* checkbox;  // 统一的复选框（替代hoecheckbox, axecheckbox等）
    Vec2 mapPosition;  // 地图位置
    int maplength;  // 地图长度
    int mapwidth;  // 地图宽度
    Size visibleSize;  // 获取当前游戏视图窗口的尺寸
    int mapWidth;  // 地图的砖块数量
    int mapHeight;  // 地图的砖块数量
    int tileWidth;  // 每个砖块的像素宽度
    int tileHeight;  // 每个砖块的像素高度

    // 公共初始化方法
    virtual bool init(TMXTiledMap* map);
    
    // 统一的设置复选框方法 - 子类只需提供图片路径和位置
    void setCheckbox(const std::string& normalImage, const std::string& selectedImage, float posX, float posY);
    
    // 统一的鼠标监听框架 - 子类实现具体的点击处理逻辑
    void setupMouseListener(TMXTiledMap* map);
    
    // 纯虚函数 - 子类必须实现具体的点击处理逻辑
    virtual void handleMouseClick(TMXTiledMap* map, Vec2 clickPos, int tileX, int tileY) = 0;
    
    // 辅助方法：将屏幕坐标转换为地图坐标
    void convertScreenToMapCoordinates(Vec2& clickPos, TMXTiledMap* map, int& tileX, int& tileY);

protected:
    ToolBase() : isActive(0), checkbox(nullptr) {}
    virtual ~ToolBase() {}
};

#endif // __TOOL_BASE_H__

