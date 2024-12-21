#include "Fishing.h"
#include "SimpleAudioEngine.h"

bool Fishing::init(TMXTiledMap* map, MainCharacter* maincharacter) {
    isfishing = 0;
  
    fishingcondition = 0;
    scheduleCounter = 0;
    visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸
    //初始化标签与背景
    isfishingnow = Label::createWithSystemFont("You are fishing now.\nClick fish pole again to cancel", "Arial", 30);
    isfishingnow->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(isfishingnow,2);
    isfishingnow->setVisible(false);
    //
    isfishingnowtext = Sprite::create("fishingtext.png");
    isfishingnowtext->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(isfishingnowtext, 1);
    isfishingnowtext->setScale(0.9);
    isfishingnowtext->setVisible(false);
    //
    mapWidth = map->getMapSize().width;  // 横向瓷砖数量
    mapHeight = map->getMapSize().height; // 纵向瓷砖数量
    tileWidth = map->getTileSize().width * ScaleFactor; // 单个瓷砖的像素宽度
    tileHeight = map->getTileSize().height * ScaleFactor; // 单个瓷砖的像素高度
    maplength = mapWidth * tileWidth;
    mapwidth = mapHeight * tileHeight;
    setfishingcheckbox();//设置复选框

    fishingListenerMouse(map, maincharacter);
    return true;
}

void  Fishing::setfishingcheckbox() {
    auto visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸
    //创建锄头图标
    fishingcheckbox = ui::CheckBox::create("fishing1.png", "fishing2.png");
    fishingcheckbox->setPosition(Vec2(visibleSize.width * 3 / 8, visibleSize.height / 4)); // 设置位置

    //// 添加事件监听器
    //glovescheckbox->addEventListener([this](Ref* sender, ui::CheckBox::EventType type) {
    //    switch (type) {
    //        case ui::CheckBox::EventType::SELECTED:
    //            isgloves = 1;
    //            break;
    //        case ui::CheckBox::EventType::UNSELECTED:
    //            isgloves = 0;
    //            break;
    //        default:
    //            break;
    //    }
    //    });
    this->addChild(fishingcheckbox);


}
//钓鱼过程
void  Fishing::fishingprocess(TMXTiledMap* map, MainCharacter* maincharacter) {
    isfishingnow->setVisible(true);
    isfishingnowtext->setVisible(true);
   
  
    std::string updateKey = "update_function_key_" + std::to_string(scheduleCounter++);
    this->schedule([=](float dt) {
        myUpdateFunction(updateKey); // 调用更新函数
        }, 0.02f, updateKey);
    std::string updateKey2 = "update2_function_key_" + std::to_string(scheduleCounter++);
    this->schedule([=](float dt) {
        myUpdateFunction2(map,maincharacter, updateKey2); // 调用更新函数
        }, 0.02f, updateKey2);
    std::string updateKey3 = "update3_function_key_" + std::to_string(scheduleCounter++);
    this->schedule([=](float dt) {
        myUpdateFunction3( updateKey3); // 调用更新函数
        }, 0.02f, updateKey3);
}
Fishing* Fishing::create(TMXTiledMap* map, MainCharacter* maincharacter) {

    Fishing* ret = new Fishing();
    if (ret && ret->init(map, maincharacter)) {
        ret->autorelease(); // 自动释放内存
        return ret;
    }
    CC_SAFE_DELETE(ret); // 如果创建失败，安全删除
    return nullptr;
}
//是否获得鱼
void Fishing::ifgainfish() {
    
    int random_number = std::rand() % (5000 + 1); // 生成 0 到 max_value 之间的随机数
    if (random_number == 5) {
        fish_number++;
        //生成提示标签
        auto label = Label::createWithSystemFont("You gain a fish!", "Arial", 35);
        label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 3));
        label->setTextColor(Color4B::RED);
        this->addChild(label, 7);
        auto callback = [=]() {
            this->removeChild(label);
          
            };
        Action* rebackaction = Sequence::create(DelayTime::create(0.5), CallFunc::create(callback), nullptr);
        this->runAction(rebackaction);
    }
}
void Fishing::fishingListenerMouse(TMXTiledMap* map, MainCharacter* maincharacter) {
    // 创建鼠标事件监听器
    auto mouseListener = EventListenerMouse::create();

    mouseListener->onMouseDown = [=](Event* event) {
        EventMouse* mouseEvent = static_cast<EventMouse*>(event);
        Vec2 mapPosition = map->getPosition();
        if (isfishing == 1) {
            // 获取鼠标点击的位置
            Vec2 clickPos = mouseEvent->getLocation();//以左上角为原点
            clickPos.y = visibleSize.height - clickPos.y;//转化为左下角为原点
            auto tileLayer = map->getLayer("Back");
            //坐标转化为相对地图左下角的
            clickPos.x = clickPos.x - mapPosition.x + maplength / 2;
            clickPos.y = clickPos.y - mapPosition.y + mapwidth / 2;
            // 转换为相对地图左上角的图块单位坐标
            int tileX = static_cast<int>(clickPos.x / (ScaleFactor * 16));
            int tileY = mapHeight - 1 - static_cast<int>((clickPos.y) / (ScaleFactor * 16));
            int tileGID = tileLayer->getTileGIDAt(Vec2(tileX, tileY));
            //CCLOG("Tile GID at (tileX: %d, tileY: %d) is %d", tileX, tileY, tileGID);
            if (tileLayer) {

                if (tileGID == AbleFishing) {//判断是否可diaoyu
                    fishingcondition = 1;
                    
                    fishingprocess(map, maincharacter);
                    //钓鱼操作
                }

            }
        }
        };
    // 将监听器添加到事件分配器
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);
}
//停止角色移动
void Fishing::myUpdateFunction2(TMXTiledMap* map,MainCharacter*maincharacter, std::string updateKey) {
   
        if (fishingcondition == 1) {
            map->stopAllActions(); // 停止所有动作
            maincharacter->stopAllActions(); // 停止所有动作
        
        }
        else {
            this->unschedule(updateKey);
        }
    
}
//设置钓鱼标签不可见
void Fishing::myUpdateFunction(std::string updateKey) {
 
        if (fishingcondition!=1) {
            isfishingnow->setVisible(false);
            isfishingnowtext->setVisible(false);
            this->unschedule(updateKey);
        }
    
}
//是否调用ifgainfish函数
void Fishing::myUpdateFunction3(std::string updateKey3) {
    if (fishingcondition == 1) {
       
            ifgainfish(); // 调用函数

        
    }
    else
        this->unschedule(updateKey3);
}