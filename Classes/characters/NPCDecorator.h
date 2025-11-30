#ifndef __NPCDECORATOR_H__
#define __NPCDECORATOR_H__

#include "INPC.h"

// NPCDecorator抽象装饰器基类
class NPCDecorator : public INPC {
protected:
    INPC* npc_;  // 被装饰的NPC组件
    
public:
    NPCDecorator(INPC* npc) : npc_(npc) {
        CC_ASSERT(npc_ != nullptr);
    }
    
    virtual ~NPCDecorator() {
        // 注意：不删除npc_，因为它可能被其他装饰器共享
    }
    
    // 默认实现：委托给被装饰对象
    virtual void interact() override {
        npc_->interact();
    }
    
    virtual std::string getName() const override {
        return npc_->getName();
    }
    
    virtual Task* getTask() override {
        return npc_->getTask();
    }
    
    virtual bool hasTask() const override {
        return npc_->hasTask();
    }
    
    virtual Dialog* getDialog() override {
        return npc_->getDialog();
    }
    
    virtual void showDialog() override {
        npc_->showDialog();
    }
    
    virtual int getRelation() const override {
        return npc_->getRelation();
    }
    
    virtual void setRelation(int value) override {
        npc_->setRelation(value);
    }
    
    virtual void setPath(const std::vector<Vec2>& path) override {
        npc_->setPath(path);
    }
    
    virtual void playAnimation(const std::string& direction) override {
        npc_->playAnimation(direction);
    }
    
    virtual void stopMovement() override {
        npc_->stopMovement();
    }
    
    virtual void startMovement() override {
        npc_->startMovement();
    }
    
    virtual bool JudgeClickNPC(Vec2 clickPos, int mapscale) override {
        return npc_->JudgeClickNPC(clickPos, mapscale);
    }
    
    virtual Sprite* getSprite() override {
        return npc_->getSprite();
    }
    
    virtual void setMap(TMXTiledMap* map) override {
        npc_->setMap(map);
    }
    
    virtual TMXTiledMap* getMap() override {
        return npc_->getMap();
    }
};

#endif // __NPCDECORATOR_H__

