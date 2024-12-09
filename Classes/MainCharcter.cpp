#include "MainCharacter.h"
#include "SimpleAudioEngine.h"

#define LEFT 1
#define UP 0
#define RIGHT 3
#define DOWN 2


MainCharacter* MainCharacter::create(const std::string& filename) {

    MainCharacter* ret = new MainCharacter();
    if (ret && ret->init(filename)) {
        ret->autorelease(); // 自动释放内存
        return ret;
    }
    CC_SAFE_DELETE(ret); // 如果创建失败，安全删除
    return nullptr;
}

bool MainCharacter::init(const std::string& filename) {
    if (!Sprite::initWithFile(filename)) { // 使用传入的文件名初始化精灵
        return false;
    }

    this->setScale(0.25);

    //添加键盘监听事件
    addKeyboardListener();
    this->schedule(CC_SCHEDULE_SELECTOR(MainCharacter::update), 1.0 / 60); //时刻刷新事件

    return true;
}

void MainCharacter::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_W: // W
            movementkeys[UP] = true; // 向上移动
            break;
        case EventKeyboard::KeyCode::KEY_A: // A
            movementkeys[LEFT] = true; // 向左移动
            break;
        case EventKeyboard::KeyCode::KEY_S: // S
            movementkeys[DOWN] = true; // 向下移动
            break;
        case EventKeyboard::KeyCode::KEY_D: // D
            movementkeys[RIGHT] = true; // 向右移动
            break;
        default:
            break;
    }
}

