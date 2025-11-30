#include "Gloves.h"
#include "SimpleAudioEngine.h"

bool Gloves::init(TMXTiledMap* map) {
    if (!ToolBase::init(map)) {
        return false;
    }
    
    // 初始化向后兼容的成员变量
    isgloves = isActive;
    
    // 使用基类的统一方法设置复选框
    setCheckbox("gloves1.png", "gloves2.png", 3.5f / 8.0f, 1.0f / 4.0f);
    
    // 同步向后兼容的成员变量
    glovescheckbox = checkbox;
    
    // 使用基类的统一方法设置鼠标监听
    setupMouseListener(map);
    
    return true;
}

Gloves* Gloves::create(TMXTiledMap* map) {
    Gloves* ret = new Gloves();
    if (ret && ret->init(map)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void Gloves::handleMouseClick(TMXTiledMap* map, Vec2 clickPos, int tileX, int tileY) {
    // 手套特定的处理逻辑：收获成熟作物
    auto tileLayer = map->getLayer("soil");
    if (!tileLayer) {
        return;
    }
    
    int tileGID = tileLayer->getTileGIDAt(Vec2(tileX, tileY));
    
    if (tileGID == MaturePlantID) {
        tileLayer->setTileGID(AbleHoeID, Vec2(tileX, tileY));
        wheat_number++;
        experience += GainExp;
    }
    
    if (tileGID == MaturePlantTwoID) {
        tileLayer->setTileGID(AbleHoeID, Vec2(tileX, tileY));
        carrot_number++;
        experience += GainExp;
    }
}
