#include "animals.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Task.h"
#include "global.h"
#include "explore.h"
#include "Date.h"
#include "MiniGame.h"
#include "ToolFactory.h"
#include "SeedFactory.h"
#include "AnimalFactory.h"
#include "FarmSceneFacade.h"

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

    auto visibleSize = Director::getInstance()->getVisibleSize();//��ȡ��ǰ��Ϸ��ͼ���ڵĳߴ�
    Vec2 origin = Director::getInstance()->getVisibleOrigin();//���д����ȡ��ǰ��Ϸ��ͼ���ڵ�ԭ������

    Scene::initWithPhysics();
    this->getPhysicsWorld()->setGravity(Vec2(0, 0));
    this->getPhysicsWorld()->setUpdateRate(1);
    // ���õ��Ի���
    //this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // ���ӳ�ʼ����ͼ
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
    map->setScale(mapscale);//��ͼ��������
    set_physical_map(map);
    map->getLayer("Back")->setLocalZOrder(-3);
    map->getLayer("Buildings")->setLocalZOrder(-2);

    auto task_layer = TaskLayer::create(task_manager);
    this->addChild(task_layer);

    // ��ȡ��ͼ�Ĵ�С
    mapWidth = map->getMapSize().width;  // �����ש����
    mapHeight = map->getMapSize().height; // �����ש����
    tileWidth = map->getTileSize().width; // ������ש�����ؿ���
    tileHeight = map->getTileSize().height; // ������ש�����ظ߶�
    map->setAnchorPoint(Vec2(0.5, 0.5));
    map->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    auto tileLayer = map->getLayer("soil"); // �滻Ϊ����ͼ������
    Vec2 tilePosition(1, 64); // ����������Ҫ��ѯ��ͼ��λ�ã���������Ͻ�
    if (scene_name_ == "newnewFarm") {
        int tileGID = tileLayer->getTileGIDAt(tilePosition, nullptr);
        CCLOG("Tile GID at: %d", tileGID);
    }

    ///////////////////////////////////////
    FarmSceneFacade::setupUI(this);
    ////////////////////////////////////////////////
    createRain();
    FarmSceneFacade::initCommonResources(this, map);
    weatherchange();

    auto date_layer = Date::create();
    this->addChild(date_layer);

    //��������
    character = MainCharacter::create("MainCharacter/transparent.png");    // this HelloWorld.png is set invisible, don't care
    character->setMap(map);//�����ͼ
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
    character->setPosition(x * tileWidth, y * tileWidth); // ��ʼλ��

    if (scene_name_ == "Town") {
        auto gobang_board = GobangBoard::create(map);
        map->addChild(gobang_board, 5);
    }

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
        map->addChild(chest, 0);
    }
    if (scene_name_ == "Town") {
        auto gobang_board = GobangBoard::create(map);
        map->addChild(gobang_board, 5);
    }
    MouseStatus = notTaken;//�������״̬

    //��ͼΪ���򣬴���NPC
    if (scene_name_ == "Town" && date != HOLIDAY_DATE) {
        FarmSceneFacade::initTownNPCs(this, map);
    }
    //��ͼΪũ��������newnew
    else if(scene_name_ == "newnewFarm") {
        FarmSceneFacade::initFarmScene(this, map);
    }

    return true;
}

