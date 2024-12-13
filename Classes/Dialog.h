#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "cocos2d.h"
#include <fstream>
//#include "ui/CocosGUI.h"

USING_NS_CC;

class Dialog : public Layer
{
public:
    std::string User;//对话人名称
    LayerColor* Dialog_Layer;//对话透明层
    Label* Dialog_Label;//对话文字
    MenuItemImage* Dialog_CloseBotton;
    Menu* Dialog_Menu;//对话菜单

    std::string GetDialogContent(std::string path);

    static Dialog* create(const std::string& Username);

    bool init();//初始化函数

    bool Dialog::JudgeClickButton(Vec2 clickp, int mapscale);

    //退出按钮回调函数
    void onExitButtonClick(Ref* obj);
};

#endif // __DIALOG_H__
