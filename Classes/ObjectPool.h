#ifndef __OBJECT_POOL_H__
#define __OBJECT_POOL_H__

#include "cocos2d.h"
#include <vector>
#include <functional>
#include <memory>

USING_NS_CC;

/**
 * @brief 对象池化接口
 * 
 * 所有需要被对象池管理的类都应该实现此接口
 * 提供 reset() 方法用于重置对象状态
 */
class IPoolable
{
public:
    virtual ~IPoolable() {}
    
    /**
     * @brief 重置对象状态，使其可以重新使用
     * 
     * 当对象从池中取出时，会自动调用此方法
     * 子类应该在此方法中重置所有运行时状态：
     * - HP、位置、动画状态
     * - 停止所有动作和调度器
     * - 清理事件监听器
     * - 重置标志位等
     */
    virtual void reset() = 0;
    
    /**
     * @brief 检查对象是否正在使用中
     * @return true 如果对象正在使用，false 如果对象在池中
     */
    virtual bool isInUse() const = 0;
    
    /**
     * @brief 设置对象的使用状态
     * @param inUse true 表示对象正在使用，false 表示对象已归还池中
     */
    virtual void setInUse(bool inUse) = 0;
};

/**
 * @brief 通用对象池模板类
 * 
 * 支持任意继承自 Ref 且实现 IPoolable 接口的类型
 * 适配 Cocos2d-x 的引用计数机制
 * 
 * @tparam T 对象类型，必须继承自 Ref 并实现 IPoolable
 */
template<typename T>
class ObjectPool
{
    static_assert(std::is_base_of<Ref, T>::value, 
                  "T must inherit from cocos2d::Ref");
    static_assert(std::is_base_of<IPoolable, T>::value, 
                  "T must implement IPoolable interface");
    
public:
    /**
     * @brief 对象创建函数类型
     * 用于在池为空时创建新对象
     */
    using CreatorFunc = std::function<T*()>;
    
    /**
     * @brief 构造函数
     * @param initialSize 初始池大小
     * @param maxSize 最大池大小（0 表示无限制）
     * @param creator 对象创建函数，如果为 nullptr，则使用 T::create()
     */
    ObjectPool(size_t initialSize = 10, 
               size_t maxSize = 100,
               CreatorFunc creator = nullptr)
        : _maxSize(maxSize)
        , _creator(creator)
    {
        // 预分配初始对象
        for (size_t i = 0; i < initialSize; ++i)
        {
            T* obj = createNewObject();
            if (obj)
            {
                obj->setInUse(false);
                _pool.push_back(obj);
            }
        }
    }
    
    /**
     * @brief 析构函数
     * 释放池中所有对象
     */
    ~ObjectPool()
    {
        clear();
    }
    
    /**
     * @brief 从池中获取一个对象
     * 
     * 如果池中有可用对象，则取出并重置其状态
     * 如果池为空，则创建新对象
     * 
     * @return 可用的对象指针，调用者需要 retain() 以持有引用
     */
    T* acquire()
    {
        T* obj = nullptr;
        
        // 尝试从池中获取对象
        for (auto it = _pool.begin(); it != _pool.end(); ++it)
        {
            if (!(*it)->isInUse())
            {
                obj = *it;
                _pool.erase(it);
                break;
            }
        }
        
        // 如果池中没有可用对象，创建新对象
        if (!obj)
        {
            obj = createNewObject();
        }
        
        if (obj)
        {
            // 重置对象状态
            obj->reset();
            obj->setInUse(true);
            
            // retain() 防止对象被自动释放
            // 调用者负责在不再使用时调用 returnObject()
            obj->retain();
        }
        
        return obj;
    }
    
    /**
     * @brief 将对象归还到池中
     * 
     * 对象会被标记为未使用状态
     * 如果池已满，对象会被直接释放
     * 
     * @param obj 要归还的对象指针
     */
    void returnObject(T* obj)
    {
        if (!obj)
        {
            return;
        }
        
        // 检查对象是否属于此池（可选，用于调试）
        // 在实际项目中可以添加更严格的检查
        
        // 停止所有动作和调度器
        if (Node* node = dynamic_cast<Node*>(obj))
        {
            node->stopAllActions();
            node->unscheduleAllCallbacks();
            node->removeFromParent();
        }
        
        // 标记为未使用
        obj->setInUse(false);
        
        // 如果池未满，将对象放回池中
        if (_maxSize == 0 || _pool.size() < _maxSize)
        {
            _pool.push_back(obj);
        }
        else
        {
            // 池已满，直接释放对象
            obj->release();
        }
    }
    
    /**
     * @brief 清空对象池，释放所有对象
     */
    void clear()
    {
        for (T* obj : _pool)
        {
            if (obj)
            {
                obj->release();
            }
        }
        _pool.clear();
    }
    
    /**
     * @brief 获取池中可用对象数量
     * @return 可用对象数量
     */
    size_t getAvailableCount() const
    {
        size_t count = 0;
        for (const T* obj : _pool)
        {
            if (obj && !obj->isInUse())
            {
                ++count;
            }
        }
        return count;
    }
    
    /**
     * @brief 获取池中总对象数量
     * @return 总对象数量
     */
    size_t getTotalCount() const
    {
        return _pool.size();
    }
    
    /**
     * @brief 预分配对象到池中
     * @param count 要预分配的对象数量
     */
    void preallocate(size_t count)
    {
        size_t currentSize = _pool.size();
        size_t targetSize = currentSize + count;
        
        if (_maxSize > 0 && targetSize > _maxSize)
        {
            targetSize = _maxSize;
        }
        
        for (size_t i = currentSize; i < targetSize; ++i)
        {
            T* obj = createNewObject();
            if (obj)
            {
                obj->setInUse(false);
                _pool.push_back(obj);
            }
        }
    }
    
private:
    /**
     * @brief 创建新对象
     * @return 新创建的对象指针
     */
    T* createNewObject()
    {
        T* obj = nullptr;
        
        if (_creator)
        {
            obj = _creator();
        }
        else
        {
            // 如果提供了创建函数，使用它
            // 否则返回 nullptr，需要外部提供创建函数
            // 注意：对于需要参数的 create() 方法，必须提供 creator 函数
            return nullptr;
        }
        
        if (obj)
        {
            // 创建的对象默认引用计数为 1（autorelease）
            // 我们需要 retain() 一次以持有引用
            obj->retain();
        }
        
        return obj;
    }
    
private:
    std::vector<T*> _pool;      // 对象池
    size_t _maxSize;            // 最大池大小（0 表示无限制）
    CreatorFunc _creator;        // 对象创建函数
};

#endif // __OBJECT_POOL_H__

