#include "Axe.h"
#include "SimpleAudioEngine.h"

bool Axe::init(TMXTiledMap* map) {
    if (!ToolBase::init(map)) {
        return false;
    }
    
    // 初始化向后兼容的成员变量
    isaxe = isActive;
    
    // 使用基类的统一方法设置复选框
    setCheckbox("axe1.png", "axe2.png", 2.0f / 8.0f, 1.0f / 4.0f);
    
    // 同步向后兼容的成员变量
    axecheckbox = checkbox;
    
    // 使用基类的统一方法设置鼠标监听
    setupMouseListener(map);
    
    return true;
}

Axe* Axe::create(TMXTiledMap* map) {
    Axe* ret = new Axe();
    if (ret && ret->init(map)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void Axe::handleMouseClick(TMXTiledMap* map, Vec2 clickPos, int tileX, int tileY) {
    // 斧头特定的处理逻辑：砍树
    for (int i = 1; i <= numberOfTree; ++i) {
        std::string layerName = "Tree" + std::to_string(i);
        auto treeLayer = map->getLayer(layerName);

        if (treeLayer) {
            int tileGID = treeLayer->getTileGIDAt(Vec2(tileX, tileY));
            if (tileGID != 0) {
                map->removeChild(treeLayer, true);
                wood_number += OneTreeForWood;
                experience += WoodExp;
            }
        }
    }
}
