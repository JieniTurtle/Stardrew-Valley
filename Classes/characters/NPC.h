#ifndef __NPC_SCENE_H__
#define __NPC_SCENE_H__
#include "BaseNPC.h"
#include "Task.h"

USING_NS_CC;

// NPC类 - 继承BaseNPC，支持装饰器模式
// 保留任务功能以保持向后兼容
class NPC : public BaseNPC
{
public:
    // NPC任务（保留以保持向后兼容，也可以通过装饰器添加）
    Task NPCtask;

    // 构造函数和析构函数
    NPC() {}
    ~NPC() {
        // BaseNPC的析构函数会处理动画释放
    }

    // 静态创建方法
    static NPC* create(const std::string& filename);

    // 重写INPC接口方法以支持任务功能
    virtual Task* getTask() override;
    virtual bool hasTask() const override;

    // 为了向后兼容，提供对NPCname的公共访问
    // BaseNPC中NPCname是private，这里通过getName()访问
    // 如果需要直接访问，可以添加访问器或使用getName()
};
#endif // __NPC_SCENE_H__;
