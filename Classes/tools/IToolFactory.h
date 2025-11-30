#ifndef __ITOOL_FACTORY_H__
#define __ITOOL_FACTORY_H__

#include "ToolBase.h"
#include "ToolType.h"
#include "cocos2d.h"

USING_NS_CC;

// 抽象工厂接口 - 工厂方法模式的核心
// 每个具体工厂都必须实现这个接口
class IToolFactory {
public:
    virtual ~IToolFactory() {}
    
    // 工厂方法：创建工具对象
    // 每个具体工厂负责创建对应的工具
    virtual ToolBase* createTool(TMXTiledMap* map) = 0;
    
    // 获取工具类型（用于注册和管理）
    virtual ToolType getToolType() const = 0;
};

#endif // __ITOOL_FACTORY_H__

