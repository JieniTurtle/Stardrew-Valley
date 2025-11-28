#ifndef TOOL_FACTORY_H
#define TOOL_FACTORY_H

#include "cocos2d.h"
#include "Tools.h"
#include "Gloves.h"
#include "Axe.h"
#include "Kettle.h"
#include "Pickaxe.h"

enum class ToolType {
    TOOLS,
    GLOVES,
    AXE,
    KETTLE,
    PICKAXE,
};

class ToolFactory {
public:
    static cocos2d::Layer* createTool(ToolType type, cocos2d::TMXTiledMap* map) {
        switch (type) {
        case ToolType::TOOLS:
            return Tools::create(map);
        case ToolType::GLOVES:
            return Gloves::create(map);
        case ToolType::AXE:
            return Axe::create(map);
        case ToolType::KETTLE:
            return Kettle::create(map);
        case ToolType::PICKAXE:
            return Pickaxe::create(map);
        default:
            return nullptr;
        }
    }
};

#endif // TOOL_FACTORY_H


