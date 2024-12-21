 #ifndef __DIALOG_H__
#define __DIALOG_H__

#include "cocos2d.h"
#include <fstream>
#include "ui/CocosGUI.h"

USING_NS_CC;

class Dialog : public Scene
{
public:
    //析构
    ~Dialog()
    {
        button_continue->release();
        button_close->release();
        content->release();
    }

    //标记是否点击了退出按钮
    int ChangeToClose;
    int TaskStatus;
    std::string User;//对话人名称

    LayerColor* BackLayer;//对话透明层
    Label* content;//对话文字
    ui::Button* button_continue;//继续对话按钮
    ui::Button* button_close;//退出对话按钮

    int contentIndex;
    std::vector<std::string> contentstore;//存储对话内容

    std::string GetDialogContent(std::string path);

    static Dialog* create(std::string& Username);

    bool init();//初始化函数

    void onContinueButtonClick(Ref* obj);//按下继续按钮
    void onEndButtonClick(Ref* obj);//按下结束按钮
};

#endif // __DIALOG_H__