void MainCharacter::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_W:
            movementkeys[UP] = false;
            break;
        case EventKeyboard::KeyCode::KEY_A:
            movementkeys[LEFT] = false;
            break;
        case EventKeyboard::KeyCode::KEY_S:
            movementkeys[DOWN] = false;
            break;
        case EventKeyboard::KeyCode::KEY_D:
            movementkeys[RIGHT] = false;
            break;
        default:
            break;
    }
}
void MainCharacter::addKeyboardListener() {
    // 添加键盘事件监听
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(MainCharacter::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(MainCharacter::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
// 更新角色位置
void MainCharacter::update(float delta) {
    // 获取当前地图位置
    Vec2 position = mainmap->getPosition();
    int mapWidth = mainmap->getMapSize().width;  // 横向瓷砖数量
    int mapHeight = mainmap->getMapSize().height; // 纵向瓷砖数量
    int tileWidth = mainmap->getTileSize().width * 2; // 单个瓷砖的像素宽度
    int tileHeight = mainmap->getTileSize().height * 2; // 单个瓷砖的像素高度
    int mapwidth = mapWidth * tileWidth;
    int mapheight = mapHeight * tileHeight;//地图长宽
    // 获取角色的尺寸
    Size characterSize = this->getContentSize();
    auto visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸
    // 根据按下的键移动角色
    double v = 200;
    // 持续移动逻辑
    if (movementkeys[UP] && check_empty(UP)) { // W
        if (this->getPositionY() < visibleSize.height / 2) {
            this->setPositionY(this->getPositionY() + v * delta);
        }
        else {
            position.y -= v * delta; // 向上移动
        }
    }
    if (movementkeys[LEFT] && check_empty(LEFT)) { // A
        if (this->getPositionX() > visibleSize.width / 2) {
            this->setPositionX(this->getPositionX() - v * delta);
        }
        else {
            position.x += v * delta; // 向左移动
        }
    }
    if (movementkeys[DOWN] && check_empty(DOWN)) { // S
        if (this->getPositionY() > visibleSize.height / 2) {
            this->setPositionY(this->getPositionY() - v * delta);
        }
        else {
            position.y += v * delta; // 向下移动
        }
    }
    if (movementkeys[RIGHT] && check_empty(RIGHT)) { // D
        if (this->getPositionX() < visibleSize.width / 2) {
            this->setPositionX(this->getPositionX() + v * delta);
        }
        else {
            position.x -= v * delta; // 向右移动
        }
    }

    
    //检查是否超出边界
    if (position.x > mapwidth / 2) {
        position.x = mapwidth / 2;
        auto character_x = this->getPositionX();
        if (character_x > 0) {
            character_x -= v * delta;
        }
        this->setPositionX(character_x);
    }
    if (position.y > mapheight / 2) {
        position.y = mapheight / 2;
        auto character_y = this->getPositionY();
        if (character_y > 0) {
            character_y -= v * delta;
        }
        this->setPositionY(character_y);
    }
    if (position.x < visibleSize.width - mapwidth / 2) {
        position.x = visibleSize.width - mapwidth / 2;
        auto character_x = this->getPositionX();
        if (character_x < visibleSize.width) {
            character_x += v * delta;
        }
        this->setPositionX(character_x);
    }
    if (position.y < visibleSize.height - mapheight / 2) {
        position.y = visibleSize.height - mapheight / 2;
        auto character_y = this->getPositionY();
        if (character_y < visibleSize.height) {
            character_y += v * delta;
        }
        this->setPositionY(character_y);
    }
    CCPoint temp_position;
    temp_position.x = int(position.x), temp_position.y = int(position.y);
    mainmap->setPosition(position);
    // 创建动画帧
    Vector<SpriteFrame*> animationFramesdown;//向下走动画
    for (int i = 1; i <= 3; i++) {
        auto frame = SpriteFrame::create(StringUtils::format("characterdown%d.png", i).c_str(), Rect(0, 0, 100, 160));
        animationFramesdown.pushBack(frame);
    }

    Vector<SpriteFrame*> animationFramesright;//向右走动画
    for (int i = 1; i <= 3; i++) {
        auto frame = SpriteFrame::create(StringUtils::format("characterright%d.png", i).c_str(), Rect(0, 0, 100, 160));
        animationFramesright.pushBack(frame);
    }

    Vector<SpriteFrame*> animationFramesleft;//向左走动画
    for (int i = 1; i <= 3; i++) {
        auto frame = SpriteFrame::create(StringUtils::format("characterleft%d.png", i).c_str(), Rect(0, 0, 100, 160));
        animationFramesleft.pushBack(frame);
    }

    Vector<SpriteFrame*> animationFramesup;//向上走动画
    for (int i = 1; i <= 3; i++) {
        auto frame = SpriteFrame::create(StringUtils::format("characterup%d.png", i).c_str(), Rect(0, 0, 100, 160));
        animationFramesup.pushBack(frame);
    }
    // 创建动画
    walkAnimationdown = Animation::createWithSpriteFrames(animationFramesdown, 0.1f);
    walkAnimationright = Animation::createWithSpriteFrames(animationFramesright, 0.1f);
    walkAnimationleft = Animation::createWithSpriteFrames(animationFramesleft, 0.1f);
    walkAnimationup = Animation::createWithSpriteFrames(animationFramesup, 0.1f);
    auto walkAnimationnext = Animation::create();
  
    frame_count = (1 + frame_count) % 24;
    if (frame_count == 0) {
        // 播放走路动画
        if (movementkeys[DOWN]) {
            walkAnimationnext = walkAnimationdown->clone();
            staticnext = "characterdown2.png";
        }
        else if (movementkeys[UP]) {
            walkAnimationnext = walkAnimationup->clone();
            staticnext = "characterup2.png";
        }
        else if (movementkeys[RIGHT]) {
            walkAnimationnext = walkAnimationright->clone();
            staticnext = "characterright2.png";
        }
        else if (movementkeys[LEFT]) {
            walkAnimationnext = walkAnimationleft->clone();
            staticnext = "characterleft2.png";
        }

        // 创建 Animate 动作
        if (movementkeys[UP] || movementkeys[LEFT] || movementkeys[DOWN] || movementkeys[RIGHT]) {
            auto animateActionnext = Animate::create(walkAnimationnext);
            this->runAction(RepeatForever::create(animateActionnext));
        }
        else {
            frame_count = 23;
            this->stopAllActions(); // 停止所有动作

            this->setTexture(staticnext); // 切换为静态图像
        }
    }
}

/****************************************************************************************************
                            Followed by Wang's code about map collision
**********************************************************************************************************/

/**
* @para direction 0-3 represents up, left, down, right
*/
bool MainCharacter::check_empty(int direction)
{
    Vec2 position = mainmap->getPosition();
    auto buildings_layer = mainmap->getLayer("Buildings");
    const int scale_multiple = 2;
    int map_width = mainmap->getMapSize().width;  // 横向瓷砖数量
    int map_height = mainmap->getMapSize().height; // 纵向瓷砖数量
    int tile_width = mainmap->getTileSize().width * scale_multiple; // 单个瓷砖的像素宽度
    int tile_height = mainmap->getTileSize().height * scale_multiple; // 单个瓷砖的像素高度


    CCPoint character_tile_coordinate, detect_tile_coordinate_1, detect_tile_coordinate_2;
    character_tile_coordinate.x = mainmap->getMapSize().width / 2 + (-position.x + this->getPositionX()) / tile_width;
    character_tile_coordinate.y = mainmap->getMapSize().height / 2 + (-this->getPositionY() + position.y) / tile_height;
    unsigned int building_GID_1, building_GID_2;
    log("character_tile_coordinate = (%.2f, %.2f)", character_tile_coordinate.x, character_tile_coordinate.y);

    double mistake_distance = 0.2;

    if (direction == UP) {
        detect_tile_coordinate_1 = ccp(character_tile_coordinate.x - 0.5, character_tile_coordinate.y - mistake_distance);
        building_GID_1 = buildings_layer->getTileGIDAt(detect_tile_coordinate_1);
        detect_tile_coordinate_2 = ccp(character_tile_coordinate.x + 0.5, character_tile_coordinate.y - mistake_distance);
        building_GID_2 = buildings_layer->getTileGIDAt(detect_tile_coordinate_2);
    }
    else if (direction == LEFT) {
        detect_tile_coordinate_1 = ccp(character_tile_coordinate.x - (0.5 + mistake_distance), character_tile_coordinate.y + 1.2);
        building_GID_1 = buildings_layer->getTileGIDAt(detect_tile_coordinate_1);
        detect_tile_coordinate_2 = ccp(character_tile_coordinate.x - (0.5 + mistake_distance), character_tile_coordinate.y);
        building_GID_2 = buildings_layer->getTileGIDAt(detect_tile_coordinate_2);
    }
    else if (direction == DOWN) {
        detect_tile_coordinate_1 = ccp(character_tile_coordinate.x - 0.5, character_tile_coordinate.y + 1.2 + mistake_distance);
        building_GID_1 = buildings_layer->getTileGIDAt(detect_tile_coordinate_1);
        detect_tile_coordinate_2 = ccp(character_tile_coordinate.x + 0.5, character_tile_coordinate.y + 1.2 + mistake_distance);
        building_GID_2 = buildings_layer->getTileGIDAt(detect_tile_coordinate_2);
    }
    else if (direction == RIGHT) {
        detect_tile_coordinate_1 = ccp(character_tile_coordinate.x + (0.5 + mistake_distance), character_tile_coordinate.y + 1.2);
        building_GID_1 = buildings_layer->getTileGIDAt(detect_tile_coordinate_1);
        detect_tile_coordinate_2 = ccp(character_tile_coordinate.x + (0.5 + mistake_distance), character_tile_coordinate.y);
        building_GID_2 = buildings_layer->getTileGIDAt(detect_tile_coordinate_2);
    }
    // log("detect_tile_coordinate = (%.2f, %.2f)", detect_tile_coordinate.x, detect_tile_coordinate.y);
    if (building_GID_1 || building_GID_2) {
        return 0;
    }
    else {
        return 1;
    }
}