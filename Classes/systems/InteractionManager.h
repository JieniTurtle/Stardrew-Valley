// Refactored with Object Pool Pattern
#ifndef __INTERACTION_MANAGER_H__
#define __INTERACTION_MANAGER_H__

#include "cocos2d.h"
#include "ObjectPool.h"
#include "explore.h"
#include <vector>

USING_NS_CC;

/**
 * @brief Interaction 管理器（使用对象池重构后）
 * 
 * 管理所有 Interaction 及其子类（Bridge, Chest, GobangBoard）
 */
class InteractionManager
{
public:
    static InteractionManager* getInstance();
    static void destroyInstance();
    
    /**
     * @brief 初始化管理器
     * @param initialPoolSize 初始池大小
     * @param maxPoolSize 最大池大小
     */
    void init(size_t initialPoolSize = 5, size_t maxPoolSize = 20);
    
    /**
     * @brief 获取一个 Interaction 对象（从池中）
     * @param map 地图指针
     * @return Interaction 对象指针
     */
    Interaction* acquireInteraction(TMXTiledMap* map);
    
    /**
     * @brief 获取一个 Bridge 对象（从池中）
     * @param map 地图指针
     * @return Bridge 对象指针
     */
    Bridge* acquireBridge(TMXTiledMap* map);
    
    /**
     * @brief 获取一个 Chest 对象（从池中）
     * @param map 地图指针
     * @param ID Chest ID
     * @return Chest 对象指针
     */
    Chest* acquireChest(TMXTiledMap* map, int ID);
    
    /**
     * @brief 获取一个 GobangBoard 对象（从池中）
     * @param map 地图指针
     * @return GobangBoard 对象指针
     */
    GobangBoard* acquireGobangBoard(TMXTiledMap* map);
    
    /**
     * @brief 归还 Interaction 对象到池中
     * @param interaction 要归还的对象指针
     */
    void returnInteraction(Interaction* interaction);
    
    /**
     * @brief 清理所有对象池
     */
    void clearAll();
    
    /**
     * @brief 获取统计信息
     */
    void printStatistics() const;
    
private:
    InteractionManager();
    ~InteractionManager();
    
    static InteractionManager* _instance;
    
    // 对象池
    ObjectPool<Interaction>* _interactionPool;
    ObjectPool<Bridge>* _bridgePool;
    ObjectPool<Chest>* _chestPool;
    ObjectPool<GobangBoard>* _gobangBoardPool;
    
    // 跟踪活跃对象
    std::vector<Interaction*> _activeInteractions;
    std::vector<Bridge*> _activeBridges;
    std::vector<Chest*> _activeChests;
    std::vector<GobangBoard*> _activeGobangBoards;
};

#endif // __INTERACTION_MANAGER_H__

