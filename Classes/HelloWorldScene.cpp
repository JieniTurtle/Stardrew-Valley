#include "global.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}


static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸
    Vec2 origin = Director::getInstance()->getVisibleOrigin();//这行代码获取当前游戏视图窗口的原点坐标

    //开始按钮
    auto startItem = MenuItemImage::create(
                                           "startgamebutton.png",
                                           "startgamebutton2.png",
                                           CC_CALLBACK_1(HelloWorld::menuStart, this));
    startItem->setScale(0.5);
    float x = origin.x + visibleSize.width*0.4 - startItem->getContentSize().width/4;
    float y = origin.y + visibleSize.height * 0.25+ startItem->getContentSize().height/4;
    startItem->setPosition(Vec2(x,y));
   
    //退出按钮
    auto closeItem = MenuItemImage::create(
        "endbutton.png",
        "endbutton2.png",
        CC_CALLBACK_1(HelloWorld::menuClose, this));
    closeItem->setScale(0.5);
    float xx = origin.x + visibleSize.width * 0.7 - closeItem->getContentSize().width / 4;
    float yy = origin.y + visibleSize.height * 0.25 + closeItem->getContentSize().height / 4;
    closeItem->setPosition(Vec2(xx, yy));

    auto menu = Menu::create(startItem, closeItem,NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    

    // 添加初始背景图
    auto sprite = Sprite::create("StartBackground.jpg");
    if (sprite == nullptr)
    {
        problemLoading("'StartBackground.jpg'");
    }
    else
    {       
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
        sprite->setScale(3.2);       
        this->addChild(sprite, 0);
    }

    global_init();  // init global variables and objects
   
    return true;
}


void HelloWorld::menuClose(Ref*obj)
{
    //结束程序
    Director::getInstance()->end();

}

void HelloWorld::menuStart(Ref* obj)
{
    //结束程序
    auto scene = GameScene::createScene("Mountain", -1);  // -1 represent place main character in the middle of the map
    Director::getInstance()->replaceScene(TransitionCrossFade::create(1, scene));

}