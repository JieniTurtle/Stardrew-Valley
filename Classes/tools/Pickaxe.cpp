#include "Pickaxe.h"
#include "SimpleAudioEngine.h"

bool Pickaxe::init(TMXTiledMap* map) {
    if (!ToolBase::init(map)) {
        return false;
    }
    
    // 初始化向后兼容的成员变量
    ispickaxe = isActive;
    
    // 使用基类的统一方法设置复选框
    setCheckbox("pickaxe1.png", "pickaxe2.png", 1.5f / 8.0f, 1.0f / 4.0f);
    
    // 同步向后兼容的成员变量
    pickaxecheckbox = checkbox;
    
    // 使用基类的统一方法设置鼠标监听
    setupMouseListener(map);
    
    return true;
}

Pickaxe* Pickaxe::create(TMXTiledMap* map) {
    Pickaxe* ret = new Pickaxe();
    if (ret && ret->init(map)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void Pickaxe::handleMouseClick(TMXTiledMap* map, Vec2 clickPos, int tileX, int tileY) {
    // 镐子特定的处理逻辑：挖矿
    auto tileLayer = map->getLayer("Mineral");
    if (!tileLayer) {
        return;
    }
    
    int tileGID = tileLayer->getTileGIDAt(Vec2(tileX, tileY));
    
    if (tileGID == StoneID) {
        tileLayer->setTileGID(0, Vec2(tileX, tileY));
        experience += GemExp;
        stone_number++;
    }
    
    if (tileGID == GemID) {
        tileLayer->setTileGID(0, Vec2(tileX, tileY));
        experience += GemExp;
        mineral_number++;
    }
}
