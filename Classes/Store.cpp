#include "Store.h"



#include "SimpleAudioEngine.h"

bool Store::init(TMXTiledMap* map, int& wheatnum, int& milknum, int& woolnum, int& eggnum, int& seedsnum, int& fertilzernum, int& seedstwonum, int& carrotnum) {

    visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸

    mapWidth = map->getMapSize().width;  // 横向瓷砖数量
    mapHeight = map->getMapSize().height; // 纵向瓷砖数量
    tileWidth = map->getTileSize().width * ScaleFactor; // 单个瓷砖的像素宽度
    tileHeight = map->getTileSize().height * ScaleFactor; // 单个瓷砖的像素高度
    maplength = mapWidth * tileWidth;
    mapwidth = mapHeight * tileHeight;
    setstorescene(wheatnum, milknum, woolnum, eggnum, seedsnum, fertilzernum,  seedstwonum,carrotnum);
    setopenitem();
    
    return true;
}

Store* Store::create(TMXTiledMap* map, int& wheatnum, int& milknum, int& woolnum, int& eggnum, int& seedsnum, int& fertilzernum, int& seedstwonum, int& carrotnum) {

    Store* ret = new Store();
    if (ret && ret->init(map, wheatnum,  milknum,  woolnum, eggnum,  seedsnum,  fertilzernum,  seedstwonum, carrotnum)) {
        ret->autorelease(); // 自动释放内存
        return ret;
    }
    CC_SAFE_DELETE(ret); // 如果创建失败，安全删除
    return nullptr;
}
void Store::setopenitem() {
    // 创建一个按钮
     openItem = MenuItemImage::create(
        "storeopen.png",
        "storeopen.png",
        CC_CALLBACK_0(Store::menuOpenCallback, this));

    openItem->setPosition(Vec2( visibleSize.width*7/10,
        visibleSize.height* 9 / 10));

    // 创建一个菜单
    auto menu = Menu::create(openItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
}
//按了商店按钮
void Store::menuOpenCallback() {
    //场景内容设为可见
    for (auto& child : StoreScene->getChildren())
    {
        child->setVisible(true);
    }
}
//设置商店界面
void Store::setstorescene(int& wheatnum,  int& milknum, int& woolnum, int& eggnum, int& seedsnum, int& fertilzernum, int& seedstwonum, int& carrotnum) {
    StoreScene =Layer::create();
    emptystoragesprite = Sprite::create("emptystorage.png");
    emptystoragesprite->setPosition(Vec2(visibleSize.width / 2,
        visibleSize.height / 2));
    emptystoragesprite->setScale(0.8);
    StoreScene -> addChild(emptystoragesprite, 0);
    //
    closeItem = MenuItemImage::create(
        "closeitem.png",
        "closeitem.png",
        CC_CALLBACK_0(Store::CloseCallback, this));

    closeItem->setPosition(Vec2(visibleSize.width * 7 / 10,
        visibleSize.height * 9 / 10));

    // 创建一个菜单
    closemenu = Menu::create(closeItem, NULL);
    closemenu->setPosition(Vec2::ZERO);
    StoreScene->addChild(closemenu, 4);
    


    setsell(wheatnum, milknum, woolnum, eggnum, seedsnum, fertilzernum,  seedstwonum, carrotnum);
    setbuy(wheatnum, milknum, woolnum, eggnum, seedsnum, fertilzernum, seedstwonum, carrotnum);
    this->addChild(StoreScene, 2);
    //初始为不可见
    for (auto& child : StoreScene->getChildren())
    {
        child->setVisible(false);
    }
}
void Store::CloseCallback() {
    for (auto& child : StoreScene->getChildren())
    {
        child->setVisible(false);
    }
}
void Store::setsell(int& wheatnum, int& milknum, int& woolnum, int& eggnum, int& seedsnum, int& fertilzernum, int& seedstwonum, int& carrotnum) {
    //小麦按钮
    wheatsellItem = MenuItemImage::create(
        "wheat1.png",
        "wheat2.png",
        [this,&wheatnum](Ref* pSender) {
            if (wheatnum > 0) {
                money += WheatSellPrice;
                wheatnum--;
            }
            else {
                sellfail();
            }
        });

    wheatsellItem->setPosition(Vec2(visibleSize.width * 13/ 40,
        visibleSize.height * 21 / 40));
    //小麦标签
    wheatsellLabel = Label::createWithTTF("SellPrice: " + std::to_string(WheatSellPrice), "fonts/Marker Felt.ttf", 18);
    wheatsellLabel->setPosition(Vec2(visibleSize.width * 13 / 40, visibleSize.height * 19 / 40)); // 设置标签显示的位置
    wheatsellLabel->setColor(Color3B::BLACK);
    StoreScene->addChild(wheatsellLabel, 4);
    //牛奶按钮
    milksellItem = MenuItemImage::create(
        "milk1.png",
        "milk2.png",
        [this, &milknum](Ref* pSender) {
            if (milknum > 0) {
                money += MilkSellPrice;
                milknum--;
            }
            else {
                sellfail();
  
            }
        });

    milksellItem->setPosition(Vec2(visibleSize.width * 16.5 / 40,
        visibleSize.height * 21 / 40));
    //牛奶标签
    milksellLabel = Label::createWithTTF("SellPrice: " + std::to_string(MilkSellPrice), "fonts/Marker Felt.ttf", 18);
    milksellLabel->setPosition(Vec2(visibleSize.width * 16.5 / 40, visibleSize.height * 19 / 40)); // 设置标签显示的位置
    milksellLabel->setColor(Color3B::BLACK);
    StoreScene->addChild(milksellLabel, 4);
    //wool按钮
    woolsellItem = MenuItemImage::create(
        "wool1.png",
        "wool2.png",
        [this, &woolnum](Ref* pSender) {
            if (woolnum > 0) {
                money += WoolSellPrice;
                woolnum--;
            }
            else {
                sellfail();

            }
        });

    woolsellItem->setPosition(Vec2(visibleSize.width * 20 / 40,
        visibleSize.height * 21 / 40));
    ////wool标签
    //woolsellLabel = Label::createWithTTF("SellPrice: " + std::to_string(WoolSellPrice), "fonts/Marker Felt.ttf", 18);
    //woolsellLabel->setPosition(Vec2(visibleSize.width * 20 / 40, visibleSize.height * 19 / 40)); // 设置标签显示的位置
    //woolsellLabel->setColor(Color3B::BLACK);
    //StoreScene->addChild(woolsellLabel, 4);
    //egg按钮
    eggsellItem = MenuItemImage::create(
        "egg1.png",
        "egg2.png",
        [this, &eggnum](Ref* pSender) {
            if (eggnum > 0) {
                money += EggSellPrice;
                eggnum--;
            }
            else {
                sellfail();

            }
        });

    eggsellItem->setPosition(Vec2(visibleSize.width * 23.5 / 40,
        visibleSize.height * 21 / 40));
    //egg标签
    eggsellLabel = Label::createWithTTF("SellPrice: " + std::to_string(EggSellPrice), "fonts/Marker Felt.ttf", 18);
    eggsellLabel->setPosition(Vec2(visibleSize.width * 23.5 / 40, visibleSize.height * 19 / 40)); // 设置标签显示的位置
    eggsellLabel->setColor(Color3B::BLACK);
    StoreScene->addChild(eggsellLabel, 4);
    //carrot按钮
    carrotsellItem = MenuItemImage::create(
        "carrot1.png",
        "carrot2.png",
        [this, &carrotnum](Ref* pSender) {
            if (carrotnum > 0) {
                money += CarrotSellPrice;
                carrotnum--;
            }
            else {
                sellfail();
            }
        });

    carrotsellItem->setPosition(Vec2(visibleSize.width *27 / 40,
        visibleSize.height * 21 / 40));
    //carrot标签
    carrotsellLabel = Label::createWithTTF("SellPrice: " + std::to_string(CarrotSellPrice), "fonts/Marker Felt.ttf", 18);
    carrotsellLabel->setPosition(Vec2(visibleSize.width * 27 / 40, visibleSize.height * 19 / 40)); // 设置标签显示的位置
    carrotsellLabel->setColor(Color3B::BLACK);
    StoreScene->addChild(carrotsellLabel, 4);
    // 创建一个菜单
    auto menu = Menu::create(wheatsellItem, milksellItem, woolsellItem, eggsellItem, carrotsellItem, NULL);
    menu->setPosition(Vec2::ZERO);
    StoreScene->addChild(menu, 3);
}
void Store::setbuy(int& wheatnum, int& milknum, int& woolnum, int& eggnum, int& seedsnum, int& fertilzernum, int& seedstwonum, int& carrotnum)
{
    //按钮
    seedstwobuyItem = MenuItemImage::create(
        "seedstwo1.png",
        "seedstwo2.png",
        [this, &seedstwonum](Ref* pSender) {
            if (money >= SeedsTwoBuyPrice) {
                money -= SeedsTwoBuyPrice;
                seedstwonum++;
            }
            else {
                buyfail();
            }
        });

    seedstwobuyItem->setPosition(Vec2(visibleSize.width * 16.5 / 40,
        visibleSize.height * 16 / 40));
    //标签
    seedstwobuyLabel = Label::createWithTTF("BuyPrice: " + std::to_string(SeedsTwoBuyPrice), "fonts/Marker Felt.ttf", 18);
    seedstwobuyLabel->setPosition(Vec2(visibleSize.width * 16.5 / 40, visibleSize.height * 14 / 40)); // 设置标签显示的位置
    seedstwobuyLabel->setColor(Color3B::BLACK);
    StoreScene->addChild(seedstwobuyLabel, 4);
    //按钮
    seedsbuyItem = MenuItemImage::create(
        "seeds1.png",
        "seeds2.png",
        [this, &seedsnum](Ref* pSender) {
            if (money >= SeedsBuyPrice) {
                money -= SeedsBuyPrice;
                seedsnum++;
            }
            else {
                buyfail();
            }
        });

    seedsbuyItem->setPosition(Vec2(visibleSize.width * 13 / 40,
        visibleSize.height * 16 / 40));
    //标签
    seedsbuyLabel = Label::createWithTTF("BuyPrice: " + std::to_string(SeedsBuyPrice), "fonts/Marker Felt.ttf", 18);
    seedsbuyLabel->setPosition(Vec2(visibleSize.width * 13 / 40, visibleSize.height * 14 / 40)); // 设置标签显示的位置
    seedsbuyLabel->setColor(Color3B::BLACK);
    StoreScene->addChild(seedsbuyLabel, 4);
    // 创建一个菜单
    auto menu = Menu::create(seedsbuyItem, seedstwobuyItem, NULL);
    menu->setPosition(Vec2::ZERO);
    StoreScene->addChild(menu, 3);
}
void Store::sellfail() {
    auto label = Label::createWithSystemFont("You don't have enough product", "Arial", 24);
    label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height/ 2));
    auto sprite = Sprite::create("buysellfailscene.png");//背景
    sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    sprite->setScale(1.2);
    StoreScene->addChild(label,6);
    StoreScene->addChild(sprite, 5);
    label->setVisible(true);

    label->setColor(Color3B::BLUE);

    auto callback = [=]() {
        StoreScene->removeChild(label);
        StoreScene->removeChild(sprite);
        };
    Action* rebackaction = Sequence::create(DelayTime::create(FailSellDisplayTime), CallFunc::create(callback), nullptr);
    this->runAction(rebackaction);
    // 调度器延迟执行，n秒长成熟

 
}
void Store::buyfail() {
    auto label = Label::createWithSystemFont("You don't have enough money", "Arial", 24);
    label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    auto sprite = Sprite::create("buysellfailscene.png");//背景
    sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    sprite->setScale(1.2);
    StoreScene->addChild(label, 6);
    StoreScene->addChild(sprite, 5);
    
    label->setVisible(true);

    label->setColor(Color3B::BLUE);

    auto callback = [=]() {
        StoreScene->removeChild(label);
        StoreScene->removeChild(sprite);
        };
    Action* rebackaction = Sequence::create(DelayTime::create(FailSellDisplayTime), CallFunc::create(callback), nullptr);
    this->runAction(rebackaction);
    // 调度器延迟执行，n秒长成熟


}
