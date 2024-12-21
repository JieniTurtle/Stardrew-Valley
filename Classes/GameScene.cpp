#include "animals.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Task.h"
#include "global.h"
#include "explore.h"
#include "Date.h"

USING_NS_CC;

#define MIDDLE -1
#define LEFT 1
#define UP 0
#define RIGHT 3
#define DOWN 2

#define HOLIDAY_DATE 1

int GameScene::mapscale = 2;

Scene* GameScene::createScene(std::string s, int direction)
{
    return GameScene::create(s, direction);
}

GameScene* GameScene::create(std::string s, int direction)
{
    GameScene* scene = new GameScene;
    if (scene) {
        scene->scene_name_ = s;
        if (scene->init(direction)) {
            return scene;
        }
    }
    return nullptr;
}


void GameScene::set_physical_map(TMXTiledMap* map) {
    auto layer = map->getLayer("Buildings");
    const int tile_width = 16, tile_height = 16;
    const int scale = 2;

    for (int x = 0; x < map->getMapSize().width; x++) {
        for (int y = 0; y < map->getMapSize().height; y++) {
            if (layer->getTileGIDAt(Vec2(x, y)) != 0) {
                if (x > 0 && x < map->getMapSize().width - 1 && y > 0 && y < map->getMapSize().height - 1) {
                    if (layer->getTileGIDAt(Vec2(x + 1, y)) != 0 && layer->getTileGIDAt(Vec2(x - 1, y)) != 0 && layer->getTileGIDAt(Vec2(x, y + 1)) != 0 && layer->getTileGIDAt(Vec2(x, y - 1)) != 0) {
                        continue;
                    }
                }
                
                auto physics_body = PhysicsBody::createBox(Size(tile_width, tile_height), PhysicsMaterial(1.0f, 0.0f, 0.01f));
                physics_body->setDynamic(false);
                auto tile = layer->getTileAt(Vec2(x, y));
                tile->addComponent(physics_body);


                // for going to the mine
                if (layer->getTileGIDAt(Vec2(x, y)) == 959 && scene_name_ == "Town") {
                    physics_body->setContactTestBitmask(0xFFFFFFFF);
                    tile->setTag(999);
                }
                if (layer->getTileGIDAt(Vec2(x, y)) == 289 && scene_name_ == "Mine") {
                    physics_body->setContactTestBitmask(0xFFFFFFFF);
                    tile->setTag(999);
                }
            }
        }
    }
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameScene::init(int direction)
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸
    Vec2 origin = Director::getInstance()->getVisibleOrigin();//这行代码获取当前游戏视图窗口的原点坐标

    Scene::initWithPhysics();
    this->getPhysicsWorld()->setGravity(Vec2(0, 0));
    this->getPhysicsWorld()->setUpdateRate(1);
    // 启用调试绘制
    this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // 添加初始背景图
    TMXTiledMap* map;
    if (date == HOLIDAY_DATE && scene_name_ == "Town")
        map = TMXTiledMap::create("maps/" + scene_name_ + "-EggFestival.tmx");
    else
        map = TMXTiledMap::create("maps/" + scene_name_ + ".tmx");

    if (!map) {
        problemLoading("'newnewFarm.tmx'");
        return false;
    }
    if (!map) {
        CCLOG("map fail");
        return false;
    }
    this->addChild(map, 0);
    map->setScale(mapscale);//地图扩大两倍
    set_physical_map(map);
    map->getLayer("Back")->setLocalZOrder(-3);
    map->getLayer("Buildings")->setLocalZOrder(-2);

    auto task_layer = TaskLayer::create(task_manager);
    this->addChild(task_layer);

    // 获取地图的大小
    mapWidth = map->getMapSize().width;  // 横向瓷砖数量
    mapHeight = map->getMapSize().height; // 纵向瓷砖数量
    tileWidth = map->getTileSize().width; // 单个瓷砖的像素宽度
    tileHeight = map->getTileSize().height; // 单个瓷砖的像素高度
    map->setAnchorPoint(Vec2(0.5, 0.5));
    map->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    auto tileLayer = map->getLayer("soil"); // 替换为您的图层名称
    Vec2 tilePosition(1, 64); // 假设这是您要查询的图块位置，相对于左上角
    if (scene_name_ == "newnewFarm") {
        int tileGID = tileLayer->getTileGIDAt(tilePosition, nullptr);
        CCLOG("Tile GID at: %d", tileGID);
    }

    ///////////////////////////////////////
    std::string expString = "Experience: " + std::to_string(experience);
    auto label = Label::createWithTTF(expString, "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(visibleSize.width * 8 / 10, visibleSize.height * 9.5 / 10)); // 设置标签显示的位置
    label->setColor(Color3B::GREEN);
    this->addChild(label, 1);

    // 创建一个定时器或调度器，以便在每帧更新标签
    this->schedule([this, label](float dt) {
        label->setString("Experience: " + std::to_string(experience));
        }, "update_label_key_exp");
    money = 10000;
    std::string  moneyString = "Money: " + std::to_string(money);
    Label* labelmoney = Label::createWithTTF(moneyString, "fonts/Marker Felt.ttf", 24);
    labelmoney->setPosition(Vec2(visibleSize.width * 8 / 10, visibleSize.height * 9 / 10)); // 设置标签显示的位置
    labelmoney->setColor(Color3B::YELLOW);
    labelmoney->setVisible(true);
    this->addChild(labelmoney, 3);

    // 创建一个定时器或调度器，以便在每帧更新标签
    this->schedule([this, labelmoney](float dt) {
        labelmoney->setString("Money: " + std::to_string(money));
        }, "update_label_key");
    ////////////////////////////////////////////////
    createRain();
    mineral = Mineral::create(map);//传入矿石
    this->addChild(mineral, 1);
    
    seeds = Seeds::create(map);//传入种子
    this->addChild(seeds, 1);
    wheat = Wheat::create(map);//传入小麦
    this->addChild(wheat, 1);
    seedstwo = Seedstwo::create(map);//传入种子2
    this->addChild(seedstwo, 1);
    carrot = Carrot::create(map);//传入carrot
    this->addChild(carrot, 1);
    wood = Wood::create(map);//传入wood
    this->addChild(wood, 1);
    farmproduct = FarmProduct::create(map);//传入
    this->addChild(farmproduct, 1);
    fertilizer = Fertilizer::create(map);//传入fertilizer
    this->addChild(fertilizer, 1);
    cookLayer = CookLayer::create();
    this->addChild(cookLayer);
    store = Store::create(map);
    this->addChild(store,3);

    weatherchange();

    auto date_layer = Date::create();
    this->addChild(date_layer);

    //创建主角
    character = MainCharacter::create("MainCharacter/transparent.png");    // this HelloWorld.png is set invisible, don't care
    character->setMap(map);//传入地图
    map->addChild(character, 1);
    int x = 0, y = 0;
    switch (direction)
    {
    case MIDDLE:   // the first time you enter this game
        x = mapWidth / 2, y = mapHeight / 2;
        break;
    case LEFT:
        x = 1;
        if (scene_name_ == "Town")
            y = 56;
        else if (scene_name_ == "Mine")
            x = 14, y = 9;
        else if (scene_name_ == "Mountain")
            y = 27;
        break;
    case RIGHT:
        x = map->getMapSize().width - 1;
        if (scene_name_ == "newnewFarm")
            y = 48;
        else if (scene_name_ == "Town")
            x = 107, y = 33;
        else if (scene_name_ == "Woods")
            y = 15;
        break;
    case UP:
        y = map->getMapSize().height - 1;
        if (scene_name_ == "Town")
            x = 81;
        break;
    case DOWN:
        y = 2;
        if (scene_name_ == "Mountain")
            x = 15;
        break;
    default:
        break;
    }
    character->setPosition(x * tileWidth, y * tileWidth); // 初始位置

    // bridge
    if (scene_name_ == "Mountain" && !bridge_repaired) {
        auto borken_bridge = Bridge::create(map);
        map->addChild(borken_bridge, 10);
    }

    // treasure chest
    if (scene_name_ == "Mountain" && !chest_opened[0]) {
        auto chest = Chest::create(map, 0);
        map->addChild(chest, 5);
    }
    if (scene_name_ == "Woods" && !chest_opened[1]) {
        auto chest = Chest::create(map, 1);
        map->addChild(chest, 5);
    }

    MouseStatus = notTaken;//设置鼠标状态

    //地图为城镇，创建NPC
    if (scene_name_ == "Town" && date != HOLIDAY_DATE) {
        std::vector<Vec2> path_Willy = { Vec2(28, 51), Vec2(29, 19), Vec2(-10, 19), Vec2(-10, 55), Vec2(26, 55), Vec2(28, 55) };
        NPC_Willy = initNPC("Willy", path_Willy, map);
        NPC_Willy->retain();

        //创建NPC Gus 及对话 在地图左上方
        std::vector<Vec2> path_Gus = { Vec2(81, 87),Vec2(20, 75), Vec2(20, 55), Vec2(-100, 55), Vec2(-100, 200), Vec2(81, 200)};
        NPC_Gus = initNPC("Gus", path_Gus, map);
        NPC_Gus->retain();

        //创建NPC Jodi 及对话 在地图右上方
        std::vector<Vec2> path_Jodi = {Vec2(-10, 55),  Vec2(28, 55), Vec2(28, 51), Vec2(29, 19), Vec2(-10, 19), Vec2(-10, 55), Vec2(26, 55)};
        NPC_Jodi = initNPC("Jodi", path_Jodi, map);
        NPC_Jodi->retain();

        //创建NPC Harvey 及对话 在地图右下方
        std::vector<Vec2> path_Harvey = { Vec2(-100, 200), Vec2(81, 200), Vec2(81, 87),Vec2(20, 75), Vec2(20, 55), Vec2(-100, 55),  };
        NPC_Harvey = initNPC("Harvey", path_Harvey, map);
        NPC_Harvey->retain();

        relationFull = Sprite::create("NPC/relationFull.png");
        relationFull->retain();

        //添加鼠标监听事件
        addMouseListener();
        this->schedule(CC_SCHEDULE_SELECTOR(GameScene::Mouseupdate), 0.1f); // 更新间隔为0.1秒
    }
    //地图为农场，创建newnew
    else if(scene_name_ == "newnewFarm") {
        tools = Tools::create(map);//传入工具
        this->addChild(tools, 1);
      

        gloves = Gloves::create(map);//传入手套
        this->addChild(gloves, 1);

        axe = Axe::create(map);//传入斧子
        this->addChild(axe, 1);
        kettle = Kettle::create(map);//传入kettle
        this->addChild(kettle, 1);


        pickaxe = Pickaxe::create(map);//传入gaozi
        this->addChild(pickaxe, 1);
        fishing = Fishing::create(map, character);//传入
        this->addChild(fishing, 1);
        ////



        auto cow = Cow::create("cow");
        cow->setMaincharacter(character);
        cow->setMap(map);
        Cow::move(cow, map);

        auto sheep = Sheep::create("sheep");
        sheep->setMaincharacter(character);
        sheep->setMap(map);
        Sheep::move(sheep, map);

        auto chicken = Chicken::create("chicken");
        chicken->setMaincharacter(character);
        chicken->setMap(map);
        Chicken::move(chicken, map);

        CheckboxOnlyone();
        NewFishingListening();//解锁钓鱼
        NewPickaxeListening();//解锁挖矿
        NewCookLayerListening();//解锁烹饪
    }

    return true;
}

//监听每一个复选框，每次只能选一个
void GameScene::CheckboxOnlyone() {
    // 添加事件监听器
    tools->hoecheckbox->addEventListener([this](Ref* sender, ui::CheckBox::EventType type) {
        switch (type) {
            case ui::CheckBox::EventType::SELECTED:
                tools->ishoe = 1;
                CloseOtherCheckbox(tools->hoecheckbox);
                break;
            case ui::CheckBox::EventType::UNSELECTED:
                tools->ishoe = 0;
                break;
            default:
                break;
        }
        });
    // 添加事件监听器
    pickaxe->pickaxecheckbox->addEventListener([this](Ref* sender, ui::CheckBox::EventType type) {
        switch (type) {
            case ui::CheckBox::EventType::SELECTED:
                pickaxe->ispickaxe = 1;
                CloseOtherCheckbox(pickaxe->pickaxecheckbox);
                break;
            case ui::CheckBox::EventType::UNSELECTED:
                pickaxe->ispickaxe = 0;
                break;
            default:
                break;
        }
        });
    // 添加事件监听器
    fishing->fishingcheckbox->addEventListener([this](Ref* sender, ui::CheckBox::EventType type) {
        switch (type) {
            case ui::CheckBox::EventType::SELECTED:
                fishing->isfishing = 1;
                CloseOtherCheckbox(fishing->fishingcheckbox);
                break;
            case ui::CheckBox::EventType::UNSELECTED:
                fishing->isfishing = 0;
                fishing->fishingcondition = 0;//钓鱼状态为0

                break;
            default:
                break;
        }
        });
    // 添加事件监听器
    seeds->seedscheckbox->addEventListener([this](Ref* sender, ui::CheckBox::EventType type) {
        switch (type) {
            case ui::CheckBox::EventType::SELECTED:
                seeds->isseeds = 1;
                CloseOtherCheckbox(seeds->seedscheckbox);
                break;
            case ui::CheckBox::EventType::UNSELECTED:
                seeds->isseeds = 0;
                break;
            default:
                break;
        }
        });
    // 添加事件监听器
    seedstwo->seedstwocheckbox->addEventListener([this](Ref* sender, ui::CheckBox::EventType type) {
        switch (type) {
            case ui::CheckBox::EventType::SELECTED:
                seedstwo->isseedstwo = 1;
                CloseOtherCheckbox(seedstwo->seedstwocheckbox);
                break;
            case ui::CheckBox::EventType::UNSELECTED:
                seedstwo->isseedstwo = 0;
                break;
            default:
                break;
        }
        });
    // 添加事件监听器
    gloves->glovescheckbox->addEventListener([this](Ref* sender, ui::CheckBox::EventType type) {
        switch (type) {
            case ui::CheckBox::EventType::SELECTED:
                gloves->isgloves = 1;
                CloseOtherCheckbox(gloves->glovescheckbox);
                break;
            case ui::CheckBox::EventType::UNSELECTED:
                gloves->isgloves = 0;
                break;
            default:
                break;
        }
        });
    // 添加事件监听器
    wheat->wheatcheckbox->addEventListener([this](Ref* sender, ui::CheckBox::EventType type) {
        switch (type) {
            case ui::CheckBox::EventType::SELECTED:
                wheat->iswheat = 1;
                CloseOtherCheckbox(wheat->wheatcheckbox);
                break;
            case ui::CheckBox::EventType::UNSELECTED:
                wheat->iswheat = 0;
                break;
            default:
                break;
        }
        });
    // 添加事件监听器
    carrot->carrotcheckbox->addEventListener([this](Ref* sender, ui::CheckBox::EventType type) {
        switch (type) {
            case ui::CheckBox::EventType::SELECTED:
                carrot->iscarrot = 1;
                CloseOtherCheckbox(carrot->carrotcheckbox);
                break;
            case ui::CheckBox::EventType::UNSELECTED:
                carrot->iscarrot = 0;
                break;
            default:
                break;
        }
        });
    // 添加事件监听器
    axe->axecheckbox->addEventListener([this](Ref* sender, ui::CheckBox::EventType type) {
        switch (type) {
            case ui::CheckBox::EventType::SELECTED:
                axe->isaxe = 1;
                CloseOtherCheckbox(axe->axecheckbox);
                break;
            case ui::CheckBox::EventType::UNSELECTED:
                axe->isaxe = 0;
                break;
            default:
                break;
        }
        });
    // 添加事件监听器
    wood->woodcheckbox->addEventListener([this](Ref* sender, ui::CheckBox::EventType type) {
        switch (type) {
            case ui::CheckBox::EventType::SELECTED:
                wood->iswood = 1;
                CloseOtherCheckbox(wood->woodcheckbox);
                break;
            case ui::CheckBox::EventType::UNSELECTED:
                wood->iswood = 0;
                break;
            default:
                break;
        }
        });
    // 添加事件监听器
    kettle->kettlecheckbox->addEventListener([this](Ref* sender, ui::CheckBox::EventType type) {
        switch (type) {
            case ui::CheckBox::EventType::SELECTED:
                kettle->iskettle = 1;
                CloseOtherCheckbox(kettle->kettlecheckbox);
                break;
            case ui::CheckBox::EventType::UNSELECTED:
                kettle->iskettle = 0;
                break;
            default:
                break;
        }
        });
    // 添加事件监听器
    fertilizer->fertilizercheckbox->addEventListener([this](Ref* sender, ui::CheckBox::EventType type) {
        switch (type) {
            case ui::CheckBox::EventType::SELECTED:
                fertilizer->isfertilizer = 1;
                CloseOtherCheckbox(fertilizer->fertilizercheckbox);
                break;
            case ui::CheckBox::EventType::UNSELECTED:
                fertilizer->isfertilizer = 0;
                break;
            default:
                break;
        }
        });
}


NPC* GameScene::initNPC(std::string NPC_Name, std::vector<Vec2>& NPC_Path, TMXTiledMap* NPC_Map)
{
    NPC* npc = NPC::create(NPC_Name);
    if (npc == nullptr) { return NULL; }
    npc->setPath(NPC_Path);
    npc->setMap(NPC_Map);
    NPC_Map->addChild(npc);
    npc->startMovement();
    npc->Dialog_NPC->retain();
    return npc;
}

void GameScene::addMouseListener()
{
    auto MouseListener = EventListenerMouse::create();
    MouseListener->onMouseDown = CC_CALLBACK_1(GameScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(MouseListener, this);
}

void GameScene::Mouseupdate(float delta)
{
    if (NPC_Willy->Dialog_NPC->ChangeToClose) {
        MouseStatus = notTaken;
        DialogClose(NPC_Willy);
    }
    if (NPC_Gus->Dialog_NPC->ChangeToClose) {
        MouseStatus = notTaken;
        DialogClose(NPC_Gus);
    }
    if (NPC_Jodi->Dialog_NPC->ChangeToClose) {
        MouseStatus = notTaken;
        DialogClose(NPC_Jodi);
    }
    if (NPC_Harvey->Dialog_NPC->ChangeToClose) {
        MouseStatus = notTaken;
        DialogClose(NPC_Harvey);
    }

    NPCTaskManger(NPC_Willy);
    NPCTaskManger(NPC_Gus);
    NPCTaskManger(NPC_Jodi);
    NPCTaskManger(NPC_Harvey);
}

void GameScene::NPCTaskManger(NPC* npc)
{
    if (npc->Dialog_NPC->TaskStatus) {
        if (npc->Dialog_NPC->TaskStatus == 1) {
            Task* npcTask = new Task;
            npcTask->name = npc->NPCname + "TaskName";
            npcTask->content = npc->NPCname + "TaskContent";
            task_manager->add_task(npcTask);
        }
        else if (npc->Dialog_NPC->TaskStatus == 2) {
            Task* npctask = task_manager->get_task_by_name(npc->NPCname + "TaskName");
            task_manager->remove_task(npctask);
        }
        npc->Dialog_NPC->TaskStatus = 0;
    }
}

void GameScene::onMouseDown(cocos2d::Event* event)
{
    //获取可视范围
    auto visibleSize = Director::getInstance()->getVisibleSize();
    //获取鼠标点击的位置（初始原点为左上角）
    EventMouse* e = static_cast<EventMouse*>(event);
    Vec2 clickPos = e->getLocation() / mapscale;
    //将鼠标点击的位置转化为以屏幕左下角为原点
    clickPos.y = visibleSize.height / mapscale - clickPos.y;

    if (MouseStatus == notTaken) {//鼠标未被占据
        //处理点击到 NPC 的事件
        if (NPC_Willy->JudgeClickNPC(clickPos, mapscale)) {
            NPC_Willy->stopMovement();
            this->addChild(NPC_Willy->Dialog_NPC);
            MouseStatus = TakenByNPC;
            return;
        }
        if (NPC_Gus->JudgeClickNPC(clickPos, mapscale)) {
            NPC_Gus->stopMovement();
            this->addChild(NPC_Gus->Dialog_NPC);
            MouseStatus = TakenByNPC;
            return;
        }
        if (NPC_Jodi->JudgeClickNPC(clickPos, mapscale)) {
            NPC_Jodi->stopMovement();
            this->addChild(NPC_Jodi->Dialog_NPC);
            MouseStatus = TakenByNPC;
            return;
        }
        if (NPC_Harvey->JudgeClickNPC(clickPos, mapscale)) {
            NPC_Harvey->stopMovement();
            this->addChild(NPC_Harvey->Dialog_NPC);
            MouseStatus = TakenByNPC;
            return;
        }
    }
}

//亲密关系处理
void GameScene::DialogClose(NPC* npc)
{
    npc->startMovement();
    //亲密度满时
    if (npc->relation == 100) {
        relationFull->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 3));
        this->addChild(relationFull);
        this->scheduleOnce([this](float) {
            relationFull->removeFromParent();
            }, 1.0f, "hide_relation_full_key");
    }
    else {
        //对话增加亲密度时
        if (npc->Dialog_NPC->ChangeToClose == 1) {
            npc->relation += 10;
            // 创建一个标签来显示消息
            relationTip = Label::createWithSystemFont("RealtionDegree+10!", "Arial", 30);
            relationTip->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
                Director::getInstance()->getVisibleSize().height / 3));

            // 将标签添加到当前场景中
            this->addChild(relationTip);

            // 设置定时器，1秒后自动移除消息
            this->scheduleOnce([this](float deltaTime) {
                if (relationTip)
                {
                    relationTip->removeFromParent();  // 移除消息标签
                }
                }, 1.0f, "remove_message_key");  // 设置延时1秒后执行
        }
        //完成NPC任务增加亲密度时
        else if (npc->Dialog_NPC->ChangeToClose == 2) {
            npc->relation += 70;
            // 创建一个标签来显示消息
            relationTip = Label::createWithSystemFont("RealtionDegree+70!", "Arial", 30);
            relationTip->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
                Director::getInstance()->getVisibleSize().height / 3));

            // 将标签添加到当前场景中
            this->addChild(relationTip);

            // 设置定时器，1秒后自动移除消息
            this->scheduleOnce([this](float deltaTime) {
                if (relationTip)
                {
                    relationTip->removeFromParent();  // 移除消息标签
                }
                }, 1.0f, "remove_message_key");  // 设置延时1秒后执行
        }
    }
    npc->Dialog_NPC->ChangeToClose = 0;
}

