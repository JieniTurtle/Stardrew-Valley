#include "Wood.h"
#include "SimpleAudioEngine.h"

bool Wood::init(TMXTiledMap* map) {
    iswood = 0;
   
    visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸

    mapWidth = map->getMapSize().width;  // 横向瓷砖数量
    mapHeight = map->getMapSize().height; // 纵向瓷砖数量
    tileWidth = map->getTileSize().width * ScaleFactor; // 单个瓷砖的像素宽度
    tileHeight = map->getTileSize().height * ScaleFactor; // 单个瓷砖的像素高度
    maplength = mapWidth * tileWidth;
    mapwidth = mapHeight * tileHeight;
    setwoodcheckbox();//设置锄头复选框
    showwoodnum();//显示种子数量
    //wheatListenerMouse(map);
    return true;
}

void  Wood::setwoodcheckbox() {
    auto visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸
    //创建锄头图标
    woodcheckbox = ui::CheckBox::create("wood1.png", "wood2.png");
    woodcheckbox->setPosition(Vec2(visibleSize.width * 9 / 10, visibleSize.height * 6 / 10)); // 设置位置

    this->addChild(woodcheckbox);


}

Wood* Wood::create(TMXTiledMap* map) {

    Wood* ret = new Wood();
    if (ret && ret->init(map)) {
        ret->autorelease(); // 自动释放内存
        return ret;
    }
    CC_SAFE_DELETE(ret); // 如果创建失败，安全删除
    return nullptr;
}



void Wood::showwoodnum() {
    // 创建标签并添加到场景中
    auto label = Label::createWithTTF(std::to_string(wood_number), "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(visibleSize.width * 9 / 10, visibleSize.height * 5.5 / 10)); // 设置标签显示的位置
    this->addChild(label, 1);

    // 创建一个定时器或调度器，以便在每帧更新标签
    this->schedule([this, label](float dt) {
        label->setString(std::to_string(wood_number));
        }, "update_label_key");
}