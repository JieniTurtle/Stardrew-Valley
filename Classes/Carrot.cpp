#include "Carrot.h"
#include "SimpleAudioEngine.h"

bool Carrot::init(TMXTiledMap* map) {
    iscarrot = 0;
 
    visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸

    mapWidth = map->getMapSize().width;  // 横向瓷砖数量
    mapHeight = map->getMapSize().height; // 纵向瓷砖数量
    tileWidth = map->getTileSize().width * ScaleFactor; // 单个瓷砖的像素宽度
    tileHeight = map->getTileSize().height * ScaleFactor; // 单个瓷砖的像素高度
    maplength = mapWidth * tileWidth;
    mapwidth = mapHeight * tileHeight;
    setcarrotcheckbox();//设置锄头复选框
    showcarrotnum();//显示种子数量
    //wheatListenerMouse(map);
    return true;
}

void  Carrot::setcarrotcheckbox() {
    auto visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸
    //创建锄头图标
    carrotcheckbox = ui::CheckBox::create("carrot1.png", "carrot2.png");
    carrotcheckbox->setPosition(Vec2(visibleSize.width * 9.7 / 10, visibleSize.height * 7 / 10)); // 设置位置

    //// 添加事件监听器
    //wheatcheckbox->addEventListener([this](Ref* sender, ui::CheckBox::EventType type) {
    //    switch (type) {
    //        case ui::CheckBox::EventType::SELECTED:
    //            iswheat = 1;
    //            break;
    //        case ui::CheckBox::EventType::UNSELECTED:
    //            iswheat = 0;
    //            break;
    //        default:
    //            break;
    //    }
    //    });
    this->addChild(carrotcheckbox);


}

Carrot* Carrot::create(TMXTiledMap* map) {

    Carrot* ret = new Carrot();
    if (ret && ret->init(map)) {
        ret->autorelease(); // 自动释放内存
        return ret;
    }
    CC_SAFE_DELETE(ret); // 如果创建失败，安全删除
    return nullptr;
}

//void Wheat::wheatListenerMouse(TMXTiledMap* map) {
//    // 创建鼠标事件监听器
//    auto mouseListener = EventListenerMouse::create();
//
//    mouseListener->onMouseDown = [=](Event* event) {
//        EventMouse* mouseEvent = static_cast<EventMouse*>(event);
//        Vec2 mapPosition = map->getPosition();
//        if (iswheat == 1) {
//            // 获取鼠标点击的位置
//            Vec2 clickPos = mouseEvent->getLocation();//以左上角为原点
//            clickPos.y = visibleSize.height - clickPos.y;//转化为左下角为原点
//            auto tileLayer = map->getLayer("Back");
//            //坐标转化为相对地图左下角的
//            clickPos.x = clickPos.x - mapPosition.x + maplength / 2;
//            clickPos.y = clickPos.y - mapPosition.y + mapwidth / 2;
//            // 转换为相对地图左上角的图块单位坐标
//            int tileX = static_cast<int>(clickPos.x / (ScaleFactor * 16));
//            int tileY = mapHeight - 1 - static_cast<int>((clickPos.y) / (ScaleFactor * 16));
//            int tileGID = tileLayer->getTileGIDAt(Vec2(tileX, tileY));
//            //CCLOG("Tile GID at (tileX: %d, tileY: %d) is %d", tileX, tileY, tileGID);
//            if (tileLayer) {
//
//                if ((tileGID == 8000) && seedsnum > 0) {//已经耕田的瓦片id，判断是否可种植
//
//                    tileLayer->setTileGID(4000, Vec2(tileX, tileY));//替换成种植后的图块，初始状态
//                    seedsnum--;//种植成功，种子数量减少
//                    // 使用调度器在两秒后更改瓦片ID
//                    auto callback = [this, tileX, tileY, tileLayer]() {
//                        tileLayer->setTileGID(5000, Vec2(tileX, tileY)); // 替换成成熟状态的图块
//                        };
//
//                    // 调度器延迟执行，2秒长成熟
//                    this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create(callback), nullptr));
//                    //CCLOG("Tile GID at (tileX: %d, tileY: %d) is %d", tileX, tileY, tileGID);
//                }
//                //瓦片左下角为锚点
//
//            }
//        }
//        };
//    // 将监听器添加到事件分配器
//    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);
//}

void Carrot::showcarrotnum() {
    // 创建标签并添加到场景中
    auto label = Label::createWithTTF(std::to_string(carrot_number), "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(visibleSize.width * 9.7 / 10, visibleSize.height * 6.5 / 10)); // 设置标签显示的位置
    this->addChild(label, 1);

    // 创建一个定时器或调度器，以便在每帧更新标签
    this->schedule([this, label](float dt) {
        label->setString(std::to_string(carrot_number));
        }, "update_label_key");
}