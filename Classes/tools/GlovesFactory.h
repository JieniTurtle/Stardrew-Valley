#ifndef __GLOVES_FACTORY_H__
#define __GLOVES_FACTORY_H__

#include "IToolFactory.h"
#include "Gloves.h"
#include "ToolType.h"

// 手套工厂 - 负责创建Gloves对象
// 这是工厂方法模式的具体工厂实现
class GlovesFactory : public IToolFactory {
public:
    virtual ToolBase* createTool(TMXTiledMap* map) override {
        return Gloves::create(map);
    }
    
    virtual ToolType getToolType() const override {
        return ToolType::GLOVES;
    }
};

#endif // __GLOVES_FACTORY_H__

