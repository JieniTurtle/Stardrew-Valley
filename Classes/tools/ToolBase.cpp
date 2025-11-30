#include "ToolBase.h"

bool ToolBase::init(TMXTiledMap* map) {
    isActive = 0;
    visibleSize = Director::getInstance()->getVisibleSize();

    mapWidth = map->getMapSize().width;
    mapHeight = map->getMapSize().height;
    tileWidth = map->getTileSize().width * ScaleFactor;
    tileHeight = map->getTileSize().height * ScaleFactor;
    maplength = mapWidth * tileWidth;
    mapwidth = mapHeight * tileHeight;
    
    return true;
}

void ToolBase::setCheckbox(const std::string& normalImage, const std::string& selectedImage, float posX, float posY) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    checkbox = ui::CheckBox::create(normalImage, selectedImage);
    checkbox->setPosition(Vec2(visibleSize.width * posX, visibleSize.height * posY));
    this->addChild(checkbox);
}

void ToolBase::convertScreenToMapCoordinates(Vec2& clickPos, TMXTiledMap* map, int& tileX, int& tileY) {
    Vec2 mapPosition = map->getPosition();
    // 转换为以地图左下角为原点
    clickPos.x = clickPos.x - mapPosition.x + maplength / 2;
    clickPos.y = clickPos.y - mapPosition.y + mapwidth / 2;
    // 转换为以地图左上角的图块单位坐标
    tileX = static_cast<int>(clickPos.x / (ScaleFactor * 16));
    tileY = mapHeight - 1 - static_cast<int>((clickPos.y) / (ScaleFactor * 16));
}

void ToolBase::setupMouseListener(TMXTiledMap* map) {
    // 创建鼠标事件监听器
    auto mouseListener = EventListenerMouse::create();

    mouseListener->onMouseDown = [=](Event* event) {
        if (!isActive) {
            return;
        }
        
        EventMouse* mouseEvent = static_cast<EventMouse*>(event);
        // 获取鼠标点击的位置
        Vec2 clickPos = mouseEvent->getLocation();  // 左上角为原点
        clickPos.y = visibleSize.height - clickPos.y;  // 转换为左下角为原点
        
        int tileX, tileY;
        convertScreenToMapCoordinates(clickPos, map, tileX, tileY);
        
        // 调用子类实现的特定处理逻辑
        handleMouseClick(map, clickPos, tileX, tileY);
    };
    
    // 将监听器添加到事件分发器
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

