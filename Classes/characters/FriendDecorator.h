// Refactored with Decorator Pattern
#ifndef __FRIEND_DECORATOR_H__
#define __FRIEND_DECORATOR_H__

#include "NPCDecorator.h"

// FriendDecorator - 友好度装饰器
class FriendDecorator : public NPCDecorator {
private:
    int friendshipLevel_;  // 友好度等级（0-5）
    
    int calculateFriendshipLevel(int relation) {
        if (relation < 20) return 0;
        else if (relation < 50) return 1;
        else if (relation < 100) return 2;
        else if (relation < 200) return 3;
        else if (relation < 500) return 4;
        else return 5;
    }
    
public:
    FriendDecorator(INPC* npc) : NPCDecorator(npc) {
        friendshipLevel_ = calculateFriendshipLevel(getRelation());
    }
    
    virtual void interact() override {
        NPCDecorator::interact();
        
        // 根据友好度等级提供不同交互
        if (friendshipLevel_ >= 3) {
            // 高友好度：可以赠送礼物
            enableGiftGiving();
        }
    }
    
    void increaseFriendship(int amount) {
        int newRelation = getRelation() + amount;
        setRelation(newRelation);
        friendshipLevel_ = calculateFriendshipLevel(newRelation);
    }
    
    int getFriendshipLevel() const {
        return friendshipLevel_;
    }
    
    bool canGiveGift() const {
        return friendshipLevel_ >= 3;
    }
    
    void updateFriendshipLevel() {
        friendshipLevel_ = calculateFriendshipLevel(getRelation());
    }
    
private:
    void enableGiftGiving() {
        // 启用礼物赠送功能
        // 这里可以添加礼物赠送的具体实现
    }
};

#endif // __FRIEND_DECORATOR_H__

