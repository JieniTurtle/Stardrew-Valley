

#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}


static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸
    Vec2 origin = Director::getInstance()->getVisibleOrigin();//这行代码获取当前游戏视图窗口的原点坐标

   
   
    // 添加初始背景图
    auto map = TMXTiledMap::create("Farm_Ranching.tmx");
    if (!map) {
        problemLoading("'Farm_FourCorners.tmx'");
        return false;
    }
    if (!map) {
        CCLOG("map fail");
        return false;
    }
    this->addChild(map, 0);
    map->setScale(2);//地图扩大两倍
 
    // 获取地图的大小
    mapWidth = map->getMapSize().width;  // 横向瓷砖数量
    mapHeight = map->getMapSize().height; // 纵向瓷砖数量
    tileWidth = map->getTileSize().width*2; // 单个瓷砖的像素宽度
    tileHeight = map->getTileSize().height*2; // 单个瓷砖的像素高度
    map->setAnchorPoint(Vec2(0.5,0.5));
    map->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

 
    //创建主角
    character = MainCharacter::create("characterstatic.png");
    character->setScale(0.5);
    character->setPosition(visibleSize.width / 2, visibleSize.height / 2); // 初始位置
    this->addChild(character);
    character->setMap(map);//传入地图

    auto tools = Tools::create();//传入工具
    this->addChild(tools, 1);
}



