#include "Mineral.h"

#include "SimpleAudioEngine.h"

bool Mineral::init(TMXTiledMap* map) {
   

    visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸

    mapWidth = map->getMapSize().width;  // 横向瓷砖数量
    mapHeight = map->getMapSize().height; // 纵向瓷砖数量
    tileWidth = map->getTileSize().width * ScaleFactor; // 单个瓷砖的像素宽度
    tileHeight = map->getTileSize().height * ScaleFactor; // 单个瓷砖的像素高度
    maplength = mapWidth * tileWidth;
    mapwidth = mapHeight * tileHeight;
    setcheckbox();//设置复选框
    showstonenum();//显示数量
    showgemnum();

    return true;
}

void  Mineral::setcheckbox() {
    auto visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸
    //创建图标
    stonecheckbox = ui::CheckBox::create("stone.png", "stone.png");
    stonecheckbox->setPosition(Vec2(visibleSize.width * 9.7 / 10, visibleSize.height * 4 / 10)); // 设置位置

    this->addChild(stonecheckbox);
    //创建图标
   gemcheckbox = ui::CheckBox::create("gem.png", "gem.png");
   gemcheckbox->setPosition(Vec2(visibleSize.width * 9.7 / 10, visibleSize.height * 3 / 10)); // 设置位置

    this->addChild(gemcheckbox);
 
}

Mineral* Mineral::create(TMXTiledMap* map) {

    Mineral* ret = new Mineral();
    if (ret && ret->init(map)) {
        ret->autorelease(); // 自动释放内存
        return ret;
    }
    CC_SAFE_DELETE(ret); // 如果创建失败，安全删除
    return nullptr;
}

void Mineral::showstonenum() {
    // 创建标签并添加到场景中
    auto label = Label::createWithTTF(std::to_string(stone_number), "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(visibleSize.width * 9.7 / 10, visibleSize.height * 3.5 / 10)); // 设置标签显示的位置
    this->addChild(label, 1);

    // 创建一个定时器或调度器，以便在每帧更新标签
    this->schedule([this, label](float dt) {
        label->setString(std::to_string(stone_number));
        }, "update_label_key");
}

void Mineral::showgemnum() {
    // 创建标签并添加到场景中
    auto label = Label::createWithTTF(std::to_string(mineral_number), "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(visibleSize.width * 9.7 / 10, visibleSize.height * 2.5 / 10)); // 设置标签显示的位置
    this->addChild(label, 1);

    // 创建一个定时器或调度器，以便在每帧更新标签
    this->schedule([this, label](float dt) {
        label->setString(std::to_string(mineral_number));
        }, "update_label_key1");
}


