#include "Dialog.h"

Dialog* Dialog::create(const std::string& Username)
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

    auto visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸

    // 创建一个黑色的半透明背景，只占据屏幕的下半部分
    Dialog_Layer = LayerColor::create(Color4B(0, 0, 0, 180), visibleSize.width, visibleSize.height / 3);
    Dialog_Layer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    Dialog_Layer->setPosition(Vec2(0, 0));  // 背景放在屏幕的下半部分
    this->addChild(Dialog_Layer);

    // 创建消息标签
    Dialog_Label = Label::createWithSystemFont("HW", "arial", 24);
    Dialog_Label->setString(GetDialogContent("NPC/" + User + "/DialogContent.txt"));
    Dialog_Label->setDimensions(visibleSize.width * 0.8, 0);
    Dialog_Label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    Dialog_Label->setPosition(visibleSize.width * 0.1, visibleSize.height / 4);
    //设置文本水平方向在左边，垂直方向在右边显示
    Dialog_Label->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    this->addChild(Dialog_Label);

    //创建退出按钮
    Dialog_CloseBotton = MenuItemImage::create(
        "NPC/" + User + "/DialogStart.png", "NPC/" + User + "/DialogEnd.png", CC_CALLBACK_1(Dialog::onExitButtonClick, this));
    Dialog_CloseBotton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    Dialog_CloseBotton->setPosition(Dialog_CloseBotton->getPosition().x + visibleSize.width / 2, Dialog_CloseBotton->getPosition().y - visibleSize.height / 2);
    Dialog_Menu = Menu::create(Dialog_CloseBotton, NULL);
    this->addChild(Dialog_Menu);

    return true;
}

std::string Dialog::GetDialogContent(std::string path)
{
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf(); // 将文件内容读取到流中
    return buffer.str();    // 返回文件内容
}

bool Dialog::JudgeClickButton(Vec2 clickp,int mapscale)
{
    Vec2 ButtonSize = Dialog_CloseBotton->getContentSize();
    auto visibleSize = Director::getInstance()->getVisibleSize() / mapscale;

    if (clickp.x >= visibleSize.width - ButtonSize.x) {
        if (clickp.x <= visibleSize.width) {
            if (clickp.y >= 0) {
                if (clickp.y < ButtonSize.y) {
                    return true;
                }
            }
        }
    }
    return false;
}

void Dialog::onExitButtonClick(Ref* obj)
{
    this->removeFromParent();
}