

#include "map/map.h"
/*
#include "SimpleAudioEngine.h"
#include <ctime>

USING_NS_CC;

Scene* OutdoorScene::createScene()
{
    return OutdoorScene::create();
}

// on "init" you need to initialize your instance
bool OutdoorScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    map = TMXTiledMap::create("Backwoods.tmx");
    mapOffset_ = Vec2::ZERO;

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    //¼üÅÌÊÂ¼þ¼àÌýÆ÷
    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyPressed = CC_CALLBACK_2(OutdoorScene::keyPressed, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

    return true;
}


void OutdoorScene::keyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_A:
        map->setPosition(--mapOffset_.x, mapOffset_.y);
        break;
    case EventKeyboard::KeyCode::KEY_D:
        map->setPosition(++mapOffset_.x, mapOffset_.y);
        break;
    case EventKeyboard::KeyCode::KEY_W:
        map->setPosition(mapOffset_.x, ++mapOffset_.y);
        break;
    case EventKeyboard::KeyCode::KEY_S:
        map->setPosition(mapOffset_.x, --mapOffset_.y);
        break;
    }
}
*/