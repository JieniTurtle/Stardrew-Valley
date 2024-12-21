#include "animals.h"
#include"MainCharacter.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

//路径初始化
void Animal::setPath(const std::vector<Vec2>& newPath)
{
    Animalpath = newPath;
    currentPathIndex = 0;  // 重置路径索引
    setPosition(Animalpath[currentPathIndex]);  // 设置 Animal 的起始位置
}

void Animal::updatemove(float dt)
{
    if (Animalpath.empty()) return;

    // 获取目标位置
    Vec2 targetPosition = Animalpath[currentPathIndex];
    Vec2 currentPosition = this->getPosition();

    // 计算当前位置和目标位置的距离
    Vec2 direction = targetPosition - currentPosition;
    float distance = direction.length();

    // 如果到达目标，切换到下一个路径点
    if (distance < 1.0f) {
        currentPathIndex = (currentPathIndex + 1) % Animalpath.size();  // 循环路径
    }

    // 移动 Animal
    Vec2 moveDirection = direction.getNormalized();
    setPosition(currentPosition + moveDirection * speed * dt);

    // 根据方向播放动画
    if (fabs(moveDirection.x) > fabs(moveDirection.y)) { // 水平移动
        if (moveDirection.x > 0) {
            playAnimation("right");
        }
        else {
            playAnimation("left");
        }
    }
    else { // 垂直移动
        if (moveDirection.y > 0) {
            playAnimation("up");
        }
        else {
            playAnimation("down");
        }
    }
}

void Animal::playAnimation(const std::string& direction)
{
    //!getActionByTag(1)
    // 如果当前动画与目标方向相同，直接返回
    if (direction == "up" && 1) {

        runAction(RepeatForever::create(moveup));
    }
    else if (direction == "down" && 1) {
        runAction(RepeatForever::create(movedown));
    }
    else if (direction == "left" && 1) {
        runAction(RepeatForever::create(moveleft));
    }
    else if (direction == "right" && 1) {
        runAction(RepeatForever::create(moveright));
    }
}

int Cow::Animalsize_x = 125;
int Cow::Animalsize_y = 125;

int Cow::Animalorder_up = 2;
int Cow::Animalorder_left = 3;
int Cow::Animalorder_right = 1;
int Cow::Animalorder_down = 0;

Cow* Cow::create(const std::string& filename)
{
    Cow* cow = new Cow();
    if (cow) {
        cow->animalName = filename;
        if (cow->init()) {
            cow->autorelease(); // 自动释放内存
            return cow;
        }
    }
    CC_SAFE_DELETE(cow); // 如果创建失败，安全删除
    return nullptr;
}

void Cow::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (isNearSprite && keyCode == cocos2d::EventKeyboard::KeyCode::KEY_E && wheat_number > 0)
    {
        if (weather == 1) {
            happiness = happiness / 2; // 减半快乐值
        }
        else if (weather == 2) {
            happiness = happiness / 2; // 减半快乐值
        }
        cow_feed_label->setVisible(true); // 显示文字
        if (happiness <= 50&&happiness>=0)
            milk_number++;
        else if(happiness >= 50&&happiness<=90)
            milk_number+=2;
        else
            milk_number+=4;
            wheat_number--;
        // 启动定时器，3秒后隐藏文字
        this->scheduleOnce(CC_SCHEDULE_SELECTOR(Cow::hideLabel), 3.0f);
    }
    if (isNearSprite && keyCode == cocos2d::EventKeyboard::KeyCode::KEY_Q)
    {
        cow_touch_label->setVisible(true); // 显示文字
        happiness += 10; // 加快乐值
        // 启动定时器，3秒后隐藏文字
        this->scheduleOnce(CC_SCHEDULE_SELECTOR(Cow::hideLabel), 3.0f);
    }
}

