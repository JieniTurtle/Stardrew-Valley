#include "BaseNPC.h"
#include "SimpleAudioEngine.h"
#include "../scenes/Dialog.h"

USING_NS_CC;

// 静态成员初始化
int BaseNPC::NPCsize_x = 20;
int BaseNPC::NPCsize_y = 40;
int BaseNPC::NPCorder_up = 2;
int BaseNPC::NPCorder_left = 3;
int BaseNPC::NPCorder_right = 1;
int BaseNPC::NPCorder_down = 0;

BaseNPC* BaseNPC::create(const std::string& filename)
{
    BaseNPC* npc = new BaseNPC();
    if (npc) {
        npc->NPCname = filename;
        if (npc->init()) {
            npc->autorelease();
            return npc;
        }
    }
    CC_SAFE_DELETE(npc);
    return nullptr;
}

bool BaseNPC::init()
{
    if (!(Sprite::initWithFile("MainCharacter/transparent.png"))) { 
        return false; 
    }

    // physical body
    auto physics_body = PhysicsBody::createCircle(8, PhysicsMaterial(0.00001f, 0.0f, 0.01f));
    physics_body->setRotationEnable(false);
    physics_body->setPositionOffset(Vec2(0, -16));
    this->addComponent(physics_body);

    // animated sprite
    this->animate_sprite = Sprite::create("NPC/" + NPCname + "/static.png");
    animate_sprite->setPosition(Vec2::ZERO);
    this->addChild(animate_sprite);

    // 创建对话
    Dialog_NPC = Dialog::create(NPCname);

    // 加载纹理
    Texture2D* Move = Director::getInstance()->getTextureCache()->addImage("NPC/" + NPCname + "/texture.png");

    speed = 50.0f;
    currentPathIndex = 0;

    // 创建向上动画
    Vector<SpriteFrame*> NPCup;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * NPCsize_x, NPCorder_up * NPCsize_y, NPCsize_x, NPCsize_y));
        NPCup.pushBack(frame);
    }
    Animation* uup = Animation::createWithSpriteFrames(NPCup, 0.3f);
    moveup = Animate::create(uup);
    moveup->setTag(1);

    // 创建向左动画
    Vector<SpriteFrame*> NPCleft;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * NPCsize_x, NPCorder_left * NPCsize_y, NPCsize_x, NPCsize_y));
        NPCleft.pushBack(frame);
    }
    Animation* lleft = Animation::createWithSpriteFrames(NPCleft, 0.3f);
    moveleft = Animate::create(lleft);
    moveleft->setTag(2);

    // 创建向右动画
    Vector<SpriteFrame*> NPCright;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * NPCsize_x, NPCorder_right * NPCsize_y, NPCsize_x, NPCsize_y));
        NPCright.pushBack(frame);
    }
    Animation* rright = Animation::createWithSpriteFrames(NPCright, 0.3f);
    moveright = Animate::create(rright);
    moveright->setTag(3);

    // 创建向下动画
    Vector<SpriteFrame*> NPCdown;
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(Move, Rect(i * NPCsize_x, NPCorder_down * NPCsize_y, NPCsize_x, NPCsize_y));
        NPCdown.pushBack(frame);
    }
    Animation* ddown = Animation::createWithSpriteFrames(NPCdown, 0.3f);
    movedown = Animate::create(ddown);
    movedown->setTag(4);

    // 创建静止动画
    Vector<SpriteFrame*> NPCstatic;
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

void BaseNPC::setPath(const std::vector<Vec2>& newPath)
{
    NPCpath = newPath;
    for (auto& i : NPCpath) {
        i *= 16;  // tile size
    }
    currentPathIndex = 0;
    setPosition(NPCpath[currentPathIndex]);
}

void BaseNPC::updatemove(float dt)
{
    if (NPCpath.empty()) return;

    Vec2 currentPosition = this->getPosition();
    Vec2 targetPosition = NPCpath[currentPathIndex];

    Vec2 direction = targetPosition - currentPosition;
    float distance = direction.length();

    if (distance < 30.0f) { 
        currentPathIndex = (currentPathIndex + 1) % NPCpath.size(); 
    }

    Vec2 moveDirection = direction.getNormalized();
    Vec2 v = moveDirection * speed;
    this->getPhysicsBody()->setVelocity(v);

    // 播放动画
    if (fabs(moveDirection.x) > fabs(moveDirection.y)) {
        if (moveDirection.x > 0) {
            playAnimation("right");
        } else {
            playAnimation("left");
        }
    } else {
        if (moveDirection.y > 0) {
            playAnimation("up");
        } else {
            playAnimation("down");
        }
    }
}

void BaseNPC::updatestatic(float dt)
{
    playAnimation("static");
}

void BaseNPC::playAnimation(const std::string& direction)
{
    animate_sprite->stopAllActions();
    if (direction == "up") {
        animate_sprite->runAction(RepeatForever::create(moveup));
    } else if (direction == "down") {
        animate_sprite->runAction(RepeatForever::create(movedown));
    } else if (direction == "left") {
        animate_sprite->runAction(RepeatForever::create(moveleft));
    } else if (direction == "right") {
        animate_sprite->runAction(RepeatForever::create(moveright));
    } else if (direction == "static") {
        animate_sprite->runAction(RepeatForever::create(movestatic));
    }
}

void BaseNPC::stopMovement()
{
    animate_sprite->stopAllActions();
    animate_sprite->runAction(RepeatForever::create(movestatic));
    getPhysicsBody()->setVelocity(Vec2::ZERO);
    unschedule("npc_notselected_key");
    schedule([=](float dt) { updatestatic(dt); }, 1.2f, "npc_isselected_key");
}

void BaseNPC::startMovement()
{
    animate_sprite->stopAllActions();
    animate_sprite->runAction(RepeatForever::create(movestatic));
    unschedule("npc_isselected_key");
    schedule([=](float dt) { updatemove(dt); }, 1.2f, "npc_notselected_key");
}

bool BaseNPC::JudgeClickNPC(Vec2 clickPos, int mapscale)
{
    if (!NPCmap) return false;
    
    float TileNum_Width = NPCmap->getMapSize().width;
    float TileNum_Height = NPCmap->getMapSize().height;
    float TileWidth = NPCmap->getTileSize().width * 1;
    float TileHeight = NPCmap->getTileSize().height * 1;
    float mapwidth = TileNum_Width * TileWidth;
    float mapheight = TileNum_Height * TileHeight;
    auto MAPposition = NPCmap->getPosition() / mapscale;

    clickPos.x = clickPos.x - MAPposition.x + mapwidth / 2;
    clickPos.y = clickPos.y - MAPposition.y + mapheight / 2;

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

void BaseNPC::interact()
{
    // 基础交互：显示对话
    showDialog();
}

void BaseNPC::showDialog()
{
    if (Dialog_NPC) {
        // 对话显示逻辑由Dialog类处理
        // 这里可以添加显示对话的代码
    }
}

