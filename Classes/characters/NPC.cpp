#include "NPC.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

// 注意：静态成员变量已在BaseNPC中定义，这里不再重复定义

NPC* NPC::create(const std::string& filename)
{
    NPC* npc = new NPC();
    if (npc) {
        // 设置NPC名称（在init之前，因为BaseNPC::init需要它）
        npc->NPCname = filename;
        if (npc->init()) {
            npc->autorelease();
            return npc;
        }
    }
    CC_SAFE_DELETE(npc);
    return nullptr;
}

// 初始化方法 - 调用基类的init
bool NPC::init()
{
    // 调用BaseNPC::init()来初始化所有基础功能
    if (!BaseNPC::init()) {
        return false;
    }
    
    // NPC特定的初始化可以在这里添加
    // 目前只需要调用基类的init即可
    
    return true;
}

// 重写getTask方法以返回NPCtask
Task* NPC::getTask()
{
    // 如果NPCtask有内容，返回它；否则返回nullptr
    // 检查任务名称是否为空来判断任务是否有效
    if (!NPCtask.name.empty()) {
        return &NPCtask;
    }
    return nullptr;
}

// 重写hasTask方法
bool NPC::hasTask() const
{
    // 判断NPCtask是否有有效内容
    // 检查任务名称是否为空
    return !NPCtask.name.empty();
}
