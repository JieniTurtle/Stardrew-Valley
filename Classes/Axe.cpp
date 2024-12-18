#include "Axe.h"

#include "SimpleAudioEngine.h"

bool Axe::init(TMXTiledMap* map, int& woodnum) {
    isaxe = 0;
    visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸

    mapWidth = map->getMapSize().width;  // 横向瓷砖数量
    mapHeight = map->getMapSize().height; // 纵向瓷砖数量
    tileWidth = map->getTileSize().width * ScaleFactor; // 单个瓷砖的像素宽度
    tileHeight = map->getTileSize().height * ScaleFactor; // 单个瓷砖的像素高度
    maplength = mapWidth * tileWidth;
    mapwidth = mapHeight * tileHeight;
    setaxecheckbox();//设置锄头复选框

    axeListenerMouse(map,woodnum);
    return true;
}

void  Axe::setaxecheckbox() {
    auto visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸
    //创建锄头图标
    axecheckbox = ui::CheckBox::create("axe1.png", "axe2.png");
    axecheckbox->setPosition(Vec2(visibleSize.width*2 / 8, visibleSize.height / 4)); // 设置位置
    this->addChild(axecheckbox);


}

Axe* Axe::create(TMXTiledMap* map, int& woodnum) {

    Axe* ret = new Axe();
    if (ret && ret->init(map,woodnum)) {
        ret->autorelease(); // 自动释放内存
        return ret;
    }
    CC_SAFE_DELETE(ret); // 如果创建失败，安全删除
    return nullptr;
}

void Axe::axeListenerMouse(TMXTiledMap* map, int& woodnum) {
    // 创建鼠标事件监听器
    auto mouseListener = EventListenerMouse::create();

    mouseListener->onMouseDown = [=,&woodnum](Event* event) {
        EventMouse* mouseEvent = static_cast<EventMouse*>(event);
        Vec2 mapPosition = map->getPosition();
        if (isaxe == 1) {
            // 获取鼠标点击的位置
            Vec2 clickPos = mouseEvent->getLocation();//以左上角为原点
            clickPos.y = visibleSize.height - clickPos.y;//转化为左下角为原点
            //坐标转化为相对地图左下角的
            clickPos.x = clickPos.x - mapPosition.x + maplength / 2;
            clickPos.y = clickPos.y - mapPosition.y + mapwidth / 2;
            // 转换为相对地图左上角的图块单位坐标
            int tileX = static_cast<int>(clickPos.x / (ScaleFactor * 16));
            int tileY = mapHeight - 1 - static_cast<int>((clickPos.y) / (ScaleFactor * 16));
          //遍历每一个树图层
            for (int i = 1; i <= numberOfTree; ++i) {
                // 构造图层名称
                std::string layerName = "Tree" + std::to_string(i); // 生成 "Tree1", "Tree2", ...

                // 获取树的图层
                auto treeLayer = map->getLayer(layerName);

                // 检查图层是否有效
                if (treeLayer) {
                    int tileGID = treeLayer->getTileGIDAt(Vec2(tileX, tileY));//点击处是否有树
                    if (tileGID != 0) {
                        map->removeChild(treeLayer, true);//移除该图层，表示砍掉了
                        woodnum += OneTreeForWood;
                    }
                    
                }

            }
          
            
            
        }
        };
    // 将监听器添加到事件分配器
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);
}