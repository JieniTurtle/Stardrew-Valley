// Refactored with Object Pool Pattern
#ifndef __NPC_MANAGER_H__
#define __NPC_MANAGER_H__

#include "cocos2d.h"
#include "ObjectPool.h"
#include "NPC.h"
#include <vector>
#include <string>
#include <map>

USING_NS_CC;

/**
 * @brief NPC 管理器（使用对象池重构后）
 * 
 * 展示如何使用 ObjectPool<T> 来管理 NPC 对象
 */
class NPCManager
{
public:
    static NPCManager* getInstance();
    static void destroyInstance();
    
    /**
     * @brief 初始化管理器
     * @param initialPoolSize 初始池大小
     * @param maxPoolSize 最大池大小
     */
    void init(size_t initialPoolSize = 10, size_t maxPoolSize = 50);
    
    /**
     * @brief 获取一个 NPC 对象（从池中）
     * @param filename NPC 文件名（如 "Willy", "Gus" 等）
     * @return NPC 对象指针，调用者负责在不再使用时调用 returnNPC()
     */
    NPC* acquireNPC(const std::string& filename);
    
    /**
     * @brief 归还 NPC 对象到池中
     * @param npc 要归还的 NPC 指针
     */
    void returnNPC(NPC* npc);
    
    /**
     * @brief 清理所有对象池
     */
    void clearAll();
    
    /**
     * @brief 获取统计信息
     */
    void printStatistics() const;
    
private:
    NPCManager();
    ~NPCManager();
    
    static NPCManager* _instance;
    
    // 对象池：使用 lambda 函数来创建带参数的对象
    // 使用 map 存储不同名称的 NPC 池（因为不同 NPC 可能有不同的创建参数）
    std::map<std::string, ObjectPool<NPC>*> _npcPools;
    
    // 跟踪活跃对象
    std::vector<NPC*> _activeNPCs;
    
    // 默认池大小
    size_t _initialPoolSize;
    size_t _maxPoolSize;
};

#endif // __NPC_MANAGER_H__

