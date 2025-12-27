// Refactored with Factory Method Pattern
#ifndef __TOOLS_FACTORY_H__
#define __TOOLS_FACTORY_H__

#include "IToolFactory.h"
#include "Tools.h"
#include "ToolType.h"

// 基础工具工厂 - 负责创建Tools对象
// 这是工厂方法模式的具体工厂实现
class ToolsFactory : public IToolFactory {
public:
    virtual ToolBase* createTool(TMXTiledMap* map) override {
        return Tools::create(map);
    }
    
    virtual ToolType getToolType() const override {
        return ToolType::TOOLS;
    }
};

#endif // __TOOLS_FACTORY_H__