// 添加键盘事件监听
void Cow::addKeyboardListener() {
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Cow::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

//标签隐藏
void Cow::hideLabel(float dt)
{
    cow_feed_label->setVisible(false); // 隐藏文字
    cow_touch_label->setVisible(false); // 隐藏文字
}

void Cow::isMainCharNear(float delta)
{
    Vec2 sprite1Position = mainChar->getPosition(); // 获取主角的位置(相对屏幕)
    Vec2 sprite2Position = this->getPosition(); // 获取地图的位置(相对地图左下角)

    float distance = sprite1Position.distance(sprite2Position);

    // 如果距离小于某个值，设置标志位
    if (distance < 50)
    {
        isNearSprite = true;
    }
    else
    {
        isNearSprite = false;
    }
}

//初始化
bool Cow::init()
{
    //创建纹理
    Texture2D* Move = Director::getInstance()->getTextureCache()->addImage("Animal/" + animalName + "/texture.png");

    if (!Sprite::initWithFile("Animal/" + animalName + "/static.png")) { return false; }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    speed = 50.0f;//设置速度
    currentPathIndex = 0;//初始路径
    ifSelected = false;//标记Animal未被选中

    // 设置位置更新函数
    schedule([=](float dt) {updatemove(dt); }, 0.0f, "animal_updatemove_key");

    Vector<SpriteFrame*> Animalup;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * Animalsize_x, Animalorder_up * Animalsize_y, Animalsize_x, Animalsize_y));
        Animalup.pushBack(frame);
    }
    Animation* uup = Animation::createWithSpriteFrames(Animalup, 0.3f);
    moveup = Animate::create(uup);
    moveup->setTag(1);

    Vector<SpriteFrame*>Animalleft;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * Animalsize_x, Animalorder_left * Animalsize_y, Animalsize_x, Animalsize_y));
        Animalleft.pushBack(frame);
    }
    Animation* lleft = Animation::createWithSpriteFrames(Animalleft, 0.3f);
    moveleft = Animate::create(lleft);
    moveleft->setTag(2);

    Vector<SpriteFrame*>Animalright;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * Animalsize_x, Animalorder_right * Animalsize_y, Animalsize_x, Animalsize_y));
        Animalright.pushBack(frame);
    }
    Animation* rright = Animation::createWithSpriteFrames(Animalright, 0.3f);
    moveright = Animate::create(rright);
    moveright->setTag(3);

    Vector<SpriteFrame*>Animaldown;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * Animalsize_x, Animalorder_down * Animalsize_y, Animalsize_x, Animalsize_y));
        Animaldown.pushBack(frame);
    }
    Animation* ddown = Animation::createWithSpriteFrames(Animaldown, 0.3f);
    movedown = Animate::create(ddown);
    movedown->setTag(4);


    // 创建文字标签
    cow_feed_label = cocos2d::Label::createWithSystemFont("The cow has been fed.Obtain Milk x1", "Arial", 25);
    cow_feed_label->setVisible(false); // 初始时隐藏
    this->addChild(cow_feed_label);
    cow_feed_label->setPosition(cocos2d::Vec2(40, 120));

    cow_touch_label = cocos2d::Label::createWithSystemFont("The cow has been touched.Happiess+10", "Arial", 25);
    cow_touch_label->setVisible(false); // 初始时隐藏
    this->addChild(cow_touch_label);
    cow_touch_label->setPosition(cocos2d::Vec2(40, 120));

    Cow::addKeyboardListener();
    this->schedule(CC_SCHEDULE_SELECTOR(Cow::isMainCharNear), 0.1f);
    this->schedule(CC_SCHEDULE_SELECTOR(Cow::decreaseHappiness), 20.0f);

    return true;
}

void Cow::move(Cow* cow, TMXTiledMap* map) {
    map->addChild(cow);
    cow->setScale(0.5f);
    cow->setPosition(Vec2(950, 765));
    Sequence* move_cow = Sequence::create(cow->moveup, cow->moveright, cow->moveleft, cow->movedown, NULL);
    cow->runAction(RepeatForever::create(move_cow));
    std::vector<Vec2> path_cow = { Vec2(950, 765), Vec2(950, 720), Vec2(1070, 720),Vec2(1070, 765) };
    cow->setPath(path_cow);
}

void Cow::decreaseHappiness(float delta) {
    if (happiness > 5)
        happiness -= 5;
}

