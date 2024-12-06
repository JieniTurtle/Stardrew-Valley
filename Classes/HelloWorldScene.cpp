/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <ctime>

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
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
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    Sprite* man = Sprite::create("HelloWorld.png");
    this->addChild(man,2);
    man->setScale(0.5);
    man->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    map_ = TMXTiledMap::create("map/Backwoods.tmx");
    map_->setScale(3);
    auto layer = map_->getLayer("Back");
    this->addChild(map_);

    mapOffset_ = Vec2::ZERO;

    //¼üÅÌÊÂ¼þ¼àÌýÆ÷
    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::keyPressed, this);
    keyListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::keyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);


    return true;
}

void HelloWorld::keyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        _isMovingLeft = true;
        break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        _isMovingRight = true;
        break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
        _isMovingUp = true;
        break;
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        _isMovingDown = true;
        break;
    }
    log("Key with keycode %d pressed", keyCode);
    this->schedule(schedule_selector(HelloWorld::my_update), 0.01f);
}

void HelloWorld::my_update(float deltaTime)
{
    
    Vec2 newPosition = map_->getPosition();
    float v = 100;

    if (_isMovingLeft)
    {
        newPosition.x += v * deltaTime;
    }
    if (_isMovingRight)
    {
        newPosition.x -= v * deltaTime;
    }
    if (_isMovingUp)
    {
        newPosition.y -= v * deltaTime;
    }
    if (_isMovingDown)
    {
        newPosition.y += v * deltaTime;
    }

    map_->setPosition(newPosition);
    newPosition = map_->getPosition();
    
}

void HelloWorld::keyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
     switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        _isMovingLeft = false;
        break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        _isMovingRight = false;
        break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
        _isMovingUp = false;
        break;
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        _isMovingDown = false;
        break;
    }
    
    log("Key with keycode %d pressed", keyCode);
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    auto myScene = HelloWorld::createScene();
    // Director::getInstance()->replaceScene(myScene);

    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
