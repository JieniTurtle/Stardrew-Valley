#include "explore.h"
#include "MiniGame.h"
Interaction* Interaction::create(TMXTiledMap* map)
{
    Interaction* interaction = new Interaction;
    if (interaction) {
        interaction->map_ = map;
        if (interaction->init()) {
            interaction->autorelease(); // �Զ��ͷ��ڴ�
            return interaction;
        }
    }
    CC_SAFE_DELETE(interaction); // �������ʧ�ܣ���ȫɾ��
    return nullptr;
}

//��ʼ��
bool Interaction::init()
{
	this->initWithFile("maps/" + name_ + ".png");
    this->setPosition(position_);

	auto physics_body = PhysicsBody::createBox(this->getContentSize());
	physics_body->setDynamic(false);
    this->addComponent(physics_body);

	auto mouseListener = EventListenerMouse::create();
	mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(Interaction::on_mouse_down, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    return true;
}

Vec2 Interaction::transfer_vision_to_map(Vec2 visible_position)
{
    //��ȡ��ͼ������Ϣ
    float TileNum_Width = map_->getMapSize().width;  // �����ש����
    float TileNum_Height = map_->getMapSize().height; // �����ש����
    float TileWidth = map_->getTileSize().width * 1; // ������ש�����ؿ���
    float TileHeight = map_->getTileSize().height * 1; // ������ש�����ظ߶�
    float mapwidth = TileNum_Width * TileWidth;//��ȡ��ͼ����
    float mapheight = TileNum_Height * TileHeight;//��ȡ��ͼ�߶�
    auto mapscale = map_->getScale();
    //��ȡ��ͼλ��
    auto visibleSize = Director::getInstance()->getVisibleSize();//��ȡ��ǰ��Ϸ��ͼ���ڵĳߴ�
    auto MAPposition = map_->getPosition();

    Vec2 position_on_map;

    //�����λ��ת��Ϊ�Ե�ͼ���½�Ϊԭ��
    position_on_map.x = (visible_position.x - MAPposition.x) / mapscale + mapwidth / 2;
    position_on_map.y = (visibleSize.height - visible_position.y - MAPposition.y) / mapscale + mapheight / 2;

    return position_on_map;
}

Bridge* Bridge::create(TMXTiledMap* map)
{
    Bridge* bridge = new Bridge;
    if (bridge) {
        bridge->map_ = map;
        if (bridge->init()) {
            bridge->autorelease(); // �Զ��ͷ��ڴ�
            return bridge;
        }
    }
    CC_SAFE_DELETE(bridge); // �������ʧ�ܣ���ȫɾ��
    return nullptr;
}

bool Bridge::init()
{
    done_flag_ = &bridge_repaired;
    name_ = "broken_bridge";
    position_ = Vec2(1520, 228);
    Interaction::init();
    return true;
}

void Bridge::on_mouse_down(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    Vec2 click_position = e->getLocation();
    auto distance = transfer_vision_to_map(click_position).distance(this->getPosition());
    if (distance < this->getContentSize().width && !*done_flag_) {
        *done_flag_ = 1;
        auto task = task_manager->get_task_by_name("repair_bridge");
        task_manager->remove_task(task);
        this->removeFromParent();
    }
}

Chest* Chest::create(TMXTiledMap* map, int ID)
{
    Chest* chest = new Chest;
    if (chest) {
        chest->map_ = map;
        chest->ID_ = ID;
        if (chest->init()) {
            chest->autorelease(); // �Զ��ͷ��ڴ�
            return chest;
        }
    }
    CC_SAFE_DELETE(chest); // �������ʧ�ܣ���ȫɾ��
    return nullptr;
}

bool Chest::init()
{
    done_flag_ = chest_opened + ID_;
    name_ = "unopened_chest";
    if (ID_ == 0) {
        position_ = Vec2(1820, 288);
    }
    else if (ID_ == 1) {
        position_ = Vec2(120, 288);
    }
    Interaction::init();
    return true;
}



void Chest::on_mouse_down(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    Vec2 click_position = e->getLocation();
    auto distance = transfer_vision_to_map(click_position).distance(this->getPosition());
    if (distance < this->getContentSize().width && !*done_flag_) {
        *done_flag_ = 1;
        Vector<SpriteFrame*> chest_animate_frames;
        for (int i = 1; i < 4; i++) {
            auto frame = SpriteFrame::create("maps/chest.png", Rect(i * 32, 0, 32, 32));
            chest_animate_frames.pushBack(frame);
        }
        Animation* chest_animate = Animation::createWithSpriteFrames(chest_animate_frames, 0.1f);
        auto animate = Animate::create(chest_animate);
        this->runAction(animate);
        mineral_number++;

    }
}
GobangBoard* GobangBoard::create(TMXTiledMap* map)
{
    GobangBoard* gobang_board = new GobangBoard;
    if (gobang_board) {
        gobang_board->map_ = map;
        gobang_board->done_flag_ = 0; // always can click
        if (gobang_board->init()) {
            gobang_board->autorelease(); // �Զ��ͷ��ڴ�
            return gobang_board;
        }
    }
    CC_SAFE_DELETE(gobang_board); // �������ʧ�ܣ���ȫɾ��
    return nullptr;
}

bool GobangBoard::init()
{
    name_ = "Gobang_board";
    position_ = Vec2(240, 928);
    Interaction::init();
    auto player = Sprite::create("NPC/Willy/static.png");
    this->addChild(player);
    player->setPosition(Vec2(8, 40));

    auto physics_body = PhysicsBody::createBox(Size(16, 16));
    physics_body->setPositionOffset(Vec2(0, -8));
    physics_body->setDynamic(false);
    player->addComponent(physics_body);

    return true;
}

void GobangBoard::on_mouse_down(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    Vec2 click_position = e->getLocation();
    auto distance = transfer_vision_to_map(click_position).distance(this->getPosition());
    if (distance < this->getContentSize().width && !is_playing_gobang) {
        auto gobang_scene = GobangScene::create();
        Director::getInstance()->pushScene(gobang_scene);
        is_playing_gobang = 1;
    }
}

// IPoolable 接口实现
void Interaction::reset()
{
    // 停止所有动作
    stopAllActions();
    
    // 取消所有调度器
    unscheduleAllCallbacks();
    
    // 从父节点移除
    removeFromParent();
    
    // 重置位置和变换
    setPosition(Vec2::ZERO);
    setScale(1.0f);
    setRotation(0.0f);
    setVisible(true);
    setOpacity(255);
    
    // 清理指针
    map_ = nullptr;
    done_flag_ = nullptr;
    
    // 清理物理体
    removeComponent(getPhysicsBody());
    
    // 清理事件监听器
    _eventDispatcher->removeEventListenersForTarget(this);
}