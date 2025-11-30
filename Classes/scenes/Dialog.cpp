#include "Dialog.h"
#include "DialogState.h"
#include "TaskObserver.h"
#include <algorithm>

// Refactored with State Pattern: 状态实现
void NormalState::onContinue(Dialog* dialog) {
    dialog->setState(std::make_unique<TaskGivingState>());
    dialog->notifyTaskAssigned();  // Notify observers
}

void NormalState::onEnd(Dialog* dialog) {
    dialog->removeFromParent();
}

std::string NormalState::getContent(const std::string& user) {
    std::ifstream file("NPC/" + user + "/DialogContent_Normal.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void TaskGivingState::onContinue(Dialog* dialog) {
    // No action in this state
}

void TaskGivingState::onEnd(Dialog* dialog) {
    dialog->setState(std::make_unique<TaskInProgressState>());
    dialog->notifyTaskProgressUpdated();  // Notify observers
    dialog->removeFromParent();
}

std::string TaskGivingState::getContent(const std::string& user) {
    std::ifstream file("NPC/" + user + "/DialogContent_TaskGiving.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void TaskInProgressState::onContinue(Dialog* dialog) {
    dialog->setState(std::make_unique<TaskCompletedState>());
    dialog->notifyTaskCompleted();  // Notify observers
    dialog->removeFromParent();
}

void TaskInProgressState::onEnd(Dialog* dialog) {
    dialog->removeFromParent();
}

std::string TaskInProgressState::getContent(const std::string& user) {
    std::ifstream file("NPC/" + user + "/DialogContent_TaskInProgress.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void TaskCompletedState::onContinue(Dialog* dialog) {
    // No action in this state
}

void TaskCompletedState::onEnd(Dialog* dialog) {
    dialog->removeFromParent();
}

std::string TaskCompletedState::getContent(const std::string& user) {
    std::ifstream file("NPC/" + user + "/DialogContent_TaskCompleted.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Refactored with Observer Pattern: 具体观察者实现
void QuestLog::onTaskAssigned(const std::string& user) {
    CCLOG("QuestLog: Task assigned by %s", user.c_str());
}

void QuestLog::onTaskProgressUpdated(const std::string& user) {
    CCLOG("QuestLog: Task progress updated for %s", user.c_str());
}

void QuestLog::onTaskCompleted(const std::string& user) {
    CCLOG("QuestLog: Task completed for %s", user.c_str());
}

void RewardSystem::onTaskAssigned(const std::string& user) {
    CCLOG("RewardSystem: Preparing rewards for %s's task", user.c_str());
}

void RewardSystem::onTaskProgressUpdated(const std::string& user) {
    CCLOG("RewardSystem: Tracking progress for %s's task", user.c_str());
}

void RewardSystem::onTaskCompleted(const std::string& user) {
    CCLOG("RewardSystem: Giving reward for %s's completed task", user.c_str());
}

void AchievementSystem::onTaskAssigned(const std::string& user) {
    CCLOG("AchievementSystem: New task from %s registered", user.c_str());
}

void AchievementSystem::onTaskProgressUpdated(const std::string& user) {
    CCLOG("AchievementSystem: Progress on %s's task noted", user.c_str());
}

void AchievementSystem::onTaskCompleted(const std::string& user) {
    CCLOG("AchievementSystem: Achievement unlocked for completing %s's task", user.c_str());
}

// Dialog类的主要实现
Dialog* Dialog::create(std::string& Username)
{
    Dialog* dialog = new Dialog();
    if (dialog) {
        dialog->User = Username;
        if (dialog && dialog->init()) {
            dialog->autorelease();
            return dialog;
        }
    }
    CC_SAFE_DELETE(dialog);
    return nullptr;
}

bool Dialog::init() {
    if (!Layer::create()) { return false; }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 创建背景
    BackLayer = LayerColor::create(Color4B(0, 0, 0, 180), visibleSize.width, visibleSize.height / 3);
    BackLayer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    BackLayer->setPosition(Vec2(0, 0));
    this->addChild(BackLayer);

    // 创建继续按钮
    button_continue = ui::Button::create("NPC/" + User + "/DialogContinue1.png", "NPC/" + User + "/DialogContinue2.png");
    button_continue->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    button_continue->setPosition(visibleSize / 8);
    button_continue->addClickEventListener(CC_CALLBACK_1(Dialog::onContinueButtonClick, this));
    this->addChild(button_continue);
    button_continue->setTitleText("continue");
    button_continue->setTitleFontSize(24);
    button_continue->setTitleColor(Color3B::WHITE);

    // 创建退出按钮
    button_close = ui::Button::create("NPC/" + User + "/DialogEnd1.png", "NPC/" + User + "/DialogEnd2.png");
    button_close->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    Vec2 button_close_position;
    button_close_position.x = visibleSize.width * 7 / 8;
    button_close_position.y = visibleSize.height / 8;
    button_close->setPosition(button_close_position);
    button_close->addClickEventListener(CC_CALLBACK_1(Dialog::onEndButtonClick, this));
    this->addChild(button_close);
    button_close->setTitleText("end");
    button_close->setTitleFontSize(24);
    button_close->setTitleColor(Color3B::WHITE);

    // 创建消息标签
    content = Label::createWithSystemFont("HW", "arial", 24);
    content->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    content->setPosition(visibleSize.width / 2, visibleSize.height / 4);
    content->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    content->setDimensions(visibleSize.width * 0.8, 0);
    this->addChild(content);

    // Refactored with State Pattern: 初始化状态
    setState(std::make_unique<NormalState>());
    
    // Refactored with Observer Pattern: 注册默认观察者 (在实际项目中可以从外部注入)
    addObserver(new QuestLog());
    addObserver(new RewardSystem());
    addObserver(new AchievementSystem());

    TaskStatus = 0;
    this->retain();
    return true;
}

// Refactored with State Pattern: 状态管理方法
void Dialog::setState(std::unique_ptr<DialogState> newState) {
    currentState = std::move(newState);
    updateContent();
    updateButtonVisibility();
}

void Dialog::updateContent() {
    content->setString(currentState->getContent(User));
}

void Dialog::updateButtonVisibility() {
    if (currentState->showContinueButton()) {
        this->addChild(button_continue);
    } else {
        this->removeChild(button_continue);
    }
}

// Refactored with Observer Pattern: 观察者管理方法
void Dialog::addObserver(TaskObserver* observer) {
    observers.push_back(observer);
}

void Dialog::removeObserver(TaskObserver* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Dialog::notifyTaskAssigned() {
    TaskStatus = 1;
    for (auto observer : observers) {
        observer->onTaskAssigned(User);
    }
}

void Dialog::notifyTaskProgressUpdated() {
    TaskStatus = 1;
    for (auto observer : observers) {
        observer->onTaskProgressUpdated(User);
    }
}

void Dialog::notifyTaskCompleted() {
    TaskStatus = 2;
    for (auto observer : observers) {
        observer->onTaskCompleted(User);
    }
}

// Refactored with State Pattern: 简化的按钮处理方法
void Dialog::onContinueButtonClick(Ref* obj) {
    button_continue->retain();
    currentState->onContinue(this);
}

void Dialog::onEndButtonClick(Ref* obj) {
    button_close->retain();
    currentState->onEnd(this);
}

std::string Dialog::GetDialogContent(std::string path)
{
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}