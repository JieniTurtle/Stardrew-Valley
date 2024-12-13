#include <NPC.h>
#include <SimpleAudioEngine.h>

USING_NS_CC;

int NPC::NPCsize_x = 20;
int NPC::NPCsize_y = 40;
int NPC::NPCorder_up = 2;
int NPC::NPCorder_left = 3;
int NPC::NPCorder_right = 1;
int NPC::NPCorder_down = 0;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

NPC* NPC::create(const std::string& filename)
{
    NPC* npc = new NPC();
    if (npc) {
        npc->NPCname = filename;
        if (npc->init()) {
            npc->autorelease(); // 自动释放内存
            return npc;
        }
    }
    CC_SAFE_DELETE(npc); // 如果创建失败，安全删除
    return nullptr;
}

//初始化
bool NPC::init()
{
    if (!Sprite::initWithFile("NPC/" + NPCname + "/static.png")) { return false; }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //创建纹理
    Texture2D* Move = Director::getInstance()->getTextureCache()->addImage("NPC/" + NPCname + "/texture.png");

    speed = 50.0f;//设置速度
    currentPathIndex = 0;//初始路径

    //向上动画
    Vector<SpriteFrame*> NPCup;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * NPCsize_x, NPCorder_up * NPCsize_y, NPCsize_x, NPCsize_y));
        NPCup.pushBack(frame);
    }
    Animation* uup = Animation::createWithSpriteFrames(NPCup, 0.3f);
    moveup = Animate::create(uup);
    moveup->setTag(1);

    //向左动画
    Vector<SpriteFrame*>NPCleft;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * NPCsize_x, NPCorder_left * NPCsize_y, NPCsize_x, NPCsize_y));
        NPCleft.pushBack(frame);
    }
    Animation* lleft = Animation::createWithSpriteFrames(NPCleft, 0.3f);
    moveleft = Animate::create(lleft);
    moveleft->setTag(2);

    //向右动画
    Vector<SpriteFrame*>NPCright;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * NPCsize_x, NPCorder_right * NPCsize_y, NPCsize_x, NPCsize_y));
        NPCright.pushBack(frame);
    }
    Animation* rright = Animation::createWithSpriteFrames(NPCright, 0.3f);
    moveright = Animate::create(rright);
    moveright->setTag(3);

    //向下动画
    Vector<SpriteFrame*>NPCdown;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * NPCsize_x, NPCorder_down * NPCsize_y, NPCsize_x, NPCsize_y));
        NPCdown.pushBack(frame);
    }
    Animation* ddown = Animation::createWithSpriteFrames(NPCdown, 0.3f);
    movedown = Animate::create(ddown);
    movedown->setTag(4);

    Vector<SpriteFrame*>NPCstatic;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(0, 0, NPCsize_x, NPCsize_y));
        NPCstatic.pushBack(frame);
    }
    Animation* sstatic = Animation::createWithSpriteFrames(NPCstatic, 0.3f);
    movestatic = Animate::create(sstatic);
    movestatic->setTag(5);

    moveup->retain();
    moveleft->retain();
    moveright->retain();
    movedown->retain();
    movestatic->retain();

    return true;
}

//路径初始化
void NPC::setPath(const std::vector<Vec2>& newPath)
{
    NPCpath = newPath;
    currentPathIndex = 0;  // 重置路径索引
    setPosition(NPCpath[currentPathIndex]);  // 设置 NPC 的起始位置
}