//����ÿһ����ѡ��ÿ��ֻ��ѡһ��
void GameScene::CheckboxOnlyone() {
    // �����¼�������
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
    // �����¼�������
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
    // �����¼�������
    fishing->fishingcheckbox->addEventListener([this](Ref* sender, ui::CheckBox::EventType type) {
        switch (type) {
            case ui::CheckBox::EventType::SELECTED:
                fishing->isfishing = 1;
                CloseOtherCheckbox(fishing->fishingcheckbox);
                break;
            case ui::CheckBox::EventType::UNSELECTED:
                fishing->isfishing = 0;
                fishing->fishingcondition = 0;//����״̬Ϊ0

                break;
            default:
                break;
        }
        });
    // �����¼�������
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
    // �����¼�������
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
    // �����¼�������
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
    // �����¼�������
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
    // �����¼�������
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
    // �����¼�������
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
    // �����¼�������
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
    // �����¼�������
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
    // �����¼�������
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
    //��ȡ���ӷ�Χ
    auto visibleSize = Director::getInstance()->getVisibleSize();
    //��ȡ�������λ�ã���ʼԭ��Ϊ���Ͻǣ�
    EventMouse* e = static_cast<EventMouse*>(event);
    Vec2 clickPos = e->getLocation() / mapscale;
    //���������λ��ת��Ϊ����Ļ���½�Ϊԭ��
    clickPos.y = visibleSize.height / mapscale - clickPos.y;

    if (MouseStatus == notTaken) {//���δ��ռ��
        //��������� NPC ���¼�
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

//���ܹ�ϵ����
void GameScene::DialogClose(NPC* npc)
{
    npc->startMovement();
    //���ܶ���ʱ
    if (npc->relation == 100) {
        relationFull->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 3));
        this->addChild(relationFull);
        this->scheduleOnce([this](float) {
            relationFull->removeFromParent();
            }, 1.0f, "hide_relation_full_key");
    }
    else {
        //�Ի��������ܶ�ʱ
        if (npc->Dialog_NPC->ChangeToClose == 1) {
            npc->relation += 10;
            // ����һ����ǩ����ʾ��Ϣ
            relationTip = Label::createWithSystemFont("RealtionDegree+10!", "Arial", 30);
            relationTip->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
                Director::getInstance()->getVisibleSize().height / 3));

            // ����ǩ���ӵ���ǰ������
            this->addChild(relationTip);

            // ���ö�ʱ����1����Զ��Ƴ���Ϣ
            this->scheduleOnce([this](float deltaTime) {
                if (relationTip)
                {
                    relationTip->removeFromParent();  // �Ƴ���Ϣ��ǩ
                }
                }, 1.0f, "remove_message_key");  // ������ʱ1���ִ��
        }
        //���NPC�����������ܶ�ʱ
        else if (npc->Dialog_NPC->ChangeToClose == 2) {
            npc->relation += 70;
            // ����һ����ǩ����ʾ��Ϣ
            relationTip = Label::createWithSystemFont("RealtionDegree+70!", "Arial", 30);
            relationTip->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
                Director::getInstance()->getVisibleSize().height / 3));

            // ����ǩ���ӵ���ǰ������
            this->addChild(relationTip);

            // ���ö�ʱ����1����Զ��Ƴ���Ϣ
            this->scheduleOnce([this](float deltaTime) {
                if (relationTip)
                {
                    relationTip->removeFromParent();  // �Ƴ���Ϣ��ǩ
                }
                }, 1.0f, "remove_message_key");  // ������ʱ1���ִ��
        }
    }
    npc->Dialog_NPC->ChangeToClose = 0;
}

