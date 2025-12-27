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
            npc->autorelease(); // �Զ��ͷ��ڴ�
            return npc;
        }
    }
    CC_SAFE_DELETE(npc); // �������ʧ�ܣ���ȫɾ��
    return nullptr;
}

//��ʼ��
bool NPC::init()
{
    if (!(Sprite::initWithFile("MainCharacter/transparent.png"))) { return false; }

    // physical body
    auto physics_body = PhysicsBody::createCircle(8, PhysicsMaterial(0.00001f, 0.0f, 0.01f));
    physics_body->setRotationEnable(false);
    //physics_body->setContactTestBitmask(0xFFFFFFFF);
    physics_body->setPositionOffset(Vec2(0, -16));
    this->addComponent(physics_body);

    // animated sprite
    this->animate_sprite = Sprite::create("NPC/" + NPCname + "/static.png");
    animate_sprite->setPosition(Vec2::ZERO);
    this->addChild(animate_sprite);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //�����Ի�
    Dialog_NPC = Dialog::create(NPCname);

    //��������
    Texture2D* Move = Director::getInstance()->getTextureCache()->addImage("NPC/" + NPCname + "/texture.png");

    speed = 50.0f;//�����ٶ�
    currentPathIndex = 0;//��ʼ·��

    //���϶���
    Vector<SpriteFrame*> NPCup;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * NPCsize_x, NPCorder_up * NPCsize_y, NPCsize_x, NPCsize_y));
        NPCup.pushBack(frame);
    }
    Animation* uup = Animation::createWithSpriteFrames(NPCup, 0.3f);
    moveup = Animate::create(uup);
    moveup->setTag(1);

    //���󶯻�
    Vector<SpriteFrame*>NPCleft;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * NPCsize_x, NPCorder_left * NPCsize_y, NPCsize_x, NPCsize_y));
        NPCleft.pushBack(frame);
    }
    Animation* lleft = Animation::createWithSpriteFrames(NPCleft, 0.3f);
    moveleft = Animate::create(lleft);
    moveleft->setTag(2);

    //���Ҷ���
    Vector<SpriteFrame*>NPCright;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * NPCsize_x, NPCorder_right * NPCsize_y, NPCsize_x, NPCsize_y));
        NPCright.pushBack(frame);
    }
    Animation* rright = Animation::createWithSpriteFrames(NPCright, 0.3f);
    moveright = Animate::create(rright);
    moveright->setTag(3);

    //���¶���
    Vector<SpriteFrame*>NPCdown;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * NPCsize_x, NPCorder_down * NPCsize_y, NPCsize_x, NPCsize_y));
        NPCdown.pushBack(frame);
    }
    Animation* ddown = Animation::createWithSpriteFrames(NPCdown, 0.3f);
    movedown = Animate::create(ddown);
    movedown->setTag(4);

    //��ֹ����
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

    relation = 0;

    return true;
}

//·����ʼ��
void NPC::setPath(const std::vector<Vec2>& newPath)
{
    NPCpath = newPath;
    //for (int i = 0; i < NPCpath.size(); i++) {
    //    NPCpath[i] *= 16;   // tile size
    //}
    
    for (auto& i : NPCpath) {
        i *= 16;
    }
    currentPathIndex = 0;  // ����·������
    setPosition(NPCpath[currentPathIndex]);  // ���� NPC ����ʼλ��
}

//NPC�ƶ��ĸ���
void NPC::updatemove(float dt)
{
    if (NPCpath.empty()) return;

    // ��ȡĿ��λ��
    Vec2 currentPosition = this->getPosition();
    Vec2 targetPosition = NPCpath[currentPathIndex];

    // ���㵱ǰλ�ú�Ŀ��λ�õľ���
    Vec2 direction = targetPosition - currentPosition;
    float distance = direction.length();

    //�������Ŀ�꣬�л�����һ��·����
    if (distance < 30.0f) { currentPathIndex = (currentPathIndex + 1) % NPCpath.size(); }

    //�����ƶ�
    Vec2 moveDirection = direction.getNormalized();
    Vec2 v = moveDirection * speed;
    this->getPhysicsBody()->setVelocity(v);

    //���Ŷ���
    if (fabs(moveDirection.x) > fabs(moveDirection.y)) { // ˮƽ�ƶ�
        if (moveDirection.x > 0) {
            playAnimation("right");
        }
        else {
            playAnimation("left");
        }
    }
    else { // ��ֱ�ƶ�
        if (moveDirection.y > 0) {
            playAnimation("up");
        }
        else {
            playAnimation("down");
        }
    }
}

