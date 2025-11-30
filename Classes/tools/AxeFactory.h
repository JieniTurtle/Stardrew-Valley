#ifndef __AXE_FACTORY_H__
#define __AXE_FACTORY_H__

#include "IToolFactory.h"
#include "Axe.h"
#include "ToolType.h"

// 斧头工厂 - 负责创建Axe对象
// 这是工厂方法模式的具体工厂实现
class AxeFactory : public IToolFactory {
public:
    virtual ToolBase* createTool(TMXTiledMap* map) override {
        return Axe::create(map);
    }
    
    virtual ToolType getToolType() const override {
        return ToolType::AXE;
    }
};

#endif // __AXE_FACTORY_H__