//NPC移动的更新
void NPC::updatemove(float dt)
{
    if (NPCpath.empty()) return;

    // 获取目标位置
    Vec2 currentPosition = this->getPosition();
    Vec2 targetPosition = NPCpath[currentPathIndex];

    // 计算当前位置和目标位置的距离
    Vec2 direction = targetPosition - currentPosition;
    float distance = direction.length();

    //如果到达目标，切换到下一个路径点
    if (distance < 1.0f) { currentPathIndex = (currentPathIndex + 1) % NPCpath.size(); }

    //进行移动
    Vec2 moveDirection = direction.getNormalized();
    setPosition(currentPosition + moveDirection * speed * dt);
    //播放动画
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

//NPC静止的更新
void NPC::updatestatic(float dt)
{
    playAnimation("static");
}

//播放动画
void NPC::playAnimation(const std::string& direction)
{
    if (direction == "up") {

        runAction(RepeatForever::create(moveup));
    }
    else if (direction == "down") {
        runAction(RepeatForever::create(movedown));
    }
    else if (direction == "left") {
        runAction(RepeatForever::create(moveleft));
    }
    else if (direction == "right") {
        runAction(RepeatForever::create(moveright));
    }
    else if (direction == "static") {
        runAction(RepeatForever::create(movestatic));
    }
}

// 停止NPC的动画和移动
void NPC::stopMovement()
{
    stopAllActions();
    runAction(RepeatForever::create(movestatic));
    // 停止 NPC 的路径更新
    unschedule("npc_notselected_key");

    schedule([=](float dt) { updatestatic(dt); }, 0.0f, "npc_isselected_key");
}

// 恢复NPC的动画和移动
void NPC::startMovement()
{
    stopAllActions();
    //Sequence* move_NPC = Sequence::create(moveup, moveright, moveleft, movedown, NULL);
    runAction(RepeatForever::create(movestatic));

    unschedule("npc_isselected_key");

    // 设置 NPC 的路径更新
    schedule([=](float dt) { updatemove(dt); }, 0.0f, "npc_notselected_key");
}

//判断鼠标是否点击在NPC上
bool NPC::JudgeClickNPC(Vec2 NPCp, Vec2 clickp)
{
    if (NPCp.x - NPCsize_x / 2 <= clickp.x) {
        if (NPCp.x + NPCsize_x / 2 >= clickp.x) {
            if (NPCp.y - NPCsize_y / 2 <= clickp.y) {
                if (NPCp.y + NPCsize_y / 2 >= clickp.y) {
                    return true;
                }
            }
        }
    }
    return false;
}

//处理鼠标事件
bool NPC::onMouseDown(cocos2d::Event* event,int mapscale)
{
    //未被选中时
    if (ifSelected == false) {
        //地图放缩尺寸
        //获取地图坐标信息
        float TileNum_Width = NPCmap->getMapSize().width;  // 横向瓷砖数量
        float TileNum_Height = NPCmap->getMapSize().height; // 纵向瓷砖数量
        float TileWidth = NPCmap->getTileSize().width * 1; // 单个瓷砖的像素宽度
        float TileHeight = NPCmap->getTileSize().height * 1; // 单个瓷砖的像素高度
        float mapwidth = TileNum_Width * TileWidth;//获取地图宽度
        float mapheight = TileNum_Height * TileHeight;//获取地图高度
        //获取地图位置
        auto MAPposition = NPCmap->getPosition() / mapscale;

        //获取当前游戏视图窗口的尺寸
        auto visibleSize = Director::getInstance()->getVisibleSize();

        //获取鼠标点击的位置（初始原点为左上角）
        EventMouse* e = static_cast<EventMouse*>(event);
        Vec2 clickPos = e->getLocation() / mapscale;
        //将鼠标点击的位置转化为以屏幕左下角为原点
        clickPos.y = visibleSize.height / mapscale - clickPos.y;
        //将鼠标位置转化为以地图左下角为原点
        clickPos.x = clickPos.x - MAPposition.x + mapwidth / 2;
        clickPos.y = clickPos.y - MAPposition.y + mapheight / 2;
        //获取NPC位置
        Vec2 NPCPosition = this->getPosition();

        // 检查点击是否在NPC范围内
        if (JudgeClickNPC(NPCPosition, clickPos)) {
            ifSelected = true;
            stopMovement();
            return true;
        }
    }
    //被选中时
    else {
        ifSelected = false;
        startMovement();
        return true;
    }
    return false;
}