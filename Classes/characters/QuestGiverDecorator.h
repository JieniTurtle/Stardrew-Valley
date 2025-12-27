// Refactored with Decorator Pattern
#ifndef __QUESTGIVER_DECORATOR_H__
#define __QUESTGIVER_DECORATOR_H__

#include "NPCDecorator.h"
#include "Task.h"

// QuestGiverDecorator - 任务给予装饰器
class QuestGiverDecorator : public NPCDecorator {
private:
    Task* quest_;  // NPC可以给予的任务
    bool questCompleted_;
    
public:
    QuestGiverDecorator(INPC* npc, Task* quest) 
        : NPCDecorator(npc), quest_(quest), questCompleted_(false) {}
    
    virtual ~QuestGiverDecorator() {
        // 不删除quest_，它可能由TaskManager管理
    }
    
    virtual void interact() override {
        // 先执行基础交互
        NPCDecorator::interact();
        
        // 添加任务给予逻辑
        if (!questCompleted_ && quest_ != nullptr) {
            giveQuest();
        }
    }
    
    virtual Task* getTask() override {
        return quest_;
    }
    
    virtual bool hasTask() const override {
        return quest_ != nullptr && !questCompleted_;
    }
    
    void giveQuest() {
        // 给予任务的逻辑
        // 例如：显示任务对话框，将任务添加到任务管理器
        if (quest_) {
            // 这里可以添加任务给予的具体实现
            // 比如显示任务提示，更新任务状态等
        }
    }
    
    void completeQuest() {
        questCompleted_ = true;
        // 可以增加友好度作为奖励
        setRelation(getRelation() + 10);
    }
    
    bool isQuestCompleted() const {
        return questCompleted_;
    }
};

#endif // __QUESTGIVER_DECORATOR_H__