int Sheep::Animalsize_x = 32;
int Sheep::Animalsize_y = 33;

int Sheep::Animalorder_up = 2;
int Sheep::Animalorder_left = 3;
int Sheep::Animalorder_right = 1;
int Sheep::Animalorder_down = 0;

Sheep* Sheep::create(const std::string& filename)
{
    Sheep* sheep = new Sheep();
    if (sheep) {
        sheep->animalName = filename;
        if (sheep->init()) {
            sheep->autorelease(); // 自动释放内存
            return sheep;
        }
    }
    CC_SAFE_DELETE(sheep); // 如果创建失败，安全删除
    return nullptr;
}

void Sheep::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (isNearSprite && keyCode == cocos2d::EventKeyboard::KeyCode::KEY_E && wheat_number > 0)
    {
        sheep_feed_label->setVisible(true); // 显示文字
        if (happiness <= 50 && happiness >= 0)
            wool_number++;
        else if (happiness >= 50 && happiness <= 90)
            wool_number += 2;
        else
            wool_number += 4;
        wheat_number--;        // 启动定时器，3秒后隐藏文字
        this->scheduleOnce(CC_SCHEDULE_SELECTOR(Sheep::hideLabel), 3.0f);
    }
    if (isNearSprite && keyCode == cocos2d::EventKeyboard::KeyCode::KEY_Q)
    {
        sheep_touch_label->setVisible(true); // 显示文字
        happiness += 10; // 扣除快乐值
        // 启动定时器，3秒后隐藏文字
        this->scheduleOnce(CC_SCHEDULE_SELECTOR(Cow::hideLabel), 3.0f);
    }
}

// 添加键盘事件监听
void Sheep::addKeyboardListener() {
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Sheep::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

//标签隐藏
void Sheep::hideLabel(float dt)
{
    sheep_feed_label->setVisible(false); // 隐藏文字
    sheep_touch_label->setVisible(false); // 隐藏文字
}

//初始化
bool Sheep::init()
{
    //创建纹理
    Texture2D* Move = Director::getInstance()->getTextureCache()->addImage("Animal/" + animalName + "/texture.png");

    if (!Sprite::initWithFile("Animal/" + animalName + "/static.png")) { return false; }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    speed = 50.0f;//设置速度
    currentPathIndex = 0;//初始路径
    ifSelected = false;//标记Animal未被选中

    // 设置位置更新函数
    schedule([=](float dt) {updatemove(dt); }, 0.0f, "animal_updatemove_key");

    Vector<SpriteFrame*> Animalup;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * Animalsize_x, Animalorder_up * Animalsize_y, Animalsize_x, Animalsize_y));
        Animalup.pushBack(frame);
    }
    Animation* uup = Animation::createWithSpriteFrames(Animalup, 0.3f);
    moveup = Animate::create(uup);
    moveup->setTag(1);

    Vector<SpriteFrame*>Animalleft;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * Animalsize_x, Animalorder_left * Animalsize_y, Animalsize_x, Animalsize_y));
        Animalleft.pushBack(frame);
    }
    Animation* lleft = Animation::createWithSpriteFrames(Animalleft, 0.3f);
    moveleft = Animate::create(lleft);
    moveleft->setTag(2);

    Vector<SpriteFrame*>Animalright;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * Animalsize_x, Animalorder_right * Animalsize_y, Animalsize_x, Animalsize_y));
        Animalright.pushBack(frame);
    }
    Animation* rright = Animation::createWithSpriteFrames(Animalright, 0.3f);
    moveright = Animate::create(rright);
    moveright->setTag(3);

    Vector<SpriteFrame*>Animaldown;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * Animalsize_x, Animalorder_down * Animalsize_y, Animalsize_x, Animalsize_y));
        Animaldown.pushBack(frame);
    }
    Animation* ddown = Animation::createWithSpriteFrames(Animaldown, 0.3f);
    movedown = Animate::create(ddown);
    movedown->setTag(4);

    // 创建文字标签
    sheep_feed_label = cocos2d::Label::createWithSystemFont("The sheep has been fed.Obtain Wool x1", "Arial", 10);
    sheep_feed_label->setVisible(false); // 初始时隐藏
    this->addChild(sheep_feed_label);
    sheep_feed_label->setPosition(cocos2d::Vec2(20, 40));

    sheep_touch_label = cocos2d::Label::createWithSystemFont("The sheep has been touched.Happiess+10", "Arial", 10);
    sheep_touch_label->setVisible(false); // 初始时隐藏
    this->addChild(sheep_touch_label);
    sheep_touch_label->setPosition(cocos2d::Vec2(20, 40));

    Sheep::addKeyboardListener();
    this->schedule(CC_SCHEDULE_SELECTOR(Sheep::isMainCharNear), 0.1f);
    this->schedule(CC_SCHEDULE_SELECTOR(Sheep::decreaseHappiness), 20.0f);

    return true;
}

