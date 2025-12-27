// Refactored with Factory Method Pattern
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
// 使用静态局部变量替代单例，更简洁高效
// 注意：如果未来需要运行时动态注册工厂，可以改回单例模式
namespace {
    // 根据类型获取工厂（使用静态局部变量，C++11保证线程安全且只初始化一次）
    IToolFactory* getFactory(ToolType type) {
        static std::map<ToolType, std::unique_ptr<IToolFactory>> factories = []() {
            std::map<ToolType, std::unique_ptr<IToolFactory>> map;
            map[ToolType::TOOLS] = std::make_unique<ToolsFactory>();
            map[ToolType::AXE] = std::make_unique<AxeFactory>();
            map[ToolType::GLOVES] = std::make_unique<GlovesFactory>();
            map[ToolType::KETTLE] = std::make_unique<KettleFactory>();
            map[ToolType::PICKAXE] = std::make_unique<PickaxeFactory>();
            return map;
        }();
        
        auto it = factories.find(type);
        return (it != factories.end()) ? it->second.get() : nullptr;
    }
}

// 工具工厂管理器 - 提供统一的接口（保持向后兼容）
// 这是工厂方法模式的客户端接口
class ToolFactory {
public:
    // 工厂方法：根据类型创建工具，返回统一的基类指针
    // 内部使用工厂注册表，委托给对应的具体工厂
    static ToolBase* createTool(ToolType type, TMXTiledMap* map) {
        IToolFactory* factory = getFactory(type);
        return factory ? factory->createTool(map) : nullptr;
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