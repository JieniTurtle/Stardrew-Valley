
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
    return true;
}
void MainCharacter::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    // 获取当前角色位置
    Vec2 position =this->getPosition();

    // 获取角色的尺寸
    Size characterSize = this->getContentSize();
    auto visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸
    // 根据按下的键移动角色
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_W: // 向上
            position.y += 10; // 10 是移动距离
            break;
        case EventKeyboard::KeyCode::KEY_A: // 向左
            position.x -= 10;
            break;
        case EventKeyboard::KeyCode::KEY_S: // 向下
            position.y -= 10;
            break;
        case EventKeyboard::KeyCode::KEY_D: // 向右
            position.x += 10;
            break;
        default:
            return;
    }
    //检查是否超出边界
    if (position.x < characterSize.width / 2) {
        position.x = characterSize.width / 2;
    }
    if (position.y < characterSize.height / 2) {
        position.y = characterSize.height / 2;
    }
    if (position.x > visibleSize.width - characterSize.width / 2) {
        position.x = visibleSize.width - characterSize.width / 2;
    }
    if (position.y > visibleSize.height - characterSize.height / 2) {
        position.y = visibleSize.height - characterSize.height / 2;
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
    std::string staticnext;//静止方向图片
    // 播放走路动画
    if (             keyCode == EventKeyboard::KeyCode::KEY_S 
       ) {
        walkAnimationnext = walkAnimationdown->clone();
        staticnext = "characterdown2.png";
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_D) {
         walkAnimationnext = walkAnimationright->clone();
         staticnext = "characterright2.png";
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_A) {
        walkAnimationnext = walkAnimationleft->clone();
        staticnext = "characterleft2.png";
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_W) {
        walkAnimationnext = walkAnimationup->clone();
        staticnext = "characterup2.png";
    }
    // 创建 Animate 动作
    auto animateActionnext = Animate::create(walkAnimationnext);
    this->runAction(RepeatForever::create(animateActionnext));
    //角色移动，播放动画
    this->runAction(Sequence::create(MoveTo::create(0.5, position), CallFunc::create([this, staticnext]() {

        this->stopAllActions(); // 停止所有动作
        
        this->setTexture(staticnext); // 切换为静态图像

        }), nullptr));

}

void MainCharacter::addKeyboardListener() {
    // 添加键盘事件监听
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(MainCharacter::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
