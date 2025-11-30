#include "Tools.h"
#include "SimpleAudioEngine.h"

bool Tools::init(TMXTiledMap* map) {
    if (!ToolBase::init(map)) {
        return false;
    }
    
    // 初始化向后兼容的成员变量
    ishoe = isActive;
    
    // 使用基类的统一方法设置复选框
    setCheckbox("chutou.png", "chutou2.png", 1.0f / 8.0f, 1.0f / 4.0f);
    
    // 同步向后兼容的成员变量
    hoecheckbox = checkbox;
    
    // 使用基类的统一方法设置鼠标监听
    setupMouseListener(map);
    
    return true;
}

Tools* Tools::create(TMXTiledMap* map) {
    Tools* ret = new Tools();
    if (ret && ret->init(map)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void Tools::handleMouseClick(TMXTiledMap* map, Vec2 clickPos, int tileX, int tileY) {
    // 锄头特定的处理逻辑：耕地
    auto tileLayer = map->getLayer("soil");
    if (!tileLayer) {
        return;
    }
    
    int tileGID = tileLayer->getTileGIDAt(Vec2(tileX, tileY));
    
    if (tileGID == AbleHoeID) {
        tileLayer->setTileGID(HoeOverID, Vec2(tileX, tileY));
    }
}
