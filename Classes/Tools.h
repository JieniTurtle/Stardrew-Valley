#ifndef __TOOLS_H__
#define __TOOLS_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

// 工具类
class Tools: public cocos2d::Layer{
public:
    bool ishoe;//是否拿锄头

    ui::CheckBox* hoecheckbox;//锄头复选框
    virtual bool init();
    void sethoecheckbox();//设置锄头复选框
    CREATE_FUNC(Tools);
};


#endif
