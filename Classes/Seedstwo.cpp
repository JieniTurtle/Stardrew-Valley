#include "Seedstwo.h"
#include "SimpleAudioEngine.h"

bool Seedstwo::init(TMXTiledMap* map) {
    isseedstwo = 0;
    seedstwonum = 10;
    scheduleCounter = 0;
    visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸

    mapWidth = map->getMapSize().width;  // 横向瓷砖数量
    mapHeight = map->getMapSize().height; // 纵向瓷砖数量
    tileWidth = map->getTileSize().width * ScaleFactor; // 单个瓷砖的像素宽度
    tileHeight = map->getTileSize().height * ScaleFactor; // 单个瓷砖的像素高度
    maplength = mapWidth * tileWidth;
    mapwidth = mapHeight * tileHeight;
    setseedstwocheckbox();//设置锄头复选框
    showseedstwonum();//显示种子数量
    seedstwoListenerMouse(map);
    return true;
}

void  Seedstwo::setseedstwocheckbox() {
    auto visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸
    //创建seedstwo图标
    seedstwocheckbox = ui::CheckBox::create("seedstwo1.png", "seedstwo2.png");
    seedstwocheckbox->setPosition(Vec2(visibleSize.width * 9.7 / 10, visibleSize.height * 8 / 10)); // 设置位置


    this->addChild(seedstwocheckbox);


}

Seedstwo* Seedstwo::create(TMXTiledMap* map) {

    Seedstwo* ret = new Seedstwo();
    if (ret && ret->init(map)) {
        ret->autorelease(); // 自动释放内存
        return ret;
    }
    CC_SAFE_DELETE(ret); // 如果创建失败，安全删除
    return nullptr;
}

void Seedstwo::seedstwoListenerMouse(TMXTiledMap* map) {
    // 创建鼠标事件监听器
    auto mouseListener = EventListenerMouse::create();

    mouseListener->onMouseDown = [=](Event* event) {
        EventMouse* mouseEvent = static_cast<EventMouse*>(event);
        Vec2 mapPosition = map->getPosition();
        if (isseedstwo == 1) {
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

                if ((tileGID == HoeOverID) && seedstwonum > 0) {//已经耕田的瓦片id，判断是否可种植

                    tileLayer->setTileGID(RawPlantID, Vec2(tileX, tileY));//替换成种植后的图块，初始状态
                    seedstwonum--;//种植成功，种子数量减少
                    // 使用调度器在n秒后更改瓦片ID
                    auto callback = [this, tileX, tileY, tileLayer]() {
                        tileLayer->setTileGID(AbleHoeID, Vec2(tileX, tileY)); // 默认没浇水，替换成可耕地的图块
                        };

                    // 调度器延迟执行，n秒后没交水复原
                    Action* rebackaction = Sequence::create(DelayTime::create(NoWaterDieTime), CallFunc::create(callback), nullptr);
                    this->runAction(rebackaction);
                    if (scheduleCounter > 1000000) {
                        scheduleCounter = 0;
                    }//防止scheduleCounter溢出
                    std::string updateKey = "update_function_key_" + std::to_string(scheduleCounter++);
                    this->schedule([=](float dt) {
                        myUpdateFunction(rebackaction, tileLayer, tileX, tileY, updateKey); // 调用更新函数
                        }, 0.1f, updateKey);
                    //CCLOG("Tile GID at (tileX: %d, tileY: %d) is %d", tileX, tileY, tileGID);
                }
                //瓦片左下角为锚点

            }
        }
        };
    // 将监听器添加到事件分配器
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void Seedstwo::showseedstwonum() {
    // 创建标签并添加到场景中
    auto label = Label::createWithTTF(std::to_string(seedstwonum), "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(visibleSize.width * 9.7 / 10, visibleSize.height * 7.5 / 10)); // 设置标签显示的位置
    this->addChild(label, 1);

    // 创建一个定时器或调度器，以便在每帧更新标签
    this->schedule([this, label](float dt) {
        label->setString(std::to_string(seedstwonum));
        }, "update_label_key");
}
//检测是否取消土地复原操作，浇过水后取消该动作

void Seedstwo::myUpdateFunction(Action* rebackaction, TMXLayer* tileLayer, int tileX, int tileY, std::string updateKey) {
    if (rebackaction) {
        if (tileLayer->getTileGIDAt(Vec2(tileX, tileY)) != RawPlantID) {
            this->stopAction(rebackaction);
            this->unschedule(updateKey);
        }
    }
}