void Sheep::isMainCharNear(float delta)
{
    Vec2 sprite1Position = mainChar->getPosition(); // 获取主角的位置(相对屏幕)
    Vec2 sprite2Position = this->getPosition(); // 获取地图的位置(相对地图左下角)

    float distance = sprite1Position.distance(sprite2Position);

    // 如果距离小于某个值，设置标志位
    if (distance < 50)
    {
        isNearSprite = true;
    }
    else
    {
        isNearSprite = false;
    }
}

void Sheep::move(Sheep* sheep, TMXTiledMap* map) {
    map->addChild(sheep);
    sheep->setScale(1.5f);
    sheep->setPosition(Vec2(950, 500));
    Sequence* move_cow = Sequence::create(sheep->moveup, sheep->moveright, sheep->moveleft, sheep->movedown, NULL);
    sheep->runAction(RepeatForever::create(move_cow));
    std::vector<Vec2> path_cow = { Vec2(950, 500), Vec2(950, 545), Vec2(1070, 500),Vec2(1070, 545) };
    sheep->setPath(path_cow);
}

void Sheep::decreaseHappiness(float delta) {
    if (happiness > 5)
        happiness -= 5;
}

int Chicken::Animalsize_x = 16;
int Chicken::Animalsize_y = 20;

int Chicken::Animalorder_up = 2;
int Chicken::Animalorder_left = 3;
int Chicken::Animalorder_right = 1;
int Chicken::Animalorder_down = 0;

Chicken* Chicken::create(const std::string& filename)
{
    Chicken* chicken = new Chicken();
    if (chicken) {
        chicken->animalName = filename;
        if (chicken->init()) {
            chicken->autorelease(); // 自动释放内存
            return chicken;
        }
    }
    CC_SAFE_DELETE(chicken); // 如果创建失败，安全删除
    return nullptr;
}

void Chicken::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (isNearSprite && keyCode == cocos2d::EventKeyboard::KeyCode::KEY_E && wheat_number > 0)
    {
        chicken_feed_label->setVisible(true); // 显示文字
        wheat_number--;
        if (happiness <= 50 && happiness >= 0)
            egg_number++;
        else if (happiness >= 50 && happiness <= 90)
            egg_number += 2;
        else
            egg_number += 4;
        // 启动定时器，3秒后隐藏文字
        this->scheduleOnce(CC_SCHEDULE_SELECTOR(Cow::hideLabel), 3.0f);
    }
    if (isNearSprite && keyCode == cocos2d::EventKeyboard::KeyCode::KEY_Q)
    {
        chicken_touch_label->setVisible(true); // 显示文字
        happiness += 10; // 扣除快乐值
        // 启动定时器，3秒后隐藏文字
        this->scheduleOnce(CC_SCHEDULE_SELECTOR(Cow::hideLabel), 3.0f);
    }
}

