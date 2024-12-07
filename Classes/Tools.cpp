#include "Tools.h"
#include "SimpleAudioEngine.h"

 bool Tools:: init() {
     sethoecheckbox();//设置锄头复选框
     ishoe = 0;
     return true;
}

 void  Tools::sethoecheckbox() {
     auto visibleSize = Director::getInstance()->getVisibleSize();//获取当前游戏视图窗口的尺寸

     hoecheckbox = ui::CheckBox::create("chutou.png", "chutou2.png");
     hoecheckbox->setPosition(Vec2(visibleSize.width / 8, visibleSize.height / 4)); // 设置位置
   
     // 添加事件监听器
     hoecheckbox->addEventListener([this](Ref* sender, ui::CheckBox::EventType type) {
         switch (type) {
             case ui::CheckBox::EventType::SELECTED:
                 ishoe = 1;
                 break;
             case ui::CheckBox::EventType::UNSELECTED:
                 ishoe = 0;
                 break;
             default:
                 break;
         }
         });
     this->addChild(hoecheckbox);
 }