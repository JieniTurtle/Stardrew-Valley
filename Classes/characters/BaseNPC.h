#ifndef __BASENPC_H__
#define __BASENPC_H__

#include "INPC.h"
#include "cocos2d.h"

// Forward declaration
class Dialog;

USING_NS_CC;

// BaseNPC类 - 具体组件，实现INPC接口
// 将现有NPC类重构为基础组件，保留核心功能
class BaseNPC : public INPC, public cocos2d::Sprite {
protected:
    // 允许子类访问这些成员以支持继承和向后兼容
    std::string NPCname;
    int relation;  // 基础友好度
    Dialog* Dialog_NPC;  // 对话
    
private:
    Sprite* animate_sprite;
    TMXTiledMap* NPCmap;
    
    // 动画
    Animate* movestatic;
    Animate* moveup;
    Animate* moveleft;
    Animate* moveright;
    Animate* movedown;
    
    // 移动相关
    int currentPathIndex;
    float speed;
    std::vector<Vec2> NPCpath;
    
    // NPC尺寸和动画顺序（静态成员）
    static int NPCsize_x;
    static int NPCsize_y;
    static int NPCorder_up;
    static int NPCorder_left;
    static int NPCorder_right;
    static int NPCorder_down;
    
public:
    static BaseNPC* create(const std::string& filename);
    bool init();
    
    ~BaseNPC() {
        if (moveup) moveup->release();
        if (moveleft) moveleft->release();
        if (moveright) moveright->release();
        if (movedown) movedown->release();
        if (movestatic) movestatic->release();
    }
    
    // 实现INPC接口
    virtual void interact() override;
    virtual std::string getName() const override { return NPCname; }
    virtual Task* getTask() override { return nullptr; }  // 基础NPC无任务
    virtual bool hasTask() const override { return false; }
    virtual Dialog* getDialog() override { return Dialog_NPC; }
    virtual void showDialog() override;
    virtual int getRelation() const override { return relation; }
    virtual void setRelation(int value) override { relation = value; }
    virtual void setPath(const std::vector<Vec2>& path) override;
    virtual void playAnimation(const std::string& direction) override;
    virtual void stopMovement() override;
    virtual void startMovement() override;
    virtual bool JudgeClickNPC(Vec2 clickPos, int mapscale) override;
    virtual Sprite* getSprite() override { return this; }
    virtual void setMap(TMXTiledMap* map) override { NPCmap = map; }
    virtual TMXTiledMap* getMap() override { return NPCmap; }
    
    // 内部方法
    void updatemove(float dt);
    void updatestatic(float dt);
};

#endif // __BASENPC_H__

