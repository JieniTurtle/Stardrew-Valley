#ifndef __KETTLE_H__
#define __KETTLE_H__

#include "ToolBase.h"
#include "global.h"
USING_NS_CC;

#define RawPlantNoWaterID 5529//发芽图片id没有水
#define RawPlantNoWaterTwoID 5531//发芽图片id没有水
#define RawPlantWaterID 5535//发芽图片id有水
#define RawPlantWaterTwoID 5537//发芽图片id有水
#define MaturePlantID 5548//成熟图片id
#define MaturePlantTwoID 5552//成熟图片id

// 水壶 - 继承ToolBase基类
class Kettle : public ToolBase {
public:
    int scheduleCounter;  // 定时器计数器
    
    static Kettle* create(TMXTiledMap* map);
    
    // 实现基类的纯虚函数 - 处理水壶特定的点击逻辑
    virtual void handleMouseClick(TMXTiledMap* map, Vec2 clickPos, int tileX, int tileY) override;
    
    // 重写init方法以设置水壶特定的UI
    virtual bool init(TMXTiledMap* map) override;
    
    // 定时取消图片变化
    void myUpdateFunction(Action* rebackaction, TMXLayer* tileLayer, int tileX, int tileY, std::string updateKey);
    
    // 向后兼容的成员变量（在init中同步）
    bool iskettle;
    ui::CheckBox* kettlecheckbox;
};

#endif
