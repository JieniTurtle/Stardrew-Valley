#include "Fertilizer.h"
#include "SimpleAudioEngine.h"

bool Fertilizer::init(TMXTiledMap* map) {
    isfertilizer = 0;
   
    visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸

    mapWidth = map->getMapSize().width;  // 横向瓷砖数量
    mapHeight = map->getMapSize().height; // 纵向瓷砖数量
    tileWidth = map->getTileSize().width * ScaleFactor; // 单个瓷砖的像素宽度
    tileHeight = map->getTileSize().height * ScaleFactor; // 单个瓷砖的像素高度
    maplength = mapWidth * tileWidth;
    mapwidth = mapHeight * tileHeight;
    setfertilizercheckbox();//设置锄头复选框
    showfertilizernum();//显示种子数量
    fertilizerListenerMouse(map);
    return true;
}

void  Fertilizer::setfertilizercheckbox() {
    auto visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸
    //创建锄头图标
    fertilizercheckbox = ui::CheckBox::create("fertilizer1.png", "fertilizer2.png");
    fertilizercheckbox->setPosition(Vec2(visibleSize.width * 9 / 10, visibleSize.height * 5 / 10)); // 设置位置

    this->addChild(fertilizercheckbox);


}

Fertilizer* Fertilizer::create(TMXTiledMap* map) {

    Fertilizer* ret = new Fertilizer();
    if (ret && ret->init(map)) {
        ret->autorelease(); // 自动释放内存
        return ret;
    }
    CC_SAFE_DELETE(ret); // 如果创建失败，安全删除
    return nullptr;
}

void Fertilizer::fertilizerListenerMouse(TMXTiledMap* map) {
    // 创建鼠标事件监听器
    auto mouseListener = EventListenerMouse::create();
   
    mouseListener->onMouseDown = [=](Event* event) {
        EventMouse* mouseEvent = static_cast<EventMouse*>(event);
        Vec2 mapPosition = map->getPosition();
        if (isfertilizer == 1) {
        
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

                if ((tileGID == RawPlantWaterID) && fertilizer_number > 0) {//有浇水的发芽id，判断是否可用肥料
                   
                    tileLayer->setTileGID(MaturePlantID, Vec2(tileX, tileY));
                    // 使用完即刻长大
 
                    fertilizer_number--;//施肥成功，肥料数量减少
                }
                //瓦片左下角为锚点
                if ((tileGID == RawPlantWaterTwoID) && fertilizer_number > 0) {//有浇水的发芽id，判断是否可用肥料

                    tileLayer->setTileGID(MaturePlantTwoID, Vec2(tileX, tileY));
                    // 使用完即刻长大
                
                    fertilizer_number--;//施肥成功，肥料数量减少
                }
            }
        }
        };
    // 将监听器添加到事件分配器
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void Fertilizer::showfertilizernum() {
    // 创建标签并添加到场景中
    auto label = Label::createWithTTF(std::to_string(fertilizer_number), "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(visibleSize.width * 9 / 10, visibleSize.height * 4.5 / 10)); // 设置标签显示的位置
    this->addChild(label, 1);

    // 创建一个定时器或调度器，以便在每帧更新标签
    this->schedule([this, label](float dt) {
        label->setString(std::to_string(fertilizer_number));
        }, "update_label_key");
}