//关闭其他复选框
void GameScene::CloseOtherCheckbox(ui::CheckBox* selectedCheckbox) {
    if (selectedCheckbox != tools->hoecheckbox) {
        tools->hoecheckbox->setSelected(false); // 取消选择另一个复选框
        tools->ishoe = 0;
    }
    if (selectedCheckbox != pickaxe->pickaxecheckbox) {
        pickaxe->pickaxecheckbox->setSelected(false); // 取消选择另一个复选框
        pickaxe->ispickaxe = 0;
    }
    if (selectedCheckbox != fishing->fishingcheckbox) {
        fishing->fishingcheckbox->setSelected(false); // 取消选择另一个复选框
        fishing->isfishing = 0;
        fishing->fishingcondition = 0;
    }
    if (selectedCheckbox != seeds->seedscheckbox) {
        seeds->seedscheckbox->setSelected(false); // 取消选择另一个复选框
        seeds->isseeds = 0;
    }
    if (selectedCheckbox != seedstwo->seedstwocheckbox) {
        seedstwo->seedstwocheckbox->setSelected(false); // 取消选择另一个复选框
        seedstwo->isseedstwo = 0;
    }
    if (selectedCheckbox != gloves->glovescheckbox) {

        gloves->glovescheckbox->setSelected(false);
        gloves->isgloves = 0;
    }
    if (selectedCheckbox != wheat->wheatcheckbox) {

        wheat->wheatcheckbox->setSelected(false);
        wheat->iswheat = 0;
    }
    if (selectedCheckbox != carrot->carrotcheckbox) {

        carrot->carrotcheckbox->setSelected(false);
        carrot->iscarrot = 0;
    }
    if (selectedCheckbox != axe->axecheckbox) {

        axe->axecheckbox->setSelected(false);
        axe->isaxe = 0;
    }
    if (selectedCheckbox != wood->woodcheckbox) {
        wood->iswood = 0;
        wood->woodcheckbox->setSelected(false);
    }
    if (selectedCheckbox != kettle->kettlecheckbox) {
        kettle->iskettle = 0;
        kettle->kettlecheckbox->setSelected(false);
    }
    if (selectedCheckbox != fertilizer->fertilizercheckbox) {
        fertilizer->isfertilizer = 0;
        fertilizer->fertilizercheckbox->setSelected(false);
    }
}
void GameScene::showmoneynum() {
    //// 创建标签并添加到场景中
    //auto label = Label::createWithTTF(std::to_string(money), "fonts/Marker Felt.ttf", 24);
    //label->setPosition(Vec2(visibleSize.width * 8 / 10, visibleSize.height * 9 / 10)); // 设置标签显示的位置
    //this->addChild(label, 1);

    //// 创建一个定时器或调度器，以便在每帧更新标签
    //this->schedule([this, label](float dt) {
    //    label->setString(std::to_string(money));
    //    }, "update_label_key");
    std::string moneyString = "Money: " + std::to_string(money);
    auto label = Label::createWithTTF(moneyString, "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(visibleSize.width * 8 / 10, visibleSize.height * 9 / 10)); // 设置标签显示的位置
    label->setColor(Color3B::BLACK);
    this->addChild(label, 1);

    // 创建一个定时器或调度器，以便在每帧更新标签
    this->schedule([this, label](float dt) {
        label->setString("Money: " + std::to_string(money));
        }, "update_label_key");
}
void GameScene::NewFishingListening() {
    //CCLOG("66666%d",NewFishingListeningbool);
    if (1) {
        
        fishing->fishingcheckbox->setEnabled(false);
        this->schedule([this](float dt) {
            // 这里编写更新逻辑，当满足特定条件时执行
            if (experience >= NewFishingExp) {
                auto VisibleSize = cocos2d::Director::getInstance()->getVisibleSize();
                fishing->fishingcheckbox->setEnabled(true);
                //设置提示标签
                if (NewFishingListeningbool == 0) {
                    auto label = Label::createWithSystemFont("You have enough experience.You can fish now.", "Arial", 24);
                    label->setPosition(Vec2(VisibleSize.width / 2, VisibleSize.height / 2));
                    auto sprite = Sprite::create("buysellfailscene.png");//背景
                    sprite->setPosition(Vec2(VisibleSize.width / 2, VisibleSize.height / 2));
                    sprite->setScale(1.2);
                    this->addChild(label, 6);
                    this->addChild(sprite, 5);

                    label->setVisible(true);

                    label->setColor(Color3B::BLUE);

                    auto callback = [=]() {
                        this->removeChild(label);
                        this->removeChild(sprite);
                        };
                    Action* rebackaction = Sequence::create(DelayTime::create(ExpTime), CallFunc::create(callback), nullptr);
                    this->runAction(rebackaction);
                    // 调度器延迟执行，n秒长成熟

                }
                NewFishingListeningbool = 1;
                this->unschedule("update_key_fishing");  // 取消调度
            }

            // 此处可以添加其他需要执行的逻辑
            }, 0.2f, "update_key_fishing");  // 使用一个键来标识这个调度
      
    }

}
void GameScene::NewPickaxeListening()//解锁挖矿
{
    if (1) {
        pickaxe->pickaxecheckbox->setEnabled(false);
        this->schedule([this](float dt) {
            // 这里编写更新逻辑，当满足特定条件时执行
            if (experience >= NewPickaxeExp) {
                auto VisibleSize = cocos2d::Director::getInstance()->getVisibleSize();
                pickaxe->pickaxecheckbox->setEnabled(true);
                //设置提示标签
                if (NewPickaxeListeningbool == 0) {
                    auto label = Label::createWithSystemFont("You have enough experience.You can mine now.", "Arial", 24);
                    label->setPosition(Vec2(VisibleSize.width / 2, VisibleSize.height / 2));
                    auto sprite = Sprite::create("buysellfailscene.png");//背景
                    sprite->setPosition(Vec2(VisibleSize.width / 2, VisibleSize.height / 2));
                    sprite->setScale(1.2);
                    this->addChild(label, 6);
                    this->addChild(sprite, 5);

                    label->setVisible(true);

                    label->setColor(Color3B::BLUE);

                    auto callback = [=]() {
                        this->removeChild(label);
                        this->removeChild(sprite);
                        };
                    Action* rebackaction = Sequence::create(DelayTime::create(ExpTime), CallFunc::create(callback), nullptr);
                    this->runAction(rebackaction);
                    // 调度器延迟执行，n秒长成熟

                }
                NewPickaxeListeningbool = 1;
                this->unschedule("update_key_pickaxe");  // 取消调度
            }

            // 此处可以添加其他需要执行的逻辑
            }, 0.2f, "update_key_pickaxe");  // 使用一个键来标识这个调度
       
    }

}
void GameScene::NewCookLayerListening()//解锁烹饪
{
    if (1) {
        cookLayer->listener->setEnabled(false); // 启用按键监听
        this->schedule([this](float dt) {
            // 这里编写更新逻辑，当满足特定条件时执行
            if (experience >= NewCookLayerExp) {
                auto VisibleSize = cocos2d::Director::getInstance()->getVisibleSize();
                cookLayer->listener->setEnabled(true); // 启用按键监听
                //设置提示标签
                if (NewCookLayerListeningbool == 0) {
                    auto label = Label::createWithSystemFont("You have enough experience.You can cook now.", "Arial", 24);
                    label->setPosition(Vec2(VisibleSize.width / 2, VisibleSize.height / 2));
                    auto sprite = Sprite::create("buysellfailscene.png");//背景
                    sprite->setPosition(Vec2(VisibleSize.width / 2, VisibleSize.height / 2));
                    sprite->setScale(1.2);
                    this->addChild(label, 6);
                    this->addChild(sprite, 5);

                    label->setVisible(true);

                    label->setColor(Color3B::BLUE);

                    auto callback = [=]() {
                        this->removeChild(label);
                        this->removeChild(sprite);
                        };
                    Action* rebackaction = Sequence::create(DelayTime::create(ExpTime), CallFunc::create(callback), nullptr);
                    this->runAction(rebackaction);
                    // 调度器延迟执行，n秒长成熟

                }
                NewCookLayerListeningbool = 1;
                this->unschedule("update_key_cook");  // 取消调度
            }

            // 此处可以添加其他需要执行的逻辑
            }, 0.2f, "update_key_cook");  // 使用一个键来标识这个调度
      
    }

}


void GameScene::createRain() {
    // 初始化阴沉层，设置为全屏黑色精灵
    auto darkOverlay = cocos2d::Sprite::create();
    darkOverlay->setTextureRect(cocos2d::Rect(0, 0, cocos2d::Director::getInstance()->getVisibleSize().width, cocos2d::Director::getInstance()->getVisibleSize().height));
    darkOverlay->setColor(cocos2d::Color3B(0, 0, 0)); // 设置颜色为黑色
    darkOverlay->setOpacity(128); // 设置透明度为128（范围0-255，0完全透明，255不透明）
    darkOverlay->setPosition(cocos2d::Director::getInstance()->getVisibleSize() / 2); // 中心位置
    //darkOverlay->setVisible(false); // 默认隐藏
    this->addChild(darkOverlay);

    // 示例：展示阴沉层
    // 创建粒子系统
    auto rainParticle = cocos2d::ParticleRain::create();
    auto VisibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    rainParticle->setPosition(VisibleSize.width/2, VisibleSize.height); // 设置粒子系统的位置为屏幕中心
    rainParticle->setScale(6.0f); // 设置粒子缩放，调整效果的大小

    // 自定义雨粒子属性
    rainParticle->setLife(1.0f);  // 每个雨滴的生命时间
    rainParticle->setSpeed(200);  // 雨滴速度
    rainParticle->setTexture(cocos2d::Director::getInstance()->getTextureCache()->addImage("raindrop.png")); // 设置雨滴的纹理图

    // 增加雨点的数量
    rainParticle->setTotalParticles(500); // 设置总粒子数量（例如，500）
    rainParticle->setEmissionRate(100); // 每秒发射的粒子数量（例如，100）
    // 添加粒子系统到层中
    this->addChild(rainParticle);

    auto callback = [=]() {
        this->removeChild(darkOverlay);
        this->removeChild(rainParticle);
        };
    Action* rebackaction = Sequence::create(DelayTime::create(WeatherChangeTime), CallFunc::create(callback), nullptr);
    this->runAction(rebackaction);
}

void GameScene::createSunEffect() {
    
    auto darkOverlay = cocos2d::Sprite::create();
    darkOverlay->setTextureRect(cocos2d::Rect(0, 0, cocos2d::Director::getInstance()->getVisibleSize().width, cocos2d::Director::getInstance()->getVisibleSize().height));
    darkOverlay->setColor(cocos2d::Color3B(128, 50, 0)); // 设置颜色为黑色
    darkOverlay->setOpacity(128); // 设置透明度为128（范围0-255，0完全透明，255不透明）
    darkOverlay->setPosition(cocos2d::Director::getInstance()->getVisibleSize() / 2); // 中心位置
    //darkOverlay->setVisible(false); // 默认隐藏
    this->addChild(darkOverlay);

    auto callback = [=]() {
        this->removeChild(darkOverlay);
       
        };
    Action* rebackaction = Sequence::create(DelayTime::create(WeatherChangeTime), CallFunc::create(callback), nullptr);
    this->runAction(rebackaction);
}

void GameScene::weatherchange() {
    this->schedule([this](float dt) {
        weather = (weather + 1)%6;
        if (weather == 4) {
            createSunEffect();
            // 创建一个标签来显示消息
            relationTip = Label::createWithSystemFont("Weather changes!", "Arial", 30);
            relationTip->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 3));

            // 将标签添加到当前场景中
            this->addChild(relationTip);

            // 设置定时器，1秒后自动移除消息
            this->scheduleOnce([this](float deltaTime) {
                if (relationTip)
                {
                    relationTip->removeFromParent();  // 移除消息标签
                }
                }, 1.0f, "remove_message_key");  // 设置延时1秒后执行
        }
        else if (weather == 5) {
            createRain();
            // 创建一个标签来显示消息
            relationTip = Label::createWithSystemFont("Weather changes!", "Arial", 30);
            relationTip->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
                Director::getInstance()->getVisibleSize().height / 3));

            // 将标签添加到当前场景中
            this->addChild(relationTip);

            // 设置定时器，1秒后自动移除消息
            this->scheduleOnce([this](float deltaTime) {
                if (relationTip)
                {
                    relationTip->removeFromParent();  // 移除消息标签
                }
                }, 1.0f, "remove_message_key");  // 设置延时1秒后执行
        }
        }, WeatherChangeTime, "lambda_key_weather"); // 设置每1秒执行一次，使用一个唯一的键（lambda_key）标识调度
}