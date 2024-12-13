#include "MainCharacter.h"
#include "HelloWorldScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

#define LEFT 1
#define UP 0
#define RIGHT 3
#define DOWN 2
#define UP_DOWN_CONFLICT 4
#define LEFT_RIGHT_CONFLICT 5

#define CHARACTER 1

MainCharacter* MainCharacter::create(const std::string& filename) {
    MainCharacter* ret = new MainCharacter();
    if (ret && ret->init(filename)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MainCharacter::init(const std::string& filename) {
    if (!Sprite::initWithFile(filename)) {
        return false;
    }

    this->setVisible(false);
    
    // add physical property
    auto physics_body = PhysicsBody::createCircle(4, PhysicsMaterial(0.00001f, 0.0f, 0.01f));
    physics_body->setRotationEnable(false);
    physics_body->setVelocityLimit(200);
    physics_body->setContactTestBitmask(0xFFFFFFFF);
    physics_body->setPositionOffset(Vec2(0, -16));
    this->addComponent(physics_body);
    this->setTag(CHARACTER);

    //add contact event listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(MainCharacter::on_contact_begin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    addKeyboardListener();
    this->schedule(CC_SCHEDULE_SELECTOR(MainCharacter::update), 1.0 / 60); // 60 fps

    return true;
}

void MainCharacter::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_W: // W
            if (!movementkeys[DOWN])
                movementkeys[UP] = true;
            else
                movementkeys[UP_DOWN_CONFLICT] = true;
            break;
        case EventKeyboard::KeyCode::KEY_A: // A
            if (!movementkeys[RIGHT])
                movementkeys[LEFT] = true; 
            else
                movementkeys[LEFT_RIGHT_CONFLICT] = true;
            break;
        case EventKeyboard::KeyCode::KEY_S: // S
            if (!movementkeys[UP])
                movementkeys[DOWN] = true; 
            else
                movementkeys[UP_DOWN_CONFLICT] = true;
            break;
        case EventKeyboard::KeyCode::KEY_D: // D
            if (!movementkeys[LEFT])
                movementkeys[RIGHT] = true;
            else
                movementkeys[LEFT_RIGHT_CONFLICT] = true;
            break;
        default:
            break;
    }
}

void MainCharacter::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_W:
            movementkeys[UP] = false;
            if (movementkeys[UP_DOWN_CONFLICT]) {
                movementkeys[UP_DOWN_CONFLICT] = false;
                movementkeys[DOWN] = true;
            }
            break;
        case EventKeyboard::KeyCode::KEY_A:
            movementkeys[LEFT] = false;
            if (movementkeys[LEFT_RIGHT_CONFLICT]) {
                movementkeys[LEFT_RIGHT_CONFLICT] = false;
                movementkeys[RIGHT] = true;
            }
            break;
        case EventKeyboard::KeyCode::KEY_S:
            movementkeys[DOWN] = false;
            if (movementkeys[UP_DOWN_CONFLICT]) {
                movementkeys[UP_DOWN_CONFLICT] = false;
                movementkeys[UP] = true;
            }
            break;
        case EventKeyboard::KeyCode::KEY_D:
            movementkeys[RIGHT] = false;
            if (movementkeys[LEFT_RIGHT_CONFLICT]) {
                movementkeys[LEFT_RIGHT_CONFLICT] = false;
                movementkeys[LEFT] = true;
            }
            break;
        default:
            break;
    }
}

