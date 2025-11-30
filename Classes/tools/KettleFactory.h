#ifndef __KETTLE_FACTORY_H__
#define __KETTLE_FACTORY_H__

#include "IToolFactory.h"
#include "Kettle.h"
#include "ToolType.h"

// 水壶工厂 - 负责创建Kettle对象
// 这是工厂方法模式的具体工厂实现
class KettleFactory : public IToolFactory {
public:
    virtual ToolBase* createTool(TMXTiledMap* map) override {
        return Kettle::create(map);
    }
    
    virtual ToolType getToolType() const override {
        return ToolType::KETTLE;
    }
};

#endif // __KETTLE_FACTORY_H__

