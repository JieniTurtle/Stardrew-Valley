#include "Gloves.h"
#include "SimpleAudioEngine.h"

bool Gloves::init(TMXTiledMap* map) {
    isgloves = 0;
    visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸

    mapWidth = map->getMapSize().width;  // 横向瓷砖数量
    mapHeight = map->getMapSize().height; // 纵向瓷砖数量
    tileWidth = map->getTileSize().width * ScaleFactor; // 单个瓷砖的像素宽度
    tileHeight = map->getTileSize().height * ScaleFactor; // 单个瓷砖的像素高度
    maplength = mapWidth * tileWidth;
    mapwidth = mapHeight * tileHeight;
    setglovescheckbox();//设置手套复选框

    glovesListenerMouse(map);
    return true;
}

void  Gloves::setglovescheckbox() {
    auto visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸
    //创建锄头图标
    glovescheckbox = ui::CheckBox::create("gloves1.png", "gloves2.png");
    glovescheckbox->setPosition(Vec2(visibleSize.width *3.5/ 8, visibleSize.height / 4)); // 设置位置

    //// 添加事件监听器
    //glovescheckbox->addEventListener([this](Ref* sender, ui::CheckBox::EventType type) {
    //    switch (type) {
    //        case ui::CheckBox::EventType::SELECTED:
    //            isgloves = 1;
    //            break;
    //        case ui::CheckBox::EventType::UNSELECTED:
    //            isgloves = 0;
    //            break;
    //        default:
    //            break;
    //    }
    //    });
    this->addChild(glovescheckbox);


}

Gloves* Gloves::create(TMXTiledMap* map) {

    Gloves* ret = new Gloves();
    if (ret && ret->init(map)) {
        ret->autorelease(); // 自动释放内存
        return ret;
    }
    CC_SAFE_DELETE(ret); // 如果创建失败，安全删除
    return nullptr;
}

void Gloves::glovesListenerMouse(TMXTiledMap* map) {
    // 创建鼠标事件监听器
    auto mouseListener = EventListenerMouse::create();

    mouseListener->onMouseDown = [=](Event* event) {
        EventMouse* mouseEvent = static_cast<EventMouse*>(event);
        Vec2 mapPosition = map->getPosition();
        if (isgloves == 1) {
            // 获取鼠标点击的位置
            Vec2 clickPos = mouseEvent->getLocation();//以左上角为原点
            clickPos.y = visibleSize.height - clickPos.y;//转化为左下角为原点
            auto tileLayer = map->getLayer("soil");
            //坐标转化为相对地图左下角的
            clickPos.x = clickPos.x - mapPosition.x + maplength / 2;
            clickPos.y = clickPos.y - mapPosition.y + mapwidth / 2;
            // 转换为相对地图左上角的图块单位坐标
            int tileX = static_cast<int>(clickPos.x / (ScaleFactor * 16));
            int tileY = mapHeight - 1 - static_cast<int>((clickPos.y) / (ScaleFactor * 16));
            int tileGID = tileLayer->getTileGIDAt(Vec2(tileX, tileY));
            //CCLOG("Tile GID at (tileX: %d, tileY: %d) is %d", tileX, tileY, tileGID);
            if (tileLayer) {

                if (tileGID == MaturePlantID) {//成熟作物瓦片id，判断是否可收取

                    tileLayer->setTileGID(AbleHoeID, Vec2(tileX, tileY));//替换成原始图块
                    wheat_number++;
                    experience += GainExp;
                    //CCLOG("Tile GID at (tileX: %d, tileY: %d) is %d", tileX, tileY, tileGID);
                }
                //瓦片左下角为锚点
                if (tileGID == MaturePlantTwoID) {//成熟作物瓦片id，判断是否可收取

                    tileLayer->setTileGID(AbleHoeID, Vec2(tileX, tileY));//替换成原始图块
                    carrot_number++;
                    experience += GainExp;
                    //CCLOG("Tile GID at (tileX: %d, tileY: %d) is %d", tileX, tileY, tileGID);
                }
                //瓦片左下角为锚点
            }
        }
        };
    // 将监听器添加到事件分配器
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);
}