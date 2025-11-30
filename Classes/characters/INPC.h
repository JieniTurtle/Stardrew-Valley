#ifndef __INPC_H__
#define __INPC_H__

#include "cocos2d.h"
#include "Task.h"

// Forward declarations
class Dialog;
class TMXTiledMap;
class MainCharacter;

USING_NS_CC;

// INPC接口 - NPC的抽象组件接口
class INPC {
public:
    virtual ~INPC() = default;
    
    // 核心交互方法
    virtual void interact() = 0;  // 与NPC交互
    virtual std::string getName() const = 0;
    
    // 任务相关（可能返回nullptr）
    virtual Task* getTask() = 0;
    virtual bool hasTask() const = 0;
    
    // 对话相关
    virtual Dialog* getDialog() = 0;
    virtual void showDialog() = 0;
    
    // 友好度相关
    virtual int getRelation() const = 0;
    virtual void setRelation(int value) = 0;
    
    // 移动和动画
    virtual void setPath(const std::vector<Vec2>& path) = 0;
    virtual void playAnimation(const std::string& direction) = 0;
    virtual void stopMovement() = 0;
    virtual void startMovement() = 0;
    
    // 位置检测
    virtual bool JudgeClickNPC(Vec2 clickPos, int mapscale) = 0;
    
    // 获取底层Sprite对象（用于添加到场景）
    virtual Sprite* getSprite() = 0;
    
    // 地图相关
    virtual void setMap(TMXTiledMap* map) = 0;
    virtual TMXTiledMap* getMap() = 0;
};

#endif // __INPC_H__

