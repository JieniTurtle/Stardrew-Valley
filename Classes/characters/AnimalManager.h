// Refactored with Object Pool Pattern
#ifndef __ANIMAL_MANAGER_H__
#define __ANIMAL_MANAGER_H__

#include "cocos2d.h"
#include "ObjectPool.h"
#include "animals.h"
#include <vector>
#include <string>

USING_NS_CC;

/**
 * @brief 动物管理器（使用对象池重构后）
 * 
 * 展示如何使用 ObjectPool<T> 来管理动物对象
 * 替代原来的直接 create() 方式
 */
class AnimalManager
{
public:
    static AnimalManager* getInstance();
    static void destroyInstance();
    
    /**
     * @brief 初始化管理器
     * @param initialPoolSize 初始池大小
     * @param maxPoolSize 最大池大小
     */
    void init(size_t initialPoolSize = 5, size_t maxPoolSize = 20);
    
    /**
     * @brief 获取一个 Cow 对象（从池中）
     * @param filename 动物文件名（如 "cow"）
     * @return Cow 对象指针，调用者负责在不再使用时调用 returnCow()
     */
    Cow* acquireCow(const std::string& filename = "cow");
    
    /**
     * @brief 归还 Cow 对象到池中
     * @param cow 要归还的 Cow 指针
     */
    void returnCow(Cow* cow);
    
    /**
     * @brief 获取一个 Sheep 对象（从池中）
     * @param filename 动物文件名（如 "sheep"）
     * @return Sheep 对象指针
     */
    Sheep* acquireSheep(const std::string& filename = "sheep");
    
    /**
     * @brief 归还 Sheep 对象到池中
     * @param sheep 要归还的 Sheep 指针
     */
    void returnSheep(Sheep* sheep);
    
    /**
     * @brief 获取一个 Chicken 对象（从池中）
     * @param filename 动物文件名（如 "chicken"）
     * @return Chicken 对象指针
     */
    Chicken* acquireChicken(const std::string& filename = "chicken");
    
    /**
     * @brief 归还 Chicken 对象到池中
     * @param chicken 要归还的 Chicken 指针
     */
    void returnChicken(Chicken* chicken);
    
    /**
     * @brief 清理所有对象池
     */
    void clearAll();
    
    /**
     * @brief 获取统计信息
     */
    void printStatistics() const;
    
private:
    AnimalManager();
    ~AnimalManager();
    
    static AnimalManager* _instance;
    
    // 对象池：使用 lambda 函数来创建带参数的对象
    ObjectPool<Cow>* _cowPool;
    ObjectPool<Sheep>* _sheepPool;
    ObjectPool<Chicken>* _chickenPool;
    
    // 跟踪活跃对象（可选，用于调试）
    std::vector<Cow*> _activeCows;
    std::vector<Sheep*> _activeSheep;
    std::vector<Chicken*> _activeChickens;
};

#endif // __ANIMAL_MANAGER_H__