// 添加键盘事件监听
void Chicken::addKeyboardListener() {
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Chicken::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

//标签隐藏
void Chicken::hideLabel(float dt)
{
    chicken_feed_label->setVisible(false); // 隐藏文字
    chicken_touch_label->setVisible(false); // 隐藏文字
}

//初始化
bool Chicken::init()
{
    //创建纹理
    Texture2D* Move = Director::getInstance()->getTextureCache()->addImage("Animal/" + animalName + "/texture.png");

    if (!Sprite::initWithFile("Animal/" + animalName + "/static.png")) { return false; }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    speed = 50.0f;//设置速度
    currentPathIndex = 0;//初始路径
    ifSelected = false;//标记Animal未被选中

    // 设置位置更新函数
    schedule([=](float dt) {updatemove(dt); }, 0.0f, "animal_updatemove_key");

    Vector<SpriteFrame*> Animalup;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * Animalsize_x, Animalorder_up * Animalsize_y, Animalsize_x, Animalsize_y));
        Animalup.pushBack(frame);
    }
    Animation* uup = Animation::createWithSpriteFrames(Animalup, 0.3f);
    moveup = Animate::create(uup);
    moveup->setTag(1);

    Vector<SpriteFrame*>Animalleft;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * Animalsize_x, Animalorder_left * Animalsize_y, Animalsize_x, Animalsize_y));
        Animalleft.pushBack(frame);
    }
    Animation* lleft = Animation::createWithSpriteFrames(Animalleft, 0.3f);
    moveleft = Animate::create(lleft);
    moveleft->setTag(2);

    Vector<SpriteFrame*>Animalright;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * Animalsize_x, Animalorder_right * Animalsize_y, Animalsize_x, Animalsize_y));
        Animalright.pushBack(frame);
    }
    Animation* rright = Animation::createWithSpriteFrames(Animalright, 0.3f);
    moveright = Animate::create(rright);
    moveright->setTag(3);

    Vector<SpriteFrame*>Animaldown;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * Animalsize_x, Animalorder_down * Animalsize_y, Animalsize_x, Animalsize_y));
        Animaldown.pushBack(frame);
    }
    Animation* ddown = Animation::createWithSpriteFrames(Animaldown, 0.3f);
    movedown = Animate::create(ddown);
    movedown->setTag(4);

    // 创建文字标签
    chicken_feed_label = cocos2d::Label::createWithSystemFont("The chicken has been fed.Obtain Egg x1", "Arial", 10);
    chicken_feed_label->setVisible(false); // 初始时隐藏
    this->addChild(chicken_feed_label);
    chicken_feed_label->setPosition(cocos2d::Vec2(20, 40));

    // 创建文字标签
    chicken_touch_label = cocos2d::Label::createWithSystemFont("The chicken has been fed.Obtain Egg x1", "Arial", 10);
    chicken_touch_label->setVisible(false); // 初始时隐藏
    this->addChild(chicken_touch_label);
    chicken_touch_label->setPosition(cocos2d::Vec2(20, 40));

    Chicken::addKeyboardListener();
    this->schedule(CC_SCHEDULE_SELECTOR(Chicken::isMainCharNear), 0.1f);
    this->schedule(CC_SCHEDULE_SELECTOR(Sheep::decreaseHappiness), 20.0f);
    return true;
}

void Chicken::isMainCharNear(float delta)
{
    Vec2 sprite1Position = mainChar->getPosition(); // 获取主角的位置(相对屏幕)
    Vec2 sprite2Position = this->getPosition(); // 获取地图的位置(相对地图左下角)

    float distance = sprite1Position.distance(sprite2Position);

    // 如果距离小于某个值，设置标志位
    if (distance < 100)
    {
        isNearSprite = true;
    }
    else
    {
        isNearSprite = false;
    }
}

void Chicken::move(Chicken* chicken, TMXTiledMap* map) {
    map->addChild(chicken);
    chicken->setScale(1.5f);
    chicken->setPosition(Vec2(1070, 325));
    Sequence* move_cow = Sequence::create(chicken->moveup, chicken->moveright, chicken->moveleft, chicken->movedown, NULL);
    chicken->runAction(RepeatForever::create(move_cow));
    std::vector<Vec2> path_cow = { Vec2(1070, 325), Vec2(950, 325), Vec2(1070, 260),Vec2(950, 260) };
    chicken->setPath(path_cow);
}

void Chicken::decreaseHappiness(float delta) {
    if (happiness > 5)
        happiness -= 5;
}