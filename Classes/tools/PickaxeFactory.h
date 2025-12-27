// Refactored with Factory Method Pattern
#ifndef __PICKAXE_FACTORY_H__
#define __PICKAXE_FACTORY_H__

#include "IToolFactory.h"
#include "Pickaxe.h"
#include "ToolType.h"

// 镐头工厂 - 负责创建Pickaxe对象
// 这是工厂方法模式的具体工厂实现
class PickaxeFactory : public IToolFactory {
public:
    virtual ToolBase* createTool(TMXTiledMap* map) override {
        return Pickaxe::create(map);
    }
    
    virtual ToolType getToolType() const override {
        return ToolType::PICKAXE;
    }
};

#endif // __PICKAXE_FACTORY_H__