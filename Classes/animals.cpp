#include "animals.h"
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

    return true;
}

void Cow::move(Cow* cow, TMXTiledMap* map) {
    map->addChild(cow);
    cow->setScale(0.5f);
    cow->setPosition(Vec2(950, 765 ));
    Sequence* move_cow = Sequence::create(cow->moveup, cow->moveright, cow->moveleft, cow->movedown, NULL);
    cow->runAction(RepeatForever::create(move_cow));
    std::vector<Vec2> path_cow = { Vec2(950, 765), Vec2(950, 720), Vec2(1070, 720),Vec2(1070, 765) };
    cow->setPath(path_cow);
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

    return true;
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

    return true;
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

