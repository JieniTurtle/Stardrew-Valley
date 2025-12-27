// Refactored with State Pattern
#ifndef __DIALOG_STATE_H__
#define __DIALOG_STATE_H__

#include <string>
#include <memory>

class Dialog;

class DialogState {
public:
    virtual ~DialogState() = default;
    virtual void onContinue(Dialog* dialog) = 0;
    virtual void onEnd(Dialog* dialog) = 0;
    virtual std::string getContent(const std::string& user) = 0;
    virtual bool showContinueButton() = 0;
};

class NormalState : public DialogState {
public:
    void onContinue(Dialog* dialog) override;
    void onEnd(Dialog* dialog) override;
    std::string getContent(const std::string& user) override;
    bool showContinueButton() override { return true; }
};

class TaskGivingState : public DialogState {
public:
    void onContinue(Dialog* dialog) override;
    void onEnd(Dialog* dialog) override;
    std::string getContent(const std::string& user) override;
    bool showContinueButton() override { return false; }
};

class TaskInProgressState : public DialogState {
public:
    void onContinue(Dialog* dialog) override;
    void onEnd(Dialog* dialog) override;
    std::string getContent(const std::string& user) override;
    bool showContinueButton() override { return true; }
};

class TaskCompletedState : public DialogState {
public:
    void onContinue(Dialog* dialog) override;
    void onEnd(Dialog* dialog) override;
    std::string getContent(const std::string& user) override;
    bool showContinueButton() override { return false; }
};

#endif // __DIALOG_STATE_H__