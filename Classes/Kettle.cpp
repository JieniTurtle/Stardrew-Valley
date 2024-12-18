#include "Kettle.h"

#include "SimpleAudioEngine.h"

bool Kettle::init(TMXTiledMap* map) {
    iskettle = 0;
    visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸

    mapWidth = map->getMapSize().width;  // 横向瓷砖数量
    mapHeight = map->getMapSize().height; // 纵向瓷砖数量
    tileWidth = map->getTileSize().width * ScaleFactor; // 单个瓷砖的像素宽度
    tileHeight = map->getTileSize().height * ScaleFactor; // 单个瓷砖的像素高度
    maplength = mapWidth * tileWidth;
    mapwidth = mapHeight * tileHeight;
    setkettlecheckbox();//设置锄头复选框

    kettleListenerMouse(map);
    return true;
}

void  Kettle::setkettlecheckbox() {
    auto visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸
    //创建锄头图标
    kettlecheckbox = ui::CheckBox::create("kettle1.png", "kettle2.png");
    kettlecheckbox->setPosition(Vec2(visibleSize.width*2.5 / 8, visibleSize.height / 4)); // 设置位置

    this->addChild(kettlecheckbox);


}

Kettle* Kettle::create(TMXTiledMap* map) {

    Kettle* ret = new Kettle();
    if (ret && ret->init(map)) {
        ret->autorelease(); // 自动释放内存
        return ret;
    }
    CC_SAFE_DELETE(ret); // 如果创建失败，安全删除
    return nullptr;
}

void Kettle::kettleListenerMouse(TMXTiledMap* map) {
    // 创建鼠标事件监听器
    auto mouseListener = EventListenerMouse::create();

    mouseListener->onMouseDown = [=](Event* event) {
        EventMouse* mouseEvent = static_cast<EventMouse*>(event);
        Vec2 mapPosition = map->getPosition();
        if (iskettle == 1) {
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

                if (tileGID == RawPlantNoWaterID) {//没有浇水的发芽id，判断是否可浇水
                    
                    tileLayer->setTileGID(RawPlantWaterID, Vec2(tileX, tileY));//替换成浇过水完的图块
                    // 使用调度器在两秒后更改瓦片ID
                    auto callback = [this, tileX, tileY, tileLayer]() {
                        tileLayer->setTileGID(MaturePlantID, Vec2(tileX, tileY)); // 替换成成熟状态的图块
                        };
                    Action* rebackaction = Sequence::create(DelayTime::create(MatureTime), CallFunc::create(callback), nullptr);
                    this->runAction(rebackaction);
                    // 调度器延迟执行，n秒长成熟
                    
                    if (scheduleCounter > 1000000) {
                        scheduleCounter = 0;
                    }//防止scheduleCounter溢出
                    std::string updateKey = "update_function_key_" + std::to_string(scheduleCounter++);
                    this->schedule([=](float dt) {
                        myUpdateFunction(rebackaction, tileLayer, tileX, tileY, updateKey); // 调用更新函数
                        }, 0.1f, updateKey);
                }
                //瓦片左下角为锚点
                if (tileGID == RawPlantNoWaterTwoID) {//没有浇水的发芽id，判断是否可浇水

                    tileLayer->setTileGID(RawPlantWaterTwoID, Vec2(tileX, tileY));//替换成浇过水完的图块
                    // 使用调度器在两秒后更改瓦片ID
                    auto callback = [this, tileX, tileY, tileLayer]() {
                        tileLayer->setTileGID(MaturePlantTwoID, Vec2(tileX, tileY)); // 替换成成熟状态的图块
                        };
                    Action* rebackaction = Sequence::create(DelayTime::create(MatureTime), CallFunc::create(callback), nullptr);
                    this->runAction(rebackaction);
                    // 调度器延迟执行，n秒长成熟

                    if (scheduleCounter > 1000000) {
                        scheduleCounter = 0;
                    }//防止scheduleCounter溢出
                    std::string updateKey = "update_function_key_" + std::to_string(scheduleCounter++);
                    this->schedule([=](float dt) {
                        myUpdateFunction(rebackaction, tileLayer, tileX, tileY, updateKey); // 调用更新函数
                        }, 0.1f, updateKey);
                }
                //瓦片左下角为锚点
            }
        }
        };
    // 将监听器添加到事件分配器
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void Kettle::myUpdateFunction(Action* rebackaction, TMXLayer* tileLayer, int tileX, int tileY, std::string updateKey) {
    if (rebackaction) {
        if ((tileLayer->getTileGIDAt(Vec2(tileX, tileY)) != RawPlantWaterID)&& (tileLayer->getTileGIDAt(Vec2(tileX, tileY)) != RawPlantWaterTwoID)) {
            this->stopAction(rebackaction);
            this->unschedule(updateKey);
        }
    }
}