void MainCharacter::addKeyboardListener() {
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(MainCharacter::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(MainCharacter::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void MainCharacter::update(float delta) {
    Vec2 position = mainmap->getPosition();
    int mapWidth = mainmap->getMapSize().width;  
    int mapHeight = mainmap->getMapSize().height; 
    int tileWidth = mainmap->getTileSize().width * 2; 
    int tileHeight = mainmap->getTileSize().height * 2; 
    int mapwidth = mapWidth * tileWidth;
    int mapheight = mapHeight * tileHeight;

    Size characterSize = this->getContentSize();
    auto visibleSize = Director::getInstance()->getVisibleSize();

    double v = 200;

    this->getPhysicsBody()->applyForce(Vec2(v * (movementkeys[RIGHT] - movementkeys[LEFT]),
                                                     v * (movementkeys[UP] - movementkeys[DOWN])));
    if (!movementkeys[UP] && !movementkeys[DOWN] && !movementkeys[LEFT] && !movementkeys[RIGHT]) {
        this->getPhysicsBody()->setVelocity(Vec2(0, 0));
    }
    set_map_position_by_character();
    log("v_x = %f, v_y = %f", this->getPhysicsBody()->getVelocity().x, this->getPhysicsBody()->getVelocity().y);
    
    frame_count = (1 + frame_count) % 24;
    int step = frame_count / 6;
    
    int temp_dir = 4;
    if (movementkeys[UP] == 1) {
        temp_dir = UP;
    }
    else if (movementkeys[DOWN] == 1) {
        temp_dir = DOWN;
    }
    else if (movementkeys[LEFT] == 1) {
        temp_dir = LEFT;
    }
    else if (movementkeys[RIGHT] == 1) {
        temp_dir = RIGHT;
    }
    // animation
    for (int direction = 0; direction < 4; direction++) {
        for (int step = 0; step < 4; step++) {
            auto frame = SpriteFrame::create("MainCharacter/Emily.png", Rect(16 * step, 32 * ((temp_dir + 2) % 4), 16, 32));
            walking_animation[direction][step] = frame;
        }
    }
    if (temp_dir != 4) {
        dir = ((temp_dir + 2) % 4);
        animate_sprite->setSpriteFrame(walking_animation[dir][step]);
    } 
    else {
        animate_sprite->setSpriteFrame(walking_animation[dir][0]);
    }

    animate_sprite->setPosition(this->getPosition());
}

/****************************************************************************************************
                            Followed by Wang's code about map collision
**********************************************************************************************************/

void MainCharacter::set_map_position_by_character()
{
    int mapWidth = mainmap->getMapSize().width;
    int mapHeight = mainmap->getMapSize().height;
    int tileWidth = mainmap->getTileSize().width;
    int tileHeight = mainmap->getTileSize().height;
    int mapwidth = mapWidth * tileWidth;
    int mapheight = mapHeight * tileHeight;

    Size characterSize = this->getContentSize();
    auto visibleSize = Director::getInstance()->getVisibleSize();

    float scale = 2.0f;
    // x axis
    log("%f", this->getPositionX());
    if ((this->getPositionX() * scale) > visibleSize.width / 2 && (this->getPositionX() * scale) < mapwidth * scale - visibleSize.width / 2) {
        mainmap->setPositionX((-this->getPositionX() + mapwidth / 2) * 2 + visibleSize.width / 2);
        log("%f", (-this->getPositionX() + mapwidth / 2) * 2 + visibleSize.width / 2);
    }
    else if ((this->getPositionX() * scale) < visibleSize.width / 2) {
        mainmap->setPositionX(mapwidth);
    }
    else {
        mainmap->setPositionX(-mapwidth + visibleSize.width);
    }

    // y axis
    log("%f", this->getPositionY());
    if ((this->getPositionY() * scale) > visibleSize.height / 2 && (this->getPositionY() * scale) < mapheight * scale - visibleSize.height / 2) {
        mainmap->setPositionY((-this->getPositionY() + mapheight / 2) * 2 + visibleSize.height / 2);
        log("%f", (-this->getPositionY() + mapheight / 2) * 2 + visibleSize.height / 2);
    }
    else if ((this->getPositionY() * scale) < visibleSize.height / 2) {
        mainmap->setPositionY(mapheight);
    }
    else {
        mainmap->setPositionY(-mapheight + visibleSize.height);
    }
}


bool MainCharacter::on_contact_begin(PhysicsContact& contact)
{
    const int EDGE = 114;
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    if (nodeA && nodeB) {
        if (nodeA->getTag() == EDGE || nodeB->getTag() == EDGE) {
            GameScene* next_scene;
            if (dynamic_cast<GameScene*>(this->getScene())->scene_name_ == "newnewFarm")
                next_scene = GameScene::create("Town");
            else
                next_scene = GameScene::create("newnewFarm");
            Director::getInstance()->replaceScene(next_scene);
        }
    }

    return true;
}