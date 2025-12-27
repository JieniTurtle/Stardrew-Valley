// Refactored with State Pattern & Observer Pattern
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

    // ״̬��ر���
    std::string User;
    int TaskStatus;

    // UI���
    LayerColor* BackLayer;
    Label* content;
    ui::Button* button_continue;
    ui::Button* button_close;

    // Refactored with State Pattern: ״̬����
private:
    std::unique_ptr<DialogState> currentState;
    
    // Refactored with Observer Pattern: �۲����б�
    std::vector<TaskObserver*> observers;

public:
    // ״̬��������
    void setState(std::unique_ptr<DialogState> newState);
    void updateContent();
    void updateButtonVisibility();

    // �۲��߹�������
    void addObserver(TaskObserver* observer);
    void removeObserver(TaskObserver* observer);
    void notifyTaskAssigned();
    void notifyTaskProgressUpdated();
    void notifyTaskCompleted();

    // ���߷���
    std::string GetDialogContent(std::string path);
    static Dialog* create(std::string& Username);
    bool init();

    // ��ť�¼��������� (�Ѽ�)
    void onContinueButtonClick(Ref* obj);
    void onEndButtonClick(Ref* obj);
};

#endif // __DIALOG_H__