//NPC��ֹ�ĸ���
void NPC::updatestatic(float dt)
{
    playAnimation("static");
}

//���Ŷ���
void NPC::playAnimation(const std::string& direction)
{
    animate_sprite->stopAllActions();
    if (direction == "up") {
        animate_sprite->runAction(RepeatForever::create(moveup));
    }
    else if (direction == "down") {
        animate_sprite->runAction(RepeatForever::create(movedown));
    }
    else if (direction == "left") {
        animate_sprite->runAction(RepeatForever::create(moveleft));
    }
    else if (direction == "right") {
        animate_sprite->runAction(RepeatForever::create(moveright));
    }
    else if (direction == "static") {
        animate_sprite->runAction(RepeatForever::create(movestatic));
    }
}

// ֹͣNPC�Ķ������ƶ�
void NPC::stopMovement()
{
    animate_sprite->stopAllActions();
    animate_sprite->runAction(RepeatForever::create(movestatic));
    getPhysicsBody()->setVelocity(Vec2::ZERO);
    // ֹͣ NPC ��·������
    unschedule("npc_notselected_key");

    schedule([=](float dt) { updatestatic(dt); }, 1.2f, "npc_isselected_key");
}

// �ָ�NPC�Ķ������ƶ�
void NPC::startMovement()
{
    animate_sprite->stopAllActions();
    //Sequence* move_NPC = Sequence::create(moveup, moveright, moveleft, movedown, NULL);
    animate_sprite->runAction(RepeatForever::create(movestatic));

    unschedule("npc_isselected_key");

    // ���� NPC ��·������
    schedule([=](float dt) { updatemove(dt); }, 1.2f, "npc_notselected_key");
}


//�ж�����Ƿ�����NPC��
bool NPC::JudgeClickNPC(Vec2 clickPos,int mapscale)
{
    //��ȡ��ͼ������Ϣ
    float TileNum_Width = NPCmap->getMapSize().width;  // �����ש����
    float TileNum_Height = NPCmap->getMapSize().height; // �����ש����
    float TileWidth = NPCmap->getTileSize().width * 1; // ������ש�����ؿ���
    float TileHeight = NPCmap->getTileSize().height * 1; // ������ש�����ظ߶�
    float mapwidth = TileNum_Width * TileWidth;//��ȡ��ͼ����
    float mapheight = TileNum_Height * TileHeight;//��ȡ��ͼ�߶�
    //��ȡ��ͼλ��
    auto MAPposition = NPCmap->getPosition() / mapscale;

    //�����λ��ת��Ϊ�Ե�ͼ���½�Ϊԭ��
    clickPos.x = clickPos.x - MAPposition.x + mapwidth / 2;
    clickPos.y = clickPos.y - MAPposition.y + mapheight / 2;

    //��ȡNPCλ��
    Vec2 NPCPosition = this->getPosition();

    if (NPCPosition.x - NPCsize_x / 2 <= clickPos.x) {
        if (NPCPosition.x + NPCsize_x / 2 >= clickPos.x) {
            if (NPCPosition.y - NPCsize_y / 2 <= clickPos.y) {
                if (NPCPosition.y + NPCsize_y / 2 >= clickPos.y) {
                    return true;
                }
            }
        }
    }
    return false;
}

// IPoolable 接口实现
void NPC::reset()
{
    // 停止所有动作
    stopAllActions();
    if (animate_sprite)
    {
        animate_sprite->stopAllActions();
    }
    
    // 取消所有调度器
    unscheduleAllCallbacks();
    
    // 从父节点移除
    removeFromParent();
    
    // 重置路径相关状态
    NPCpath.clear();
    currentPathIndex = 0;
    speed = 0.0f;
    
    // 重置位置和变换
    setPosition(Vec2::ZERO);
    setScale(1.0f);
    setRotation(0.0f);
    setVisible(true);
    setOpacity(255);
    
    // 重置关系值
    relation = 0;
    
    // 清理指针
    NPCmap = nullptr;
    Dialog_NPC = nullptr;
    
    // 清理物理体
    removeComponent(getPhysicsBody());
}