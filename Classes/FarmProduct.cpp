#include "FarmProduct.h"



#include "SimpleAudioEngine.h"

bool FarmProduct::init(TMXTiledMap* map) {

    visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸

    mapWidth = map->getMapSize().width;  // 横向瓷砖数量
    mapHeight = map->getMapSize().height; // 纵向瓷砖数量
    tileWidth = map->getTileSize().width * ScaleFactor; // 单个瓷砖的像素宽度
    tileHeight = map->getTileSize().height * ScaleFactor; // 单个瓷砖的像素高度
    maplength = mapWidth * tileWidth;
    mapwidth = mapHeight * tileHeight;
    setcheckbox();//设置复选框
    showmilknum();//显示数量
    showeggnum();
    showwoolnum();
    showfishnum();
    return true;
}

void  FarmProduct::setcheckbox() {
    auto visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸
    //创建图标
    milkcheckbox = ui::CheckBox::create("milk1.png", "milk1.png");
    milkcheckbox->setPosition(Vec2(visibleSize.width * 9 / 10, visibleSize.height*4 / 10)); // 设置位置

    this->addChild(milkcheckbox);
    //创建图标
    woolcheckbox = ui::CheckBox::create("wool1.png", "wool1.png");
    woolcheckbox->setPosition(Vec2(visibleSize.width * 9 / 10, visibleSize.height*3 / 10)); // 设置位置

    this->addChild(woolcheckbox);
    //创建图标
    eggcheckbox = ui::CheckBox::create("egg1.png", "egg1.png");
    eggcheckbox->setPosition(Vec2(visibleSize.width * 9 / 10, visibleSize.height*2 / 10)); // 设置位置

    this->addChild(eggcheckbox);
    //创建图标
    fishcheckbox = ui::CheckBox::create("fish1.png", "fish1.png");
    fishcheckbox->setPosition(Vec2(visibleSize.width * 9/ 10, visibleSize.height * 1 / 10)); // 设置位置

    this->addChild(fishcheckbox);
}

FarmProduct* FarmProduct::create(TMXTiledMap* map) {

    FarmProduct* ret = new FarmProduct();
    if (ret && ret->init(map)) {
        ret->autorelease(); // 自动释放内存
        return ret;
    }
    CC_SAFE_DELETE(ret); // 如果创建失败，安全删除
    return nullptr;
}

void FarmProduct::showmilknum() {
    // 创建标签并添加到场景中
    auto label = Label::createWithTTF(std::to_string(milk_number), "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(visibleSize.width * 9 / 10, visibleSize.height * 3.5 / 10)); // 设置标签显示的位置
    this->addChild(label, 1);

    // 创建一个定时器或调度器，以便在每帧更新标签
    this->schedule([this, label](float dt) {
        label->setString(std::to_string(milk_number));
        }, "update_label_key");
}

void FarmProduct::showeggnum() {
    // 创建标签并添加到场景中
    auto label = Label::createWithTTF(std::to_string(egg_number), "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(visibleSize.width * 9 / 10, visibleSize.height * 1.5 / 10)); // 设置标签显示的位置
    this->addChild(label, 1);

    // 创建一个定时器或调度器，以便在每帧更新标签
    this->schedule([this, label](float dt) {
        label->setString(std::to_string(egg_number));
        }, "update_label_key1");
}

void FarmProduct::showwoolnum() {
    // 创建标签并添加到场景中
    auto label = Label::createWithTTF(std::to_string(wool_number), "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(visibleSize.width * 9 / 10, visibleSize.height * 2.5 / 10)); // 设置标签显示的位置
    this->addChild(label, 1);

    // 创建一个定时器或调度器，以便在每帧更新标签
    this->schedule([this, label](float dt) {
        label->setString(std::to_string(wool_number));
        }, "update_label_key2");
}

void FarmProduct::showfishnum() {
    // 创建标签并添加到场景中
    auto label = Label::createWithTTF(std::to_string(fish_number), "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(visibleSize.width * 9 / 10, visibleSize.height * 0.5 / 10)); // 设置标签显示的位置
    this->addChild(label, 1);

    // 创建一个定时器或调度器，以便在每帧更新标签
    this->schedule([this, label](float dt) {
        label->setString(std::to_string(fish_number));
        }, "update_label_key3");
}
