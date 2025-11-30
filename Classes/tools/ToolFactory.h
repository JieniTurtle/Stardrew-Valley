#ifndef TOOL_FACTORY_H
#define TOOL_FACTORY_H

#include "ToolBase.h"
#include "ToolType.h"
#include "IToolFactory.h"
#include "ToolsFactory.h"
#include "AxeFactory.h"
#include "GlovesFactory.h"
#include "KettleFactory.h"
#include "PickaxeFactory.h"
#include <map>
#include <vector>
#include <string>
#include <memory>

USING_NS_CC;

// 工厂注册表 - 管理所有工具工厂（工厂方法模式）
// 使用工厂方法模式：每个工具都有对应的工厂类
class ToolFactoryRegistry {
private:
    std::map<ToolType, std::unique_ptr<IToolFactory>> factories;
    
    // 单例模式
    ToolFactoryRegistry() {
        // 注册所有工厂
        registerFactory<ToolsFactory>(ToolType::TOOLS);
        registerFactory<AxeFactory>(ToolType::AXE);
        registerFactory<GlovesFactory>(ToolType::GLOVES);
        registerFactory<KettleFactory>(ToolType::KETTLE);
        registerFactory<PickaxeFactory>(ToolType::PICKAXE);
    }
    
    template<typename FactoryType>
    void registerFactory(ToolType type) {
        factories[type] = std::make_unique<FactoryType>();
    }
    
public:
    // 获取单例
    static ToolFactoryRegistry& getInstance() {
        static ToolFactoryRegistry instance;
        return instance;
    }
    
    // 根据类型获取工厂
    IToolFactory* getFactory(ToolType type) const {
        auto it = factories.find(type);
        return (it != factories.end()) ? it->second.get() : nullptr;
    }
    
    // 根据类型创建工具（委托给对应的工厂）
    ToolBase* createTool(ToolType type, TMXTiledMap* map) const {
        IToolFactory* factory = getFactory(type);
        return factory ? factory->createTool(map) : nullptr;
    }
};

// 工具工厂管理器 - 提供统一的接口（保持向后兼容）
// 这是工厂方法模式的客户端接口
class ToolFactory {
public:
    // 工厂方法：根据类型创建工具，返回统一的基类指针
    // 内部使用工厂注册表，委托给对应的具体工厂
    static ToolBase* createTool(ToolType type, TMXTiledMap* map) {
        return ToolFactoryRegistry::getInstance().createTool(type, map);
    }
    
    // 可以根据字符串创建工具（从配置文件读取）
    static ToolBase* createToolFromString(const std::string& toolName, TMXTiledMap* map) {
        if (toolName == "tools" || toolName == "hoe") return createTool(ToolType::TOOLS, map);
        if (toolName == "gloves") return createTool(ToolType::GLOVES, map);
        if (toolName == "axe") return createTool(ToolType::AXE, map);
        if (toolName == "kettle") return createTool(ToolType::KETTLE, map);
        if (toolName == "pickaxe") return createTool(ToolType::PICKAXE, map);
        return nullptr;
    }
    
    // 批量创建工具 - 工厂模式的优势：统一接口，批量处理
    static std::vector<ToolBase*> createAllTools(TMXTiledMap* map) {
        std::vector<ToolBase*> tools;
        tools.push_back(createTool(ToolType::TOOLS, map));
        tools.push_back(createTool(ToolType::GLOVES, map));
        tools.push_back(createTool(ToolType::AXE, map));
        tools.push_back(createTool(ToolType::KETTLE, map));
        tools.push_back(createTool(ToolType::PICKAXE, map));
        return tools;
    }
};

#endif // TOOL_FACTORY_H
