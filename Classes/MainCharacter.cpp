
#include "MainCharacter.h"
#include "SimpleAudioEngine.h"
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
    this->schedule(CC_SCHEDULE_SELECTOR(MainCharacter::update), 0.15f); //时刻刷新事件

    return true;
}
void MainCharacter::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_W: // W
            movementkeys[0] = true; // 向上移动
            break;
        case EventKeyboard::KeyCode::KEY_A: // A
            movementkeys[1] = true; // 向左移动
            break;
        case EventKeyboard::KeyCode::KEY_S: // S
            movementkeys[2] = true; // 向下移动
            break;
        case EventKeyboard::KeyCode::KEY_D: // D
            movementkeys[3] = true; // 向右移动
            break;
        default:
            break;
    }

}
void MainCharacter::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_W:
            movementkeys[0] = false;
            break;
        case EventKeyboard::KeyCode::KEY_A:
            movementkeys[1] = false;
            break;
        case EventKeyboard::KeyCode::KEY_S:
            movementkeys[2] = false;
            break;
        case EventKeyboard::KeyCode::KEY_D:
            movementkeys[3] = false;
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
      // 持续移动逻辑
    if (movementkeys[0]) { // W
        position.y -= 10; // 向上移动
    }
    if (movementkeys[1]) { // A
        position.x += 10; // 向左移动
    }
    if (movementkeys[2]) { // S
        position.y += 10; // 向下移动
    }
    if (movementkeys[3]) { // D
        position.x -= 10; // 向右移动
    }
    //检查是否超出边界
    if (position.x > mapwidth / 2) {
        position.x = mapwidth / 2;
    }
    if (position.y > mapheight / 2) {
        position.y = mapheight / 2;
    }
    if (position.x < visibleSize.width - mapwidth / 2) {
        position.x = visibleSize.width - mapwidth / 2;
    }
    if (position.y < visibleSize.height - mapheight / 2) {
        position.y = visibleSize.height - mapheight / 2;
    }

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
  
    // 播放走路动画
    if (movementkeys[2]
        ) {
        walkAnimationnext = walkAnimationdown->clone();
        staticnext = "characterdown2.png";
    }
    if (movementkeys[3]) {
        walkAnimationnext = walkAnimationright->clone();
        staticnext = "characterright2.png";
    }
    if (movementkeys[1]) {
        walkAnimationnext = walkAnimationleft->clone();
        staticnext = "characterleft2.png";
    }
    if (movementkeys[0]) {
        walkAnimationnext = walkAnimationup->clone();
        staticnext = "characterup2.png";
    }

    // 创建 Animate 动作
    if (movementkeys[0] || movementkeys[1] || movementkeys[2] || movementkeys[3]) {
        auto animateActionnext = Animate::create(walkAnimationnext);
        this->runAction(RepeatForever::create(animateActionnext));
        mainmap->runAction(MoveTo::create(0.3, position));
    }
    else {
        this->stopAllActions(); // 停止所有动作

        this->setTexture(staticnext); // 切换为静态图像
    }


}