//�ر�������ѡ��
void GameScene::CloseOtherCheckbox(ui::CheckBox* selectedCheckbox) {
    if (selectedCheckbox != tools->hoecheckbox) {
        tools->hoecheckbox->setSelected(false); // ȡ��ѡ����һ����ѡ��
        tools->ishoe = 0;
    }
    if (selectedCheckbox != pickaxe->pickaxecheckbox) {
        pickaxe->pickaxecheckbox->setSelected(false); // ȡ��ѡ����һ����ѡ��
        pickaxe->ispickaxe = 0;
    }
    if (selectedCheckbox != fishing->fishingcheckbox) {
        fishing->fishingcheckbox->setSelected(false); // ȡ��ѡ����һ����ѡ��
        fishing->isfishing = 0;
        fishing->fishingcondition = 0;
    }
    if (selectedCheckbox != seeds->seedscheckbox) {
        seeds->seedscheckbox->setSelected(false); // ȡ��ѡ����һ����ѡ��
        seeds->isseeds = 0;
    }
    if (selectedCheckbox != seedstwo->seedstwocheckbox) {
        seedstwo->seedstwocheckbox->setSelected(false); // ȡ��ѡ����һ����ѡ��
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
    //// ������ǩ�����ӵ�������
    //auto label = Label::createWithTTF(std::to_string(money), "fonts/Marker Felt.ttf", 24);
    //label->setPosition(Vec2(visibleSize.width * 8 / 10, visibleSize.height * 9 / 10)); // ���ñ�ǩ��ʾ��λ��
    //this->addChild(label, 1);

    //// ����һ����ʱ������������Ա���ÿ֡���±�ǩ
    //this->schedule([this, label](float dt) {
    //    label->setString(std::to_string(money));
    //    }, "update_label_key");
    std::string moneyString = "Money: " + std::to_string(money);
    auto label = Label::createWithTTF(moneyString, "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(visibleSize.width * 8 / 10, visibleSize.height * 9 / 10)); // ���ñ�ǩ��ʾ��λ��
    label->setColor(Color3B(0,0,0));
    this->addChild(label, 1);

    // ����һ����ʱ������������Ա���ÿ֡���±�ǩ
    this->schedule([this, label](float dt) {
        label->setString("Money: " + std::to_string(money));
        }, "update_label_key");
}
void GameScene::NewFishingListening() {
    //CCLOG("66666%d",NewFishingListeningbool);
    if (1) {
        
        fishing->fishingcheckbox->setEnabled(false);
        this->schedule([this](float dt) {
            // �����д�����߼����������ض�����ʱִ��
            if (experience >= NewFishingExp) {
                auto VisibleSize = cocos2d::Director::getInstance()->getVisibleSize();
                fishing->fishingcheckbox->setEnabled(true);
                //������ʾ��ǩ
                if (NewFishingListeningbool == 0) {
                    auto label = Label::createWithSystemFont("You have enough experience.You can fish now.", "Arial", 24);
                    label->setPosition(Vec2(VisibleSize.width / 2, VisibleSize.height / 2));
                    auto sprite = Sprite::create("buysellfailscene.png");//����
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
                    // �������ӳ�ִ�У�n�볤����

                }
                NewFishingListeningbool = 1;
                this->unschedule("update_key_fishing");  // ȡ������
            }

            // �˴���������������Ҫִ�е��߼�
            }, 0.2f, "update_key_fishing");  // ʹ��һ��������ʶ�������
      
    }

}
void GameScene::NewPickaxeListening()//�����ڿ�
{
    if (1) {
        pickaxe->pickaxecheckbox->setEnabled(false);
        this->schedule([this](float dt) {
            // �����д�����߼����������ض�����ʱִ��
            if (experience >= NewPickaxeExp) {
                auto VisibleSize = cocos2d::Director::getInstance()->getVisibleSize();
                pickaxe->pickaxecheckbox->setEnabled(true);
                //������ʾ��ǩ
                if (NewPickaxeListeningbool == 0) {
                    auto label = Label::createWithSystemFont("You have enough experience.You can mine now.", "Arial", 24);
                    label->setPosition(Vec2(VisibleSize.width / 2, VisibleSize.height / 2));
                    auto sprite = Sprite::create("buysellfailscene.png");//����
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
                    // �������ӳ�ִ�У�n�볤����

                }
                NewPickaxeListeningbool = 1;
                this->unschedule("update_key_pickaxe");  // ȡ������
            }

            // �˴���������������Ҫִ�е��߼�
            }, 0.2f, "update_key_pickaxe");  // ʹ��һ��������ʶ�������
       
    }

}
void GameScene::NewCookLayerListening()//�������
{
    if (1) {
        cookLayer->listener->setEnabled(false); // ���ð�������
        this->schedule([this](float dt) {
            // �����д�����߼����������ض�����ʱִ��
            if (experience >= NewCookLayerExp) {
                auto VisibleSize = cocos2d::Director::getInstance()->getVisibleSize();
                cookLayer->listener->setEnabled(true); // ���ð�������
                //������ʾ��ǩ
                if (NewCookLayerListeningbool == 0) {
                    auto label = Label::createWithSystemFont("You have enough experience.You can cook now.", "Arial", 24);
                    label->setPosition(Vec2(VisibleSize.width / 2, VisibleSize.height / 2));
                    auto sprite = Sprite::create("buysellfailscene.png");//����
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
                    // �������ӳ�ִ�У�n�볤����

                }
                NewCookLayerListeningbool = 1;
                this->unschedule("update_key_cook");  // ȡ������
            }

            // �˴���������������Ҫִ�е��߼�
            }, 0.2f, "update_key_cook");  // ʹ��һ��������ʶ�������
      
    }

}


void GameScene::createRain() {
    // ��ʼ�������㣬����Ϊȫ����ɫ����
    auto darkOverlay = cocos2d::Sprite::create();
    darkOverlay->setTextureRect(cocos2d::Rect(0, 0, cocos2d::Director::getInstance()->getVisibleSize().width, cocos2d::Director::getInstance()->getVisibleSize().height));
    darkOverlay->setColor(cocos2d::Color3B(0, 0, 0)); // ������ɫΪ��ɫ
    darkOverlay->setOpacity(128); // ����͸����Ϊ128����Χ0-255��0��ȫ͸����255��͸����
    darkOverlay->setPosition(cocos2d::Director::getInstance()->getVisibleSize() / 2); // ����λ��
    //darkOverlay->setVisible(false); // Ĭ������
    this->addChild(darkOverlay);

    // ʾ����չʾ������
    // ��������ϵͳ
    auto rainParticle = cocos2d::ParticleRain::create();
    auto VisibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    rainParticle->setPosition(VisibleSize.width/2, VisibleSize.height); // ��������ϵͳ��λ��Ϊ��Ļ����
    rainParticle->setScale(6.0f); // �����������ţ�����Ч���Ĵ�С

    // �Զ�������������
    rainParticle->setLife(1.0f);  // ÿ����ε�����ʱ��
    rainParticle->setSpeed(200);  // ����ٶ�
    rainParticle->setTexture(cocos2d::Director::getInstance()->getTextureCache()->addImage("raindrop.png")); // ������ε�����ͼ

    // ������������
    rainParticle->setTotalParticles(500); // �������������������磬500��
    rainParticle->setEmissionRate(100); // ÿ�뷢����������������磬100��
    // ��������ϵͳ������
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
    darkOverlay->setColor(cocos2d::Color3B(128, 50, 0)); // ������ɫΪ��ɫ
    darkOverlay->setOpacity(128); // ����͸����Ϊ128����Χ0-255��0��ȫ͸����255��͸����
    darkOverlay->setPosition(cocos2d::Director::getInstance()->getVisibleSize() / 2); // ����λ��
    //darkOverlay->setVisible(false); // Ĭ������
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
            // ����һ����ǩ����ʾ��Ϣ
            relationTip = Label::createWithSystemFont("Weather changes!", "Arial", 30);
            relationTip->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 3));

            // ����ǩ���ӵ���ǰ������
            this->addChild(relationTip);

            // ���ö�ʱ����1����Զ��Ƴ���Ϣ
            this->scheduleOnce([this](float deltaTime) {
                if (relationTip)
                {
                    relationTip->removeFromParent();  // �Ƴ���Ϣ��ǩ
                }
                }, 1.0f, "remove_message_key");  // ������ʱ1���ִ��
        }
        else if (weather == 5) {
            createRain();
            // ����һ����ǩ����ʾ��Ϣ
            relationTip = Label::createWithSystemFont("Weather changes!", "Arial", 30);
            relationTip->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
                Director::getInstance()->getVisibleSize().height / 3));

            // ����ǩ���ӵ���ǰ������
            this->addChild(relationTip);

            // ���ö�ʱ����1����Զ��Ƴ���Ϣ
            this->scheduleOnce([this](float deltaTime) {
                if (relationTip)
                {
                    relationTip->removeFromParent();  // �Ƴ���Ϣ��ǩ
                }
                }, 1.0f, "remove_message_key");  // ������ʱ1���ִ��
        }
        }, WeatherChangeTime, "lambda_key_weather"); // ����ÿ1��ִ��һ�Σ�ʹ��һ��Ψһ�ļ���lambda_key����ʶ����
}