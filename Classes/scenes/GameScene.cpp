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