#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "cocos2d.h"
#include <fstream>
#include <vector>
#include <memory>
#include "ui/CocosGUI.h"
#include "DialogState.h"
#include "TaskObserver.h"

USING_NS_CC;

class Dialog : public Scene
{
public:
    // Refactored with State Pattern and Observer Pattern
    ~Dialog()
    {
        button_continue->release();
        button_close->release();
        content->release();
    }

    // 状态相关变量
    std::string User;
    int TaskStatus;

    // UI组件
    LayerColor* BackLayer;
    Label* content;
    ui::Button* button_continue;
    ui::Button* button_close;

    // Refactored with State Pattern: 状态管理
private:
    std::unique_ptr<DialogState> currentState;
    
    // Refactored with Observer Pattern: 观察者列表
    std::vector<TaskObserver*> observers;

public:
    // 状态管理方法
    void setState(std::unique_ptr<DialogState> newState);
    void updateContent();
    void updateButtonVisibility();

    // 观察者管理方法
    void addObserver(TaskObserver* observer);
    void removeObserver(TaskObserver* observer);
    void notifyTaskAssigned();
    void notifyTaskProgressUpdated();
    void notifyTaskCompleted();

    // 工具方法
    std::string GetDialogContent(std::string path);
    static Dialog* create(std::string& Username);
    bool init();

    // 按钮事件处理方法 (已简化)
    void onContinueButtonClick(Ref* obj);
    void onEndButtonClick(Ref* obj);
};

#endif // __DIALOG